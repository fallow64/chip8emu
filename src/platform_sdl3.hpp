#pragma once

#include "SDL3/SDL.h"
#include "platform.hpp"

class PlatformSDL3 : public IPlatform {
  public:
    PlatformSDL3(const char *title = "chip8emu", int width = 640, int height = 320,
                 int textureWidth = 64, int textureHeight = 32);
    ~PlatformSDL3();

    void Update(void const *buffer, int pitch);
    bool ProcessInput(uint8_t *keys);

  private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
};