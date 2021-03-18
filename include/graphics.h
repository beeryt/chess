#pragma once
#include <SDL2/SDL.h>
#include <functional>

struct Color {
  uint8_t r, g, b, a;
  Color (uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xFF);
};

class Graphics {
  public:
    Graphics(const char* title = "");

    using MotionCallback = std::function<void(int,int)>;
    void setMotionCallback(MotionCallback cb);
    MotionCallback getMotionCallback() const;

    using ClickCallback = std::function<void(int,int)>;
    void setClickCallback(ClickCallback cb);
    ClickCallback getClickCallback() const;

    void setLogical(int w, int h);
    void getLogical(int*w, int*h);

    bool loop();
    void clear();
    void swap();

    void drawRect(int x, int y, int w, int h, Color c);
    void fillRect(int x, int y, int w, int h, Color c);

  private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    MotionCallback motionCallback;
    ClickCallback clickCallback;
};

