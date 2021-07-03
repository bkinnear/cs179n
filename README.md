# cs179n

*CS179N Team1 Game*

### Development Notes:

#### Terminology:

**Game:** holds the game state and window  
**State:** aka a "gamestate". Controls game logic and rendering. Holds assets.  
**sf::Window:** the window and render target in one object. Use window.draw() to draw to the render target  
**sf::View:** the "camera" of the gamestate. Determines what is drawn to the window and where  
**sf::Texture:** holds raw pixel data on the GPU  
**sf::Sprite:** a game object on or off screen associated with a texture, transformation, position, bounding box, etc.  
**AnimSprite:** an sf::Sprite with subsprites. Can be easily animated

#### Merging:

* Main should always compile and run
* Main should always have all the required resources in `./res` to run