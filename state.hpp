#ifndef STATE_H_
#define STATE_H_

#include "game.hpp"

#include <vector>
#include <unordered_map>
#include <list>

typedef AnimSprite Effect;

/* Abstract game state object
 * 
 * Game states define behavior for a given "scene" in the game
 * Ex: menu, story mode, endless mode, end screen, etc.
 * 
 * Game states load their resources on creation and free on deletion (RAII)
 * Changing game state can be done through Game::setState(State* newState)
 * 
 * To create a new state, create a child object of State and
 * define logic() and render() in that child
*/
class State {
public:
	State(Game& game): game(game) {};
	virtual ~State() {};

	/* defines the game logic of the gamestate */
	virtual void logic() = 0;

	/* updates effects and deletes expired effects */
	void updateEffects();

	/* draws effects to screen */
	void drawEffects();

	/* defines how to render the gamestate */
	virtual void render() = 0;

	/* creates texture from image file
	 * loads file and takes the entire image
	 * texture is stored in the state, and freed upon state dealloc
	 * returns ref to texture
	*/
	sf::Texture& createTexture(const std::string& fname);

	/* creates texture from image file
	 * loads file and takes the subimage specified by a rect
	 * texture is stored in the state, and freed upon state dealloc
	 * returns ref to texture
	*/
	sf::Texture& createTexture(const std::string& fname, sf::IntRect src);

	/* creates effect sprite from texture
	 * can now be placed in the world with createEffect()
	*/
	Effect* loadEffect(const sf::Texture& texture, const sf::IntRect& subRect, unsigned nSubsprites, unsigned animationSpeed);

	/* creates effect in world using given effect pointer
	*/
	void createEffect(Effect* effect, const sf::Vector2f& pos);

protected:
	Game& game;

private:
	// holds raw textures - only interact through createTexture()
	std::list<sf::Texture> textures;

	// holds effects - only interact through loadEffect() and createEffect()
	std::list<AnimSprite> effectSprites;
	std::unordered_map<AnimSprite*, std::pair<unsigned, sf::Vector2f>> effects;
};

#endif