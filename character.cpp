#include "character.hpp"
#include "tilemap.hpp"
#include "utils.hpp"

#include <unordered_map>
#include <list>
#include <iostream>

// max distance for pathing (in tiles)
#define MAX_DIST 100

Node* createPath(const TileMap* tileMap, sf::Vector2i start, sf::Vector2i target);

// returns cost between two nodes (edge on a graph)
int getCost(const TileMap& tileMap, const Node& a, const Node& b) {
    if (tileMap.isOpaque(a.pos.x, a.pos.y))
        return MAX_DIST;
    if (tileMap.isOpaque(b.pos.x, b.pos.y))
        return MAX_DIST;
    return 1;
}

// returns priority of node depending on distance from target
int heuristic(const sf::Vector2i& target, const Node& node) {
    // manhattan distance on a square grid
    return (abs(target.x - node.pos.x) + abs(target.y - node.pos.y))/32;
}

// A* Node comparison
class NodeComparison {
public:
    bool operator() (Node* lhs, Node* rhs) const {
        return lhs->priority > rhs->priority;
    }
};

void Character::moveOnPath(const TileMap& tileMap) {
    // check for path to follow
    if (!isOnPath())
            return;

    Node* head = pathHead;
    
    // find delta between next path node and current position
    sf::Vector2f delta = sf::Vector2f(head->pos) - getPosition();
    float len = sqrt(powf(delta.x, 2) + powf(delta.y, 2));
    sf::Vector2f moveVector = sf::Vector2f(delta.x / len, delta.y / len);

    setAnimSpeed(12);

    // move when free
    if (tileMap.areaClear(*this, moveVector.x, 0))
        move(moveVector.x, 0);
    if (tileMap.areaClear(*this, 0, moveVector.y))
        move(0, moveVector.y);

    if (moveVector.x > 0)
        direction = 0;
    else
        direction = 1;

    // reached node in path, move to next node
    if (len <= 2) {
        //std::cout << "reached node. New pathhead: " << pathHead->parent << std::endl;
        pathHead = head->parent;
        delete head;
    }
}

void Character::moveTowards(const TileMap& tileMap, sf::Vector2f target) {
    // find delta between next path node and current position
    sf::Vector2f delta = target - getPosition();
    float len = sqrt(powf(delta.x, 2) + powf(delta.y, 2));
    sf::Vector2f moveVector = sf::Vector2f(delta.x / len, delta.y / len);

    setAnimSpeed(12);

    // move when free
    if (tileMap.areaClear(*this, moveVector.x, 0))
        move(moveVector.x, 0);
    if (tileMap.areaClear(*this, 0, moveVector.y))
        move(0, moveVector.y);

    if (moveVector.x > 0)
        direction = 0;
    else
        direction = 1;
}

int hashPos(const TileMap& tileMap, sf::Vector2i pos) {
    return (pos.x / 32) + (pos.y / 32) * tileMap.mapWidth;
}

void Character::updatePath() {
    if (pathRetrieved)
        return;

    bool futureIsValid = false;
    try {
        futureIsValid = nextPathHead.valid();
    }
    catch (std::exception e) {
        std::cout << "future valid error" << std::endl;
        return;
    }

    // check if new path available
    if (futureIsValid) {
        if (nextPathHead.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready) {
            // new path is available
            isPathing = false;
            
            // reset path
            while (pathHead != nullptr) {
                Node* parent = pathHead->parent;
                delete pathHead;
                pathHead = parent;
            }

            pathHead = nextPathHead.get();

            // we have retrieved the path
            pathRetrieved = true;

            // delete the first head for smoother path adoption
            if (pathHead) {
                Node* parent = pathHead->parent;
                delete pathHead;
                pathHead = parent;
            }
        }
    }
}

void Character::findPath(const TileMap& tileMap, sf::Vector2i target) {    
    pathRetrieved = false;

    if (isPathing)
        return;

    if (!nextPathHead.valid()) {
        isPathing = true;

        // get the start position of charcter
        sf::Vector2i start = Utils::snapToTile(getPosition() + sf::Vector2f({ 16.f, 16.f }));
        // create and run new async path finding task
        nextPathHead = std::async(createPath, &tileMap, start, target);

        // we have new path being created
        pathRetrieved = false;
    }
}

bool Character::isOnPath() {
    updatePath();
    return (pathHead != nullptr);
}

Node* createPath(const TileMap* tileMap, sf::Vector2i start, sf::Vector2i target) {
    Node* head = nullptr;

    // A* algorithm
    // adapted from https://www.redblobgames.com/pathfinding/a-star/introduction.html

    std::list<Node*> allNodes;
    std::unordered_map<int, bool> explored;

    // create frontier priority queue
    std::priority_queue<Node*, std::vector<Node*>, NodeComparison> frontier;
    // add start node;
    Node* startNode = new Node(start, nullptr, 0, 0);
    frontier.push(startNode);
    allNodes.push_back(startNode);
    //std::cout << "start pos: (" << start->pos.x << ", " << start->pos.y << ')' << std::endl;
    // go through frontier nodes until empty
    while (!frontier.empty() && frontier.size() <= 300 && frontier.top()->cost <= MAX_DIST) {
        Node& cur = *frontier.top(); // get current node from best candidate
        frontier.pop();

        //std::cout << "exploring: (" << cur.pos.x << ", " << cur.pos.y << ") P: " << cur.priority << ", T: (" << target.x << ", " << target.y << ") " << std::endl;

        // set path if current node is on target position
        if (cur.pos == target) {
            head = &cur;
            break;
        }

        // loop through all neighbors of current node
        std::list<Node> neighbors;
        neighbors.push_back({ cur.pos + sf::Vector2i({ 32, 0 }), &cur, 0, 99 }); // add right node
        neighbors.push_back({ cur.pos + sf::Vector2i({ 0, -32 }), &cur, 0, 99 }); // add top node
        neighbors.push_back({ cur.pos + sf::Vector2i({ -32, 0 }), &cur, 0, 99 }); // add left node
        neighbors.push_back({ cur.pos + sf::Vector2i({ 0, 32 }), &cur, 0, 99 }); // add right node
        for (Node& next : neighbors) {
            // check to see if already explored
            int hashKey = hashPos(*tileMap, next.pos);
            auto itr = explored.find(hashKey);
            if (itr == explored.end())
                explored[hashKey] = true;
            else
                continue;

            int newCost = cur.cost + getCost(*tileMap, cur, next);
            if (next.cost == 0 || newCost < next.cost) { // if next is new node or new path is less expensive than old path to next
                Node* newNode = new Node(next.pos, next.parent, newCost, newCost + heuristic(target, next));

                frontier.push(newNode);
                allNodes.push_back(newNode);
            }
        }
    }

    // set isPath flag for path nodes and reverse path
    Node* pLastNode = nullptr;
    if (head) {
        for (;;) {
            head->isPath = true;
            Node* nextNode = head->parent;
            head->parent = pLastNode;
            pLastNode = head;

            if (nextNode)
                head = nextNode;
            else
                break;
        }
    }

    // delete useless nodes
    for (Node* node : allNodes) {
        if (!node->isPath)
            delete node;
    }

    return head;

    /*std::cout << "Found path to target: \n\t";
    Node* pNode = pathHead;
    while (pNode != nullptr) {
        std::cout << pNode << " -> ";
        pNode = pNode->parent;
    }
    std::cout << "\n\t";
    pNode = pathHead;
    while (pNode != nullptr) {
        std::cout << "(" << pNode->pos.x << ',' << pNode->pos.y << ") -> ";
        pNode = pNode->parent;
    }
    std::cout << std::endl;*/
}

