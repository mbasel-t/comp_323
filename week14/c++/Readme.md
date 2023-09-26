### Readme - Game Dev - OOP Examples - Shooter Arena

Basic game dev example with C++ and SFML.

To build game demo,

* in root of project directory
	* create `build` directory
	* `cd` to `build` directory
	* issue command `cmake ..`
	* issue command `make`
	* start game `./STGArenaSpire`

To start game demo after opening game window,

* press **Enter** key
* then press **1** key
* mouse controls rotation of player sprite

#### Game outline

* v0.5
	* add bonus options and additions
	* detect required collisions
		* update sprites &c.
		* detect mob object and player collisions
	* update game state
	* game plays
		* requires updates for HUD, sound, game reset &c.
* V0.6
	* add text, background image &c. to views
	* add initial HUD for game score, player lives &c.
	* layer views in game window
	* ...
