# Updates & tuning log

## Meaningful change(s)

For my meaningful change, I chose to include a speed ramp where the game's movement speeds for the player accelerates based on their score, increasing by 2% per point at first. This made the game feel like it accelerated very quickly, so I changed it to 1.5%. This made the game ramping feel a lot more natural. Because this is a very quick addition, I also wanted to add a "three lives" system. To facilitate this, I added a system that grants 180 frames (three seconds) of complete invincibility to damage.

## Tweaks

For my first tweak, I chose to solve the diagonal movement problem where the cube moves much quicker when up/down and left/right are pressed. I won't explain why this is the issue because we already did in class. I remembered from my physics class, though, that to compute the x or y velocity of a movement in a 2D plane, you take the cosine or sine of the angle of the movement times the velocity. Using this knowledge, I used Google to inform myself and make each key give only 71% of the movement so long as either of the keys perpendicular to it are pressed. This made the game feel much more smooth and professional.

For my second tweak, since the higher speed was making it hard to grab some coins, I made the coins increase from 18 pixels to 25 pixels in diameter. This made it substantially easier to grab coins and eliminated the problem for scores I was playing at.