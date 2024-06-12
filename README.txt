SDL Pong Game : Slam Sphere
Overview:
This is a simple Pong game implemented in C using the SDL library. It allows two players to control paddles and compete against each other to hit a ball back and forth, with the goal of scoring points by getting the ball past the opponent's paddle.

Features:
Two-player gameplay
Single-player mode with AI-controlled paddle
Sound effects for ball-paddle collisions
Background music during gameplay
Title screen and game over screens
Simple graphics using SDL surfaces and textures
Images and GUI interface created using canva.

Dependencies:
SDL2
SDL_image
SDL_mixer
Make sure you have these libraries installed on your system before compiling the code.

Compilation
Compile the code using the following command:

gcc -o slam_sphere b23ee1052_b23cs1008_b23ee1041_b23me1067.c -lSDL2 -lSDL2_image -lSDL2_mixer

Usage
Run the compiled executable with optional arguments to specify fullscreen mode:

./slam_sphere             # Run the game in windowed mode
./slam_sphere -f          # Run the game in fullscreen mode

Controls:
Player 1:
Up: Up Arrow Key
Down: Down Arrow Key
Player 2 (in two-player mode):
Up: W Key
Down: S Key

Gameplay:
Press Spacebar on the title screen to start the game.
The game continues until one player scores three points.
Press Spacebar on the game over screen to return to the title screen.

Credits:
Sound effects and Background music from freesound.org.
