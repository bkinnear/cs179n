#ifndef STATE_H_
#define STATE_H_

#include "game.hpp"

#include <vector>
#include <unordered_map>
#include <list>

typedef unsigned EffectID;

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
	State(Game& game) : game(game) {};
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

	/* loads effect from texture
	 * can be placed in the world with createEffect()
	 * 
	 * set animationTime for how long sprite should loop for
	 * default animationTime = 0.f makes effect delete itself after 1 loop
	*/
	EffectID loadEffect(const sf::Texture& texture, const sf::IntRect& subRect, unsigned nSubsprites, unsigned animationSpeed, float animationTime = 0.f);

	/* creates effect in world using given effect pointer
	*/
	void createEffect(EffectID effect, const sf::Vector2f& pos);

protected:
	Game& game;

private:
	// next effect ID to be used for next effect
	EffectID nextEffectID = 0;
	sf::Clock effectClock;

	// data for an effect instance
	struct Effect {
		float startTime;
		unsigned tick = 0;
		sf::Vector2f pos;
	};

	// holds effect sprite, life time, and list of effects currently on the map
	struct EffectData {
		AnimSprite effectSprite;
		float lifetime;
		std::list<Effect> effectList;
	};

	// holds raw textures - only interact through createTexture()
	std::list<sf::Texture> textures;

	// holds effects and effect data - only interact through loadEffect() and createEffect()
	std::unordered_map<EffectID, EffectData> effects;
};

#endif