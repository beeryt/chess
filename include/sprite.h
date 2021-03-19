#pragma once
#include "texture.h"
#include <SDL.h>

class Sprite {
  public:
    Sprite(Texture* texture = nullptr);
    virtual ~Sprite();

    Sprite(Sprite&&) noexcept;
    Sprite& operator=(Sprite&&) noexcept;

    void draw(SDL_Renderer*, SDL_Rect dst) const;

    void setRegion(SDL_Rect);
    SDL_Rect getRegion() const;

    void setFrame(unsigned);
    unsigned getFrame() const;

    void setHFrames(unsigned);
    unsigned getHFrames() const;

    void setVFrames(unsigned);
    unsigned getVFrames() const;

  private:
    Texture* texture;
    SDL_Rect region;
    unsigned frame = 0;
    unsigned hframes = 0;
    unsigned vframes = 0;
};

