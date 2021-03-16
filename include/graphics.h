#pragma once
#include <SDL2/SDL.h>

class Graphics {
  private:
    SDL_Window* window;
    SDL_Renderer* renderer;
  public:
    Graphics(const char* title = "");
    bool loop();
};

