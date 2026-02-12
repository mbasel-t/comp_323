# Week 4 stuff

## How to run
Run "python3 game.py"

## Controls
Space to start, WASD to move

## What was changed
* Player color changes to gold for a short period after picking up a coin
* Player retains score after picking up all coins
* Coin placement RNG key iterates after picking up all coins, so that coin placement is different from before

## Gameplay loop
The gameplay loop is defined by the player picking up coins to increase their score. After all coins are picked
up, the coins are placed again, and the player continues to pick up coins. The player loses "lives" every time they hit
one of the moving hazard pieces, and the game terminates once all lives are lost.