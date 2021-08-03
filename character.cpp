#include "character.hpp"
#include "tilemap.hpp"
#include "utils.hpp"

#include <map>
#include <list>
#include <iostream>

// node for use in path finding
struct Node {
    Node::Node(const sf::Vector2i& pos, Node* parent, int cost, int priority) :
        pos(pos),
        parent(parent),
        cost(cost),
        priority(priority)
    {}

    sf::Vector2i pos;

    Node* parent;

    int cost;
    int priority;
    bool isPath = false;
};

// returns cost between two nodes (edge on a graph)
int getCost(const TileMap& tileMap, const Node& a, const Node& b) {
    if (tileMap.isOpaque(b.pos.x, b.pos.y))
        return 1000;
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
    if (!pathHead)
        return;
    
    // find delta between next path node and current position
    sf::Vector2f delta = sf::Vector2f(pathHead->pos) - getPosition();
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
        Node* oldHead = pathHead;
        pathHead = pathHead->parent;
        delete oldHead;
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

void Character::findPath(const TileMap& tileMap, sf::Vector2i target) {
    // reset path
    while (pathHead != nullptr) {
        Node* parent = pathHead->parent;
        delete pathHead;
        pathHead = parent;
    }

    // A* algorithm
    // adapted from https://www.redblobgames.com/pathfinding/a-star/introduction.html

    std::list<Node*> allNodes;

    // create frontier priority queue
    std::priority_queue<Node*, std::vector<Node*>, NodeComparison> frontier;
    // add start node;
    Node* start = new Node(Utils::snapToTile(getPosition()), nullptr, 0, 0);
    frontier.push(start);
    allNodes.push_back(start);
    //std::cout << "start pos: (" << start->pos.x << ", " << start->pos.y << ')' << std::endl;
    // go through frontier nodes until empty
    while (!frontier.empty() && frontier.size()<=200 && frontier.top()->cost <= 2000) {
        Node& cur = *frontier.top(); // get current node from best candidate
        frontier.pop();
        /*std::cout << "exploring: (" << cur.pos.x << ", " << cur.pos.y << ") P: " << cur.priority
            << ", T: (" << target.x << ", " << target.y << ") " << std::endl;*/

        // set path if current node is on target position
        if (cur.pos == target) {
            pathHead = &cur;
            break;
        }

        // loop through all neighbors of current node
        std::list<Node> neighbors;
        neighbors.push_back({ cur.pos + sf::Vector2i({ 32, 0 }), &cur, 0, 99 }); // add right node
        neighbors.push_back({ cur.pos + sf::Vector2i({ 0, -32 }), &cur, 0, 99 }); // add top node
        neighbors.push_back({ cur.pos + sf::Vector2i({ -32, 0 }), &cur, 0, 99 }); // add left node
        neighbors.push_back({ cur.pos + sf::Vector2i({ 0, 32 }), &cur, 0, 99 }); // add right node
        for (Node& next : neighbors) {
            int newCost = cur.cost + getCost(tileMap, cur, next);
            if (next.cost == 0 || newCost < next.cost) { // if next is new node or new path is less expensive than old path to next
                Node* newNode = new Node(next.pos, next.parent, newCost, newCost + heuristic(target, next));
                frontier.push(newNode);
                allNodes.push_back(newNode);
            }
        }
    }

    if (!pathHead)
        return;

    // set isPath flag for path nodes and reverse path
    Node* pLastNode = nullptr;
    for (;;) {
        pathHead->isPath = true;
        Node* oldParent = pathHead->parent;
        pathHead->parent = pLastNode;
        pLastNode = pathHead;
        if (oldParent != nullptr)
            pathHead = oldParent;
        else
            break;
    }

    // delete useless nodes
    for (Node* node : allNodes) {
        if (!node->isPath)
            delete node;
    }

    // ignore head node if further from target than current position
    float distCur = Utils::pointDistance(getPosition(), { (float)target.x, (float)target.y });
    float distHead;
    do {
        distHead = Utils::pointDistance(sf::Vector2f(pathHead->pos), { (float)target.x, (float)target.y });
        Node* oldHead = pathHead;
        pathHead = pathHead->parent;
        delete oldHead;
    } while (distHead > distCur && pathHead != nullptr);

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