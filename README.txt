
# SDL Pong Game: Slam Sphere

## Overview
Slam Sphere is a simple Pong game implemented in C using the SDL (Simple DirectMedia Layer) library. It allows two players to control paddles and compete against each other to hit a ball back and forth, with the goal of scoring points by getting the ball past the opponent's paddle.

## Features
- **Two-player gameplay**: Play against a friend or AI.
- **Single-player mode**: Compete against an AI-controlled paddle.
- **Sound effects**: Enjoy sound effects for ball-paddle collisions.
- **Background music**: Immerse yourself in gameplay with background music.
- **User interface**: Title screen and game over screens for an engaging experience.
- **Simple graphics**: Utilize SDL surfaces and textures for basic visuals.
- **Custom images and GUI**: Created using Canva for a polished look.

## Dependencies
Make sure you have the following libraries installed on your system before compiling the code:
- SDL2
- SDL_image
- SDL_mixer

## Compilation
To compile the code, use the following command:
```bash
gcc -o slam_sphere b23ee1052_b23cs1008_b23ee1041_b23me1067.c -lSDL2 -lSDL2_image -lSDL2_mixer
```

## Usage
Run the compiled executable with optional arguments to specify fullscreen mode:
```bash
./slam_sphere             # Run the game in windowed mode
./slam_sphere -f          # Run the game in fullscreen mode
```

## Controls
- **Player 1**:
  - Up: Up Arrow Key
  - Down: Down Arrow Key
- **Player 2 (in two-player mode)**:
  - Up: W Key
  - Down: S Key

## Gameplay
1. Press **Spacebar** on the title screen to start the game.
2. The game continues until one player scores three points.
3. Press **Spacebar** on the game over screen to return to the title screen.

## Code Structure
1. **Initialization**: The code initializes SDL, SDL_image, and SDL_mixer libraries, creates a window and renderer, and sets up necessary surfaces and textures.
2. **Structures**: Defines structures for the ball and paddles to manage their positions and dimensions.
3. **Global Variables**: Declares global variables for the game elements, such as the ball, paddles, score, window, renderer, and audio files.
4. **Functions**: Handles different aspects of the game:
   - `init_background_music()`: Initializes background music for different screens.
   - `init_game()`: Initializes the starting position and sizes of game elements.
   - `check_score()`: Checks if any player has reached the score limit to declare a winner.
   - `check_collision()`: Checks collision between the ball and paddles.
   - `move_ball()`: Moves the ball and handles collisions with walls and paddles.
   - `move_paddle_ai()`: Implements AI for player vs. computer mode to control one of the paddles.
   - `move_paddle()`: Moves the player-controlled paddle based on keyboard input.
   - `draw_game_over()`: Draws the game over screen with appropriate messages.
   - `draw_menu()`: Draws the main menu screen.
   - `draw_background()`: Draws the game background.
   - `draw_net()`: Draws the net in the middle of the game screen.
   - `draw_ball()` and `draw_paddle()`: Draw the ball and paddles on the screen.
   - `draw_player_0_score()` and `draw_player_1_score()`: Draw the scores of players on the screen.
5. **Main Function**: Controls the flow of the game, initializes necessary components, and contains the main game loop for user input, game state updates, and rendering.

## Audio Support
- The game includes background music for the title screen, gameplay, and end screen.
- It also features a bounce sound effect when the ball hits a paddle.

## Improvements
1. **Code Refactoring**: Refactor functions for better organization and readability.
2. **Error Handling**: Implement more robust error handling for file loading and resource allocation.
3. **Optimization**: Optimize collision detection algorithms and game logic for better performance.
4. **Enhanced AI**: Improve AI behavior for single-player mode to provide a more challenging experience.
5. **Graphics**: Enhance visual elements such as paddles, ball, and background for better aesthetics.

## Conclusion
The provided code successfully implements a basic Pong game using SDL library in C, offering simple yet engaging gameplay with support for single-player and multiplayer modes, scoring, audio, and a user-friendly interface. With further enhancements and refinements, it has the potential to become a polished and enjoyable gaming experience.

## Credits
Sound effects and background music sourced from [freesound.org](https://freesound.org).
