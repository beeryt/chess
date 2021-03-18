#include "graphics.h"

Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : r(r), g(g), b(b), a(a) {}

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
    if (e.type == SDL_MOUSEMOTION) {
      if (motionCallback) motionCallback(e.motion.x, e.motion.y);
    }
    if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT) {
      if (clickCallback) clickCallback(e.button.x, e.button.y);
    }
  }
  return true;
}

void Graphics::swap() {
  SDL_RenderPresent(renderer);
}

void Graphics::clear() {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
  SDL_RenderClear(renderer);
}

void Graphics::setMotionCallback(MotionCallback cb) { motionCallback = cb; }
void Graphics::setClickCallback(ClickCallback cb) { clickCallback = cb; }

void Graphics::setLogical(int w, int h) {
  if (0 != SDL_RenderSetLogicalSize(renderer, w, h)) {
    fprintf(stderr, "Failed to set logical size: %s\n", SDL_GetError());
    return;
  }
}

void Graphics::fillRect(int x, int y, int w, int h, Color c) {
  SDL_Rect rect{ x, y, w, h };
  SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
  SDL_RenderFillRect(renderer, &rect);
}
