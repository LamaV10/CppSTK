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



// Vektor2D-Klasse
class Vec2 {
public:
    double x, y;
    Vec2(double x = 0, double y = 0) : x(x), y(y) {}
};


// Auto-Klasse
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
        vel_scale = 2;
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
    // Benutzer zur Eingabe der Fenstergröße aufforder
    std::cout << "Resolution: 2560x1440 (1) 1920x1080 (2) 1600x900 (3) 1280x720 (4): ";
    std::cout.flush();
    std::cin >> resolution;
    std::cout << resolution; 

    //Resolution options
    if (resolution == 1) {
      WIDTH = 2560;
      HEIGHT = 1440;
    }
    
    if (resolution == 2) {
      WIDTH = 1920;
      HEIGHT = 1080;
    }

    if (resolution == 3) {
      WIDTH = 1600;
      HEIGHT = 900;
    }
    
    if (resolution == 4) {
      WIDTH = 1280;
      HEIGHT = 720;
    }
    
    std::cout << WIDTH; 
    std::cout << HEIGHT; 

    // SDL2 und SDL_image initialisieren
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL konnte nicht initialisiert werden! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "SDL_image konnte nicht initialisiert werden! SDL_image Error: " << IMG_GetError() << std::endl;
        return -1;
    }
    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf konnte nicht initialisiert werden! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return -1;
    }

    // Fenster erstellen
    SDL_Window* window = SDL_CreateWindow("SuperTuxKart", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Fenster konnte nicht erstellt werden! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Renderer erstellen
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer konnte nicht erstellt werden! SDL Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Bilder laden
    SDL_Surface* track_surface = IMG_Load("imgs/rennstrecke.jpg");
    SDL_Texture* track_texture = SDL_CreateTextureFromSurface(renderer, track_surface);
    SDL_FreeSurface(track_surface);

    //Car No.1
    SDL_Surface* car1_surface = IMG_Load("imgs/tuxi.xcf");
    SDL_Texture* car1_texture = SDL_CreateTextureFromSurface(renderer, car1_surface);
    SDL_FreeSurface(car1_surface);

    //Car No.2
    SDL_Surface* car2_surface = IMG_Load("imgs/yoshi.xcf");
    SDL_Texture* car2_texture = SDL_CreateTextureFromSurface(renderer, car2_surface);
    SDL_FreeSurface(car2_surface);

    Car car1(car1_texture, Vec2(580.0, 785.0), 3.0, 4.0);
    Car car2(car2_texture, Vec2(680.0, 885.0), 3.0, 4.0);

    // Auto erstellen
    //WQHD 2560x1440
    if (resolution == 1){
      car1.pos.x = 820.0;
      car2.pos.x = 810.0;
      car1.pos.y = 1085.0; 
      car2.pos.y = 1335.0; 
    }

    //FHD 192x1080
    if (resolution == 2){
      car1.pos.x = 580.0;
      car2.pos.x = 605.0;
      car1.pos.y = 785.0; 
      car2.pos.y = 995.0; 
    }

    //1600x900
    if (resolution == 3){
      car1.pos.x = 455.0;
      car2.pos.x = 495.0;
      car1.pos.y = 635.0;
      car2.pos.y = 825.0;
    }
    
    //HD 1280x720
    if (resolution == 4){
      car1.pos.x = 345.0;
      car2.pos.x = 410.0;
      car1.pos.y = 480.0;
      car2.pos.y = 655.0;
    }



// Hauptloop
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

      // Ereignisse verarbeiten
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

      //player1
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


      //player 2
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
      // Bildschirm löschen
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
      SDL_RenderClear(renderer);
        
      // Strecke und Auto zeichnen
      SDL_RenderCopy(renderer, track_texture, nullptr, nullptr);
      car1.draw(renderer);
      car2.draw(renderer);
      // Bildschirm aktualisieren
      SDL_RenderPresent(renderer);
  }

    // Ressourcen freigeben
    SDL_DestroyTexture(car1_texture);
    SDL_DestroyTexture(car2_texture);
    
    SDL_DestroyTexture(track_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    // SDL und zugehörige Bibliotheken beenden
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();

    return 0;
}
