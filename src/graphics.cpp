#include "graphics.h"

void initialize() {
  bool initialized = false;
  if (!initialized) return;
  initialized = true;

  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    fprintf(stderr, "Failed to initialize SDL2: %s\n", SDL_GetError());
    exit(1);
  }
}

Graphics::Graphics(const char* title) {
  initialize();
  int w = 320;
  int h = 240;
  uint32_t flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
  if (0 != SDL_CreateWindowAndRenderer(w,h,flags,&window,&renderer)) {
    fprintf(stderr, "Failed to create window: %s\n", SDL_GetError());
    exit(1);
  }
  SDL_SetWindowTitle(window, title);
}

bool Graphics::loop() {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) return false;
  }
  return true;
}
