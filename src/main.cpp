#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <cmath>
#include <chrono>
#include <thread>
#include <print>
#include <cstdio>

int WIDTH;
int HEIGHT;
int resolution = 0;

const double PI = 3.14159265358979323846;

// Vektor2D-class
class Vec2 {
public:
    double x, y;
    Vec2(double x = 0, double y = 0) : x(x), y(y) {}
};


// car-class
class Car {
public:
    SDL_Texture* texture;
    Vec2 pos;
    double vel;
    double angle;
    double max_vel;
    double rotation_vel;
    double acceleration;
    float vel_scale;
 
    Car(SDL_Texture* texture, Vec2 pos, double max_vel, double rotation_vel)
        : texture(texture), pos(pos), vel(0), angle(0), max_vel(max_vel), rotation_vel(rotation_vel), acceleration(0.1) {}

    void rotate(bool left, bool right) {
        if (left) angle += rotation_vel;
        if (right) angle -= rotation_vel;
    }

    void move_backward() {
        vel = std::min(vel + acceleration, max_vel / 2.0);
        update_position();
    }

    void move_forward() {
	switch (resolution) {
	    case 1: vel_scale = 3; break;
	    case 2: vel_scale = 2.5; break;
	    case 3: vel_scale = 2; break;
	    case 4: vel_scale = 1.5; break;
			
	}

        vel = std::max(vel - acceleration * vel_scale, -max_vel * vel_scale);
        update_position();
    }

    void update_position() {
        double radians = angle * PI / -180.0;
        pos.x += vel * std::cos(radians);
        pos.y += vel * std::sin(radians);
    }

    void draw(SDL_Renderer* renderer) {
        int w, h;
        SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
        SDL_Rect dst = { static_cast<int>(pos.x), static_cast<int>(pos.y), w, h };
        SDL_RenderCopyEx(renderer, texture, nullptr, &dst, -angle, nullptr, SDL_FLIP_NONE);
    }
};



int main(int argc, char* argv[]) {
    // user input for the resolution
    std::cout << "Resolution: \n 1: 2560x1440 \n 2: 1920x1080 \n 3: 1600x900 "
                 "\n 4: 1280x720 \n => ";
    std::cout.flush();
    std::cin >> resolution;
    std::cout << resolution; 

    //Resolution options
    switch (resolution) {
	case 1:
	  WIDTH = 2560;
	  HEIGHT = 1440;
	  break;
	case 2:
	  WIDTH = 1920;
	  HEIGHT = 1080;
	  break;
	case 3:
	  WIDTH = 1600;
	  HEIGHT = 900;
	  break;
	case 4: 
	  WIDTH = 1280;
	  HEIGHT = 720;
	  break;
    }
    
    std::cout << WIDTH; 
    std::cout << HEIGHT; 

    // create window 
    SDL_Window* window = SDL_CreateWindow("SuperTuxKart", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);

    // create render
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // load pictures 
    SDL_Surface* track_surface = IMG_Load("imgs/rennstrecke.jpg");
    SDL_Texture* track_texture = SDL_CreateTextureFromSurface(renderer, track_surface);
    SDL_FreeSurface(track_surface);

    // car No.1
    SDL_Surface* car1_surface = IMG_Load("imgs/tuxi.xcf");
    SDL_Texture* car1_texture = SDL_CreateTextureFromSurface(renderer, car1_surface);
    SDL_FreeSurface(car1_surface);

    // car No.2
    SDL_Surface* car2_surface = IMG_Load("imgs/yoshi.xcf");
    SDL_Texture* car2_texture = SDL_CreateTextureFromSurface(renderer, car2_surface);
    SDL_FreeSurface(car2_surface);

    Car car1(car1_texture, Vec2(580.0, 785.0), 3.0, 4.0);
    Car car2(car2_texture, Vec2(680.0, 885.0), 3.0, 4.0);

    // create car pos for different resolutions
    switch (resolution) {
	case 1: 
	    //WQHD 2560x1440
	    car1.pos.x = 820.0;
	    car2.pos.x = 810.0;
	    car1.pos.y = 1085.0; 
	    car2.pos.y = 1335.0; 
	    break;
	case 2:
	    //FHD 1920x1080
	    car1.pos.x = 580.0;
	    car2.pos.x = 605.0;
	    car1.pos.y = 785.0; 
	    car2.pos.y = 995.0; 
	    break;
	case 3:
	    //1600x900
	    car1.pos.x = 455.0;
	    car2.pos.x = 495.0;
	    car1.pos.y = 635.0;
	    car2.pos.y = 825.0;
	    break;
	case 4:
	    //HD 1280x720
	    car1.pos.x = 345.0;
	    car2.pos.x = 410.0;
	    car1.pos.y = 480.0;
	    car2.pos.y = 655.0;
	    break;
    }


// main loop
bool quit = false;
SDL_Event e;
auto last_update = std::chrono::high_resolution_clock::now();
std::chrono::duration<double> frame_duration(1.0 / 60.0);

  while (!quit) {
      auto now = std::chrono::high_resolution_clock::now();
      std::chrono::duration<double> delta_time = now - last_update;
      if (delta_time < frame_duration) {
          std::this_thread::sleep_for(frame_duration - delta_time);
          continue;
      }
      last_update = now;

      // handle actions 
      while (SDL_PollEvent(&e) != 0) {
          if (e.type == SDL_QUIT) {
              quit = true;
          } else if (e.type == SDL_KEYDOWN) {
               if (e.key.keysym.sym == SDLK_ESCAPE) {
                   quit = true;
              }
          }
      }

      const Uint8* currentKeyStates = SDL_GetKeyboardState(nullptr);
      bool moving1 = false;
      bool moving2 = false;

      // player1
      if (currentKeyStates[SDL_SCANCODE_A]) {
           car1.rotate(true, false);
      }
      if (currentKeyStates[SDL_SCANCODE_D]) {
            car1.rotate(false, true);
      }
      if (currentKeyStates[SDL_SCANCODE_W]) {
            car1.move_forward();
            moving1 = true;
      }
      if (currentKeyStates[SDL_SCANCODE_S]) {
            car1.move_backward();
            moving1 = true;
      }

      if (!moving1) {
            car1.vel *= 0.9;
            car1.update_position();
      }


      // player 2
      if (currentKeyStates[SDL_SCANCODE_J]) {
           car2.rotate(true, false);
      }
      if (currentKeyStates[SDL_SCANCODE_L]) {
            car2.rotate(false, true);
      }
      if (currentKeyStates[SDL_SCANCODE_I]) {
            car2.move_forward();
            moving2 = true;
      }
      if (currentKeyStates[SDL_SCANCODE_K]) {
            car2.move_backward();
            moving2 = true;
      }

      if (!moving2) {
            car2.vel *= 0.9;
            car2.update_position();
      }

      // clear screen 
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
      SDL_RenderClear(renderer);
        
      // draw track and car 
      SDL_RenderCopy(renderer, track_texture, nullptr, nullptr);
      car1.draw(renderer);
      car2.draw(renderer);
      // refresh screen 
      SDL_RenderPresent(renderer);
  }

    // free resoureces 
    SDL_DestroyTexture(car1_texture);
    SDL_DestroyTexture(car2_texture);
    
    SDL_DestroyTexture(track_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    // quit libraries
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();

    return 0;
}
