
//Using graphics lib SDL and other important libs
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>

#define SCREEN_WIDTH 640	//window width
#define SCREEN_HEIGHT 480	//window height


//initialize SDL
int init(int w, int h, int argc, char *args[]);

typedef struct ball_s {

	int x, y; //coordinates in screen
	int w,h; // ball width and height
	int dx, dy; //speed with sign--> vector+
} ball_t;

typedef struct paddle {

	int x,y;
	int w,h;
} paddle_t;

// Program globals
static ball_t ball;
static paddle_t paddle[2];
int score[] = {0,0};
int width, height;		//used if fullscreen

SDL_Window* window = NULL;	//The window we'll be rendering to
SDL_Renderer *renderer;		//The renderer SDL will use to draw to the screen

Mix_Music *titleMusic = NULL; //audio files for title and in-game bgm
Mix_Music *gameplayMusic = NULL;
Mix_Music *endScreenMusic = NULL;

void init_background_music() {
    // Load title screen background music
    titleMusic = Mix_LoadMUS("title_bgm.mp3");
    if (titleMusic == NULL) {
        printf("Failed to load title screen background music! SDL_mixer Error: %s\n", Mix_GetError());
    }

    // Load gameplay background music
    gameplayMusic = Mix_LoadMUS("gameplay_bgm.mp3");
    if (gameplayMusic == NULL) {
        printf("Failed to load gameplay background music! SDL_mixer Error: %s\n", Mix_GetError());
    }
	 // Load end screen background music
    endScreenMusic = Mix_LoadMUS("end_screen_bgm.wav");
    if (endScreenMusic == NULL) {
        printf("Failed to load end screen background music! SDL_mixer Error: %s\n", Mix_GetError());
    }
}


//surfaces
static SDL_Surface *screen;
static SDL_Surface *title;
static SDL_Surface *numbermap;
static SDL_Surface *end[3];

//textures
SDL_Texture *screen_texture;

//inisilise starting position and sizes of game elemements
static void init_game() {
	
	ball.x = screen->w / 2;
	ball.y = screen->h / 2;
	ball.w = 10;
	ball.h = 10;
	ball.dy = 1;
	ball.dx = 1;
	
	paddle[0].x = 20;
	paddle[0].y = screen->h / 2 - 50 ;
	paddle[0].w = 10;
	paddle[0].h = 50;

	paddle[1].x = screen->w - 20 - 10;
	paddle[1].y = screen->h / 2 - 50;
	paddle[1].w = 10;
	paddle[1].h = 50;
}

int check_score() {
	
	int i;

	//loop through player scores
	for(i = 0; i < 2; i++) {
	
		//check if score is within the score win limit
		if (score[i] == 3 ) {
		
			//reset scores
			score[0] = 0;
			score[1] = 0;
			
			//return 1 if player 1/computer wins
			if (i == 0) {

				return 1;	

			//return 2 if player 2 wins
			} else {
				
				return 2;
			}
		}
	}
	
	//return 0 if no one has reached the wining score
	return 0;
}

//if return value is 1 collision occured. if return is 0, no collision.
int check_collision(ball_t a, paddle_t b) {

	int left_a, left_b;
	int right_a, right_b;
	int top_a, top_b;
	int bottom_a, bottom_b;

	left_a = a.x;
	right_a = a.x + a.w;
	top_a = a.y;
	bottom_a = a.y + a.h;

	left_b = b.x;
	right_b = b.x + b.w;
	top_b = b.y;
	bottom_b = b.y + b.h;
	

	if (left_a > right_b) {
		return 0;
	}

	if (right_a < left_b) {
		return 0;
	}

	if (top_a > bottom_b) {
		return 0;
	}

	if (bottom_a < top_b) {
		return 0;
	}

	return 1;
}

// This routine moves each ball by its motion vector. 
static void move_ball() {
	
	// Move the ball by its motion vector.
	ball.x += ball.dx;
	ball.y += ball.dy;
	
	 //Turn the ball around if it hits the edge of the screen. 
	if (ball.x < 0) {
		
		score[1] += 1;
		init_game();
	}

	if (ball.x > screen->w - 10) { 
		
		score[0] += 1;
		init_game();
	}

	if (ball.y < 0 || ball.y > screen->h - 10) {
		
		ball.dy = -ball.dy;
	}

	//check for collision with the paddle
	int i;

	for (i = 0; i < 2; i++) {
		
		int c = check_collision(ball, paddle[i]); 

		//collision detected	
		if (c == 1) {
			
			//ball moving left
			if (ball.dx < 0) {
					
				ball.dx -= 1;

			//ball moving right
			} else {
					
				ball.dx += 1;
			}
			
			//change ball direction
			ball.dx = -ball.dx;
			
			//change ball angle based on where on the paddle it hit
			int hit_pos = (paddle[i].y + paddle[i].h) - ball.y;

			if (hit_pos >= 0 && hit_pos < 7) {
				ball.dy = 4;
			}

			if (hit_pos >= 7 && hit_pos < 14) {
				ball.dy = 3;
			}
			
			if (hit_pos >= 14 && hit_pos < 21) {
				ball.dy = 2;
			}

			if (hit_pos >= 21 && hit_pos < 28) {
				ball.dy = 1;
			}

			if (hit_pos >= 28 && hit_pos < 32) {
				ball.dy = 0;
			}

			if (hit_pos >= 32 && hit_pos < 39) {
				ball.dy = -1;
			}

			if (hit_pos >= 39 && hit_pos < 46) {
				ball.dy = -2;
			}

			if (hit_pos >= 46 && hit_pos < 53) {
				ball.dy = -3;
			}

			if (hit_pos >= 53 && hit_pos <= 60) {
				ball.dy = -4;
			}

			//ball moving right
			if (ball.dx > 0) {

				//teleport ball to avoid mutli collision glitch
				if (ball.x < 30) {
				
					ball.x = 30;
				}
				
			//ball moving left
			} else {
				
				//teleport ball to avoid mutli collision glitch
				if (ball.x > 600) {
				
					ball.x = 600;
				}
			}
		}
	}
}

static void move_paddle_ai() {

	int center = paddle[0].y + 25;
	int screen_center = screen->h / 2 - 25;
	int ball_speed = ball.dy;

	if (ball_speed < 0) {
	
		ball_speed = -ball_speed;
	}

	//ball moving right
	if (ball.dx > 0) {
		
		//return to center position
		if (center < screen_center) {
			
			paddle[0].y += ball_speed;
		
		} else {
		
			paddle[0].y -= ball_speed;	
		}

	//ball moving left
	} else {
	
		//ball moving down
		if (ball.dy > 0) { 
			
			if (ball.y > center) { 
				
				paddle[0].y += ball_speed;

			} else { 
			
				paddle[0].y -= ball_speed;
			}
		}
		
		//ball moving up
		if (ball.dy < 0) {
		
			if (ball.y < center) { 
				
				paddle[0].y -= ball_speed;
			
			} else {
			
				paddle[0].y += ball_speed;
			}
		}

		//ball moving stright across
		if (ball.dy == 0) {
			
			if (ball.y < center) { 
			
				paddle[0].y -= 5;

			} else {
			
				paddle[0].y += 5;
			}
		}
	}
}

static void move_paddle(int d) {

	// if the down arrow is pressed move paddle down
	if (d == 0) {
		
		if(paddle[1].y >= screen->h - paddle[1].h) {
		
			paddle[1].y = screen->h - paddle[1].h;
		
		} else { 
		
			paddle[1].y += 5;
		}
	}
	
	// if the up arrow is pressed move paddle up
	if (d == 1) {

		if(paddle[1].y <= 0) {
		
			paddle[1].y = 0;

		} else {
		
			paddle[1].y -= 5;
		}
	}
}

static void move_paddle_p2(int d) {

	// if the down arrow is pressed move paddle down
	if (d == 0) {
		
		if(paddle[0].y >= screen->h - paddle[0].h) {
		
			paddle[0].y = screen->h - paddle[0].h;
		
		} else { 
		
			paddle[0].y += 5;
		}
	}
	
	// if the up arrow is pressed move paddle up
	if (d == 1) {

		if(paddle[0].y <= 0) {
		
			paddle[0].y = 0;

		} else {
		
			paddle[0].y -= 5;
		}
	}
}

static void draw_game_over(int p) { 

	SDL_Rect p1;
	SDL_Rect p2;
	SDL_Rect comp;
	SDL_Rect dest;

	p1.x = 0;
	p1.y = 0;
	p1.w = end[1]->w;
	p1.h = end[1]->h;

	p2.x = 0;
	p2.y = 0;
	p2.w = end[2]->w;
	p2.h = end[2]->h;
	
	comp.x = 0;
	comp.y = 0;
	comp.w = end[0]->w;
	comp.h = end[0]->h;

	dest.x = 0;
	dest.y = 0;
	dest.w = screen->w;
	dest.h = screen->h;

	if(p==1){
		SDL_BlitSurface(end[1], &p1, screen, &dest);
	}
	else if(p==2)
	{
		SDL_BlitSurface(end[2], &p2, screen, &dest);
	}
	else{
		SDL_BlitSurface(end[0], &comp, screen, &dest);
	}
	
}

static void draw_menu() {

	SDL_Rect src;
	SDL_Rect dest;

	src.x = 0;
	src.y = 0;
	src.w = title->w;
	src.h = title->h;

	dest.x = 0;
	dest.y = 0;
	dest.w = title->w;
	dest.h = title->h;

	SDL_BlitSurface(title, &src, screen, &dest);
}

static void draw_background() {
 
	SDL_Rect src;
	
	//draw bg with net
	src.x = 0;
	src.y = 0;
	src.w = screen->w;
	src.h = screen->h;

	
}

static void draw_net() {

	SDL_Rect net;
	
	net.x = screen->w / 2;
	net.y = 20;
	net.w = 5;
	net.h = 15;

	//draw the net
	int i,r;

	for(i = 0; i < 15; i++) {
		
		r = SDL_FillRect(screen, &net, 0xffffffff);
	
	

		net.y = net.y + 30;
	}
}

static void draw_ball() {
	
	SDL_Rect src;

	src.x = ball.x;
	src.y = ball.y;
	src.w = ball.w;
	src.h = ball.h;
	
	int r = SDL_FillRect(screen, &src, SDL_MapRGB(screen->format, 0xFF, 0x7F, 0x00));

	

}

static void draw_paddle() {

	SDL_Rect src;
	int i;

	for (i = 0; i < 2; i++) {
	
		src.x = paddle[i].x;
		src.y = paddle[i].y;
		src.w = paddle[i].w;
		src.h = paddle[i].h;
	
		int r = SDL_FillRect(screen, &src, 0xffffffff);
		
		
		

	}
}

static void draw_player_0_score() {
	
	SDL_Rect src;
	SDL_Rect dest;

	src.x = 0;
	src.y = 0;
	src.w = 64;
	src.h = 64;

	dest.x = (screen->w / 2) - src.w - 12; //12 is just padding spacing
	dest.y = 0;
	dest.w = 64;
	dest.h = 64;

	if (score[0] > 0 && score[0] < 3) {
		
		src.x += src.w * score[0];
	}
	
	SDL_BlitSurface(numbermap, &src, screen, &dest);
}

static void draw_player_1_score() {
	
	SDL_Rect src;
	SDL_Rect dest;

	src.x = 0;
	src.y = 0;
	src.w = 64;
	src.h = 64;

	dest.x = (screen->w / 2) + 12;
	dest.y = 0;
	dest.w = 64;
	dest.h = 64;
	
	if (score[1] > 0 && score[1] < 3) {
		
		src.x += src.w * score[1];
	}

	SDL_BlitSurface(numbermap, &src, screen, &dest);
}

int main (int argc, char *args[]) {
    int cond=0,sound1=0;
	int sound2=0;
	int sound3=0;
	int intersection=0;

    
    // SDL Window setup
    if (init(SCREEN_WIDTH, SCREEN_HEIGHT, argc, args) == 1) {
        return 0;
    }

    // Load sound effects
    Mix_Chunk *bounceSound = Mix_LoadWAV("bounce.wav");
    if (bounceSound == NULL) {
        printf("Failed to load bounce sound effect! SDL_mixer Error: %s\n", Mix_GetError());
        return 1;
    }
	
	init_background_music(); // generate background music


    // Set up rendering variables variables
    SDL_GetWindowSize(window, &width, &height);
    int sleep = 0;
    int quit = 0;
    int state = 0;
    int r = 0;
    Uint32 next_game_tick = SDL_GetTicks();

    // generate the ball position data
    init_game();
	SDL_Event event;


    // Render loop
    while (quit == 0) {
        // Check for new events every frame
        
		while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            }
        }

		SDL_PumpEvents();
        const Uint8 *keystate = SDL_GetKeyboardState(NULL);

        // Handle keyboard events
        if (keystate[SDL_SCANCODE_ESCAPE]) {
            state=0;
			 int cond=0,sound1=0;
			int sound2=0;
			int sound3=0;
			int intersection=0;
			

        }
        if (keystate[SDL_SCANCODE_DOWN]) {
            move_paddle(0);
        }
        if (keystate[SDL_SCANCODE_UP]) {
            move_paddle(1);
        }
        if (keystate[SDL_SCANCODE_W] && cond == 1) {
            move_paddle_p2(1);
        }
        if (keystate[SDL_SCANCODE_S] && cond == 1) {
            move_paddle_p2(0);
        }

        // Draw background
        SDL_RenderClear(renderer);
        SDL_FillRect(screen, NULL, 0x000000ff);

        // Display main menu
        if (state == 0) {
            intersection=0;
			
			if(sound1==0){
			Mix_PlayMusic(titleMusic, -1); //start title song
			sound1=sound1+1;
			}
			if (keystate[SDL_SCANCODE_1]) {
                cond = 2;
                 Mix_HaltMusic();
				state = 1;
            }
            if (keystate[SDL_SCANCODE_2]) {
                cond = 1;
                 Mix_HaltMusic();
				state = 1;
            }
            // Draw menu
            draw_menu();
        } 
        // Display game over
        else if (state == 2) {
            if (keystate[SDL_SCANCODE_SPACE]) {
                sound1=0;
				sound2=0;
				sound3=0;
				state = 0;
				intersection=1;
				 
                SDL_Delay(200); // Delay to prevent double trigger
            }
            if(sound3==0 && intersection==0){
			Mix_PlayMusic(endScreenMusic, -1);
			SDL_Delay(1000);
			Mix_HaltMusic();
			sound3++;
			}
			if (r == 1 && cond==2) {
                draw_game_over(3);
            } 
			else if(r==1 && cond==1){
				draw_game_over(1);
			}
			else if(r==2 && cond==2){
				draw_game_over(1);
			}
			else if(r==2 && cond==1){
				draw_game_over(2);
			}
        } 
        // Display the game
        else if (state == 1) {
            
             if(sound2==0){
			 Mix_PlayMusic(gameplayMusic, -1); // start gameplay music
			 sound2++;
			 }
			// Check score
			r = check_score();
            // If either player wins, change to game over state
            if (r == 1 || r == 2) {
                state = 2;
				continue;
				
				Mix_HaltMusic(); // Stop gameplay music
            }
            if (cond == 2) {
                // Paddle AI movement
                move_paddle_ai();
            }
            // Move the ball for the next frame
            move_ball();
            // Draw net, paddles, ball, and scores
            draw_net();
            draw_paddle();
            draw_ball();
            draw_player_0_score();
            draw_player_1_score();
        }

        // Update the screen
        SDL_UpdateTexture(screen_texture, NULL, screen->pixels, screen->w * sizeof(Uint32));
        SDL_RenderCopy(renderer, screen_texture, NULL, NULL);
        SDL_RenderPresent(renderer);

        // Play bounce sound when ball hits paddle
        if (check_collision(ball, paddle[0]) || check_collision(ball, paddle[1])) {
            Mix_PlayChannel(-1, bounceSound, 0);
        }

        // Calculate sleep time to maintain frame rate
        next_game_tick += 1000/ 60;
        sleep = next_game_tick - SDL_GetTicks();
        if (sleep >= 0) {
            SDL_Delay(sleep);
        }
    }

    // Free loaded images
    SDL_FreeSurface(screen);
    SDL_FreeSurface(title);
    SDL_FreeSurface(numbermap);
    SDL_FreeSurface(end[0]);
	SDL_FreeSurface(end[1]);
	SDL_FreeSurface(end[2]);

    // Free loaded sound effects
    Mix_FreeChunk(bounceSound);

    // Free renderer and all textures used with it
    SDL_DestroyRenderer(renderer);

    // Destroy window
    SDL_DestroyWindow(window);

    // Quit SDL subsystems
    Mix_Quit(); // Quit SDL_mixer,SDL_image subsystem
    SDL_Quit();
	IMG_Quit();

    return 0;
}



int init(int width, int height, int argc, char *args[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }
    
    // Initialize SDL_image
    int imgFlags = IMG_INIT_JPG | IMG_INIT_PNG; // Specify image formats to initialize
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return 1;
    }
    
    // Initialize SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        return 1;
    }

    int i;
    for (i = 0; i < argc; i++) {
        // Create window
        if(strcmp(args[i], "-f")) {
            SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN, &window, &renderer);
        } else {
            SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP, &window, &renderer);
        }
    }

    if (window == NULL) { 
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Create the screen surface where all the elements will be drawn onto (ball, paddles, net etc)
    screen = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_RGBA32);
    if (screen == NULL) {
        printf("Could not create the screen surface! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Create the screen texture to render the screen surface to the actual display
    screen_texture = SDL_CreateTextureFromSurface(renderer, screen);
    if (screen_texture == NULL) {
        printf("Could not create the screen texture! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Load the title image
    title = IMG_Load("TITLEF.png");
    if (title == NULL) {
        printf("Could not load title image! SDL_Error: %s\n", IMG_GetError());
        return 1;
    }
    
    // Load the numbermap image
    numbermap = IMG_Load("NUMBERSFINAL.png");
    if (numbermap == NULL) {
        printf("Could not load numbermap image! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }
    
    // Load the gameover image
    end[0] = IMG_Load("gameoverComp.png");
	end[1] = IMG_Load("gameoverP1.png");
	end[2] = IMG_Load("gameoverP2.png");


    if (end == NULL) {
        printf("Could not load gameover image! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    
     //Set the title colourkey
    Uint32 colorkey = SDL_MapRGB(title->format, 255, 0, 255);
    SDL_SetColorKey(title, SDL_TRUE, colorkey);
    SDL_SetColorKey(numbermap, SDL_TRUE, colorkey);

    return 0;
}

