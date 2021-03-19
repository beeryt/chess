#pragma once
#include <SDL.h>
#include <string>

class Texture {
  public:
    Texture(SDL_Texture* texture = nullptr);
    Texture(const std::string& filename, SDL_Renderer* renderer);

    virtual ~Texture();

    // move constructor/assignment
    Texture(Texture&&) noexcept;
    Texture& operator=(Texture&&) noexcept;

    // copy constructor/assignment
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    SDL_Rect getRegion() const;
    int width() const;
    int height() const;

    SDL_Texture* get();

  private:
    SDL_Texture* texture;
};

