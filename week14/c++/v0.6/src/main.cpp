#include <sstream>
#include <SFML/Graphics.hpp>
#include "Arena.h"
#include "Player.h"
#include "Textures.h"
#include "Projectile.h"

using namespace sf;

int main() {

	// create instance for textures - many textures from one
	Textures holder;

	// game states
	// - one of four is always active
	enum class State { PAUSED, LEVEL_UP, GAME_OVER, PLAYING };
	// set initial state for game
	State state = State::GAME_OVER;

	/* 
	* create SFML window
	*/
	// set resolution of game window
	Vector2f screenRes;
	screenRes.x = VideoMode::getDesktopMode().width;
	screenRes.y = VideoMode::getDesktopMode().height;
	// render game window
	RenderWindow window(VideoMode(screenRes.x, screenRes.y),
		"Mummies", Style::Fullscreen);

	// create SFML View for main game usage
	View mainView(sf::FloatRect(0, 0, screenRes.x, screenRes.y));

	// clock for timing 
	Clock clock;
	// record duration PLAYING state active
	Time gameTotalTime;

	// mouse relative to world coordinates
	Vector2f mouseWorldPosn;
	// mouse relative to screen coordinates
	Vector2i mouseScreenPosn;

	// instantiate player
	Player player;

	// define boundaries of game arena
	IntRect arena;

	// background
	VertexArray background;
	// load sprite sheet
	Texture textureBg = Textures::GetTexture("../assets/graphics/background_sheet.png");

	// set up many mob objects
	int numMummies;
	int numMummiesAlive;
	Mummy* mummies = nullptr;

	// initialise array for projectile objects
	Projectile projectiles[100];

	// variables for projectiles
	int currentProjectile = 0; 
	int projectilesSpare = 24; 
	int projectilesLoaded = 6; 
	int projectilesSize = 6; 
	float shootRate = 1; 
	// last time shoot called
	Time lastPressed;

	// game score and result
	int score = 0;
	int highScore = 0;

	/*
	* HUD views
	*/
	// home & game over screen
	Sprite spriteGOver;
	Texture textureGOver = Textures::GetTexture("../assets/graphics/background.png");
	spriteGOver.setTexture(textureGOver);
	spriteGOver.setPosition(0, 0);
	// head up display view
	View hudView(sf::FloatRect(0, 0, screenRes.x, screenRes.y));
	// sprite for projectile icon
	Sprite spriteProjectile;
	Texture textureProjectile = Textures::GetTexture("../assets/graphics/ammo_icon.png");
	spriteProjectile.setTexture(textureProjectile);
	spriteProjectile.setPosition(20, 980);
	// load font for rendering
	Font font;
	font.loadFromFile("../assets/fonts/Vera.ttf");
	// pause state
	Text pauseText;
	pauseText.setFont(font);
	pauseText.setCharacterSize(155);
	pauseText.setFillColor(Color::White);
	pauseText.setPosition(400, 400);
	pauseText.setString("Continue - Press Enter");
	// game over state
	Text gOverText;
	gOverText.setFont(font);
	gOverText.setCharacterSize(125);
	gOverText.setFillColor(Color::White);
	gOverText.setPosition(250, 850);
	gOverText.setString("Play - Press Enter");
	// level up state
	Text levelUpText;
	levelUpText.setFont(font);
	levelUpText.setCharacterSize(80);
	levelUpText.setFillColor(Color::White);
	levelUpText.setPosition(150, 250);
	std::stringstream levelUpStream;
	levelUpStream <<
		"1- Increased fire" <<
		"\n2- Increased projectile nos" <<
		"\n3- Increased health" <<
		"\n4- Increased run speed" <<
		"\n5- More & better health" <<
		"\n6- More & better projectiles";
	levelUpText.setString(levelUpStream.str());
	// projectiles
	Text projectileText;
	projectileText.setFont(font);
	projectileText.setCharacterSize(55);
	projectileText.setFillColor(Color::White);
	projectileText.setPosition(200, 980);
	// game score
	Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(55);
	scoreText.setFillColor(Color::White);
	scoreText.setPosition(20, 0);
	// game high score
	Text highScoreText;
	highScoreText.setFont(font);
	highScoreText.setCharacterSize(55);
	highScoreText.setFillColor(Color::White);
	highScoreText.setPosition(1400, 0);
	std::stringstream s;
	s << "High Score:" << highScore;
	highScoreText.setString(s.str());
	// mob objects remaining
	Text mummiesRemainingText;
	mummiesRemainingText.setFont(font);
	mummiesRemainingText.setCharacterSize(55);
	mummiesRemainingText.setFillColor(Color::White);
	mummiesRemainingText.setPosition(1500, 980);
	mummiesRemainingText.setString("Mummies: 100");
	// mob object Wave number
	int mobWave = 0;
	Text mobWaveText;
	mobWaveText.setFont(font);
	mobWaveText.setCharacterSize(55);
	mobWaveText.setFillColor(Color::White);
	mobWaveText.setPosition(1250, 980);
	mobWaveText.setString("Mummies Wave: 0");
	// health bar
	RectangleShape healthBar;
	healthBar.setFillColor(Color::Red);
	healthBar.setPosition(450, 980);

	// time since last update of HUD 
	int framesSinceLastUpdateHUD = 0; 
	// rate of update FPS of HUD - between updates...
	int fpsPerFrameInterval = 1000;
	
	// main game loop
	while (window.isOpen()) {

		/*
		* Input
		*/
		Event event;
		// poll for events
		while (window.pollEvent(event)) {

			if (event.type == Event::KeyPressed) {
				// pause game during gameplay
				if (event.key.code == Keyboard::Return && state == State::PLAYING) {
					state = State::PAUSED;
				} // toggle restart
				else if (event.key.code == Keyboard::Return && state == State::PAUSED) {
					state = State::PLAYING;
					// reset clock, avoids frame jump
					clock.restart();
				} // start new game
				else if (event.key.code == Keyboard::Return && state == State::GAME_OVER) {
					state = State::LEVEL_UP;
				}

				if (state == State::PLAYING) {
					// handler for reloading projectile objects
					if (event.key.code == Keyboard::R) {
						if (projectilesSpare >= projectilesSize) {
							// reload if projectiles available
							projectilesLoaded = projectilesSize;
							projectilesSpare -= projectilesSize;
						} else if (projectilesSpare > 0) {
							// if few projectiles available
							projectilesLoaded = projectilesSpare;
							projectilesSpare = 0;
						} else {

						}
					}
				}

			}
		} // end of polling

		// handle quitting game window
		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			window.close();
		}

		// handle events for gameplay - wasd layout
		if (state == State::PLAYING) {
			// handle wasd key press and release
			if (Keyboard::isKeyPressed(Keyboard::W)) {
				player.motionUp();
			} else {
				player.stopUp();
			}
			if (Keyboard::isKeyPressed(Keyboard::S)) {
				player.motionDown();
			} else {
				player.stopDown();
			}
			if (Keyboard::isKeyPressed(Keyboard::A)) {
				player.motionLeft();
			} else {
				player.stopLeft();
			}
			if (Keyboard::isKeyPressed(Keyboard::D)) {
				player.motionRight();
			} else {
				player.stopRight();
			}

			// handle mouse click for shooting a projectile
			if (Mouse::isButtonPressed(sf::Mouse::Left)) {
				if (gameTotalTime.asMilliseconds() - lastPressed.asMilliseconds() > 1000 / shootRate && projectilesLoaded > 0) {
					// args
    			// - centre of player & centre of cross-hair for targetting
					projectiles[currentProjectile].shoot(
					player.getPlayerCentre().x, player.getPlayerCentre().y,
					mouseWorldPosn.x, mouseWorldPosn.y);
					currentProjectile++;
					if (currentProjectile > 99) {
						currentProjectile = 0;
					}
					lastPressed = gameTotalTime;
					projectilesLoaded--;
				}
			} // end shooting projectile

		}

		// handle level up state - change by player
		if (state == State::LEVEL_UP) {
			if (event.key.code == Keyboard::Num1) {
				state = State::PLAYING;
			}

			// check state is PLAYING
			if (state == State::PLAYING) {
				// set the level
				arena.width = 500;
				arena.height = 500;
				arena.left = 0;
				arena.top = 0;

				// pass VertexArray by reference
				int tileSize = generateBackground(background, arena);
				
				// spawn player in middle of arena
				player.spawn(arena, screenRes, tileSize);

				// create many mob objects
				numMummies = 10;
				// delete previous allocated memory, if available
				delete[] mummies;
				mummies = createMob(numMummies, arena);
				numMummiesAlive = numMummies;

				// reset clock, avoids frame jump
				clock.restart();
			}
		} // end level up

		/*
		* Update the frame
		*/
		if (state == State::PLAYING) {
			// update delta time
			Time dt = clock.restart();
			// update total time for game
			gameTotalTime += dt;						
			// get fraction from delta time
		  float dtAsSeconds = dt.asSeconds();
			
			// check posn of mouse pointer
		  mouseScreenPosn = Mouse::getPosition();
			// update world coords for main view
			mouseWorldPosn = window.mapPixelToCoords(Mouse::getPosition(), mainView);	
			
			// update player
			player.update(dtAsSeconds, Mouse::getPosition());
			// record new player posn
			Vector2f playerPosition(player.getPlayerCentre());
			// centre view around player
			mainView.setCenter(player.getPlayerCentre());

			// iterate over each mob object and update
			for (int i = 0; i < numMummies; i++) {
				if (mummies[i].isAlive()) {
					mummies[i].update(dt.asSeconds(), playerPosition);
				}
			}

			// update projectiles in-flight
			for (int i = 0; i < 100; i++) {
				if (projectiles[i].isInFlight()) {
					projectiles[i].update(dtAsSeconds);
				}
			}

			/* 
      * Collision detection
			*/

			// check collision with mob objects
			for (int i = 0; i < 100; i++) {
				for (int j = 0; j < numMummies; j++) {
					if (projectiles[i].isInFlight() && mummies[j].isAlive()) {
						if (projectiles[i].getPosition().intersects(mummies[j].getPosn())) {
							// stop projectile
							projectiles[i].stop();
							// record hit, check if mob object terminated
							if (mummies[j].hit()) {
								// hit terminates mob object, increment score...
								score += 10;
								if (score >= highScore) {
									highScore = score;
								}
								numMummiesAlive--;
								// check if all mob objects terminated
								if (numMummiesAlive == 0) {
									state = State::LEVEL_UP;
								}
							}
						}
					}
				}
			}// end collision detection - mob objects

			// check for collisions with player
			for (int i = 0; i < numMummies; i++) {
				if (player.getPlayerPosn().intersects(mummies[i].getPosn()) && mummies[i].isAlive()) {
					if (player.hit(gameTotalTime)) {
						// more to come...
					}
					if (player.getPlayerHealth() <= 0) {
						state = State::GAME_OVER;
					}
				}
			}// end collision detection - player object

			/* 
			* HUD update
			*/
			// size health bar 
			healthBar.setSize( Vector2f( player.getPlayerHealth() * 3, 50)); 
			// increment frames since previous update 
			framesSinceLastUpdateHUD ++; 
			// re-calculate every fpsPerFrameInterval frames 
			if (framesSinceLastUpdateHUD > fpsPerFrameInterval) { 
				// update HUD text 
				std:: stringstream ssProjectiles; 
				std:: stringstream ssScore; 
				std:: stringstream ssHighScore; 
				std:: stringstream ssMobWave; 
				std:: stringstream ssMummiesAlive; 
				// update projectiles text 
				ssProjectiles << projectilesLoaded << "/" << projectilesSpare;
				projectileText.setString(ssProjectiles.str()); 
				// update score text 
				ssScore << "Score:" << score; 
				scoreText.setString(ssScore.str()); 
				// update high score text 
				ssHighScore << "High Score:" << highScore; 
				highScoreText.setString(ssHighScore.str()); 
				// update mob wave 
				ssMobWave << "Wave:" << mobWave; 
				mobWaveText.setString(ssMobWave.str()); 
				// update high score text 
				ssMummiesAlive << "Mummies:" << numMummiesAlive; 
				mummiesRemainingText.setString( ssMummiesAlive.str()); 
				framesSinceLastUpdateHUD = 0; 
			} // end HUD update

		} // end of frame update

		/*
		* Draw the scene
		*/
		if (state == State::PLAYING) {
			window.clear();
			// set main view to render in game window
			window.setView(mainView);
			// draw sprite sheet background
			window.draw(background, &textureBg);

			// switch to HUD view
			window.setView(hudView);
			// draw HUD elements
			window.draw(spriteProjectile); 
			window.draw(projectileText); 
			window.draw(scoreText); 
			window.draw(highScoreText); 
			window.draw(healthBar); 
			window.draw(mobWaveText); 
			window.draw(mummiesRemainingText);


			// draw mob objects
			for (int i = 0; i < numMummies; i++) {
				window.draw(mummies[i].getSprite());
			}
			// draw projectiles as player shoots
			for (int i = 0; i < 100; i++) {
				if (projectiles[i].isInFlight()) {
					window.draw(projectiles[i].getShape());
				}
			}

			// draw player
			window.draw(player.getPlayerSprite());
		}

		if (state == State::LEVEL_UP) {
			window.draw(spriteGOver);
			window.draw(levelUpText);
		}

		if (state == State::PAUSED) {
			window.draw(pauseText);
		}

		if (state == State::GAME_OVER) {
			window.draw(spriteGOver);
			window.draw(gOverText);
			window.draw(scoreText);
			window.draw(highScoreText);
		}

		window.display();

	}// end game loop

	// delete memory allocation, where appropriate...
	delete[] mummies;

	return 0;
}
