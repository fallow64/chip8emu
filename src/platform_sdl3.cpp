#include "platform_sdl3.hpp"
#include "SDL3/SDL.h"

PlatformSDL3::PlatformSDL3(const char *title, int width, int height, int textureWidth,
                           int textureHeight) {
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(title, width, height, 0);
    renderer = SDL_CreateRenderer(window, nullptr);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING,
                                textureWidth, textureHeight);
    
    // Set nearest-neighbor scaling for pixel-perfect rendering
    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
}

PlatformSDL3::~PlatformSDL3() {
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void PlatformSDL3::Update(void const *buffer, int pitch) {
    SDL_UpdateTexture(texture, nullptr, buffer, pitch);
    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

bool PlatformSDL3::ProcessInput(uint8_t *keys) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            return true;
        } else if (event.type == SDL_EVENT_KEY_DOWN || event.type == SDL_EVENT_KEY_UP) {
            bool isDown = (event.type == SDL_EVENT_KEY_DOWN);
            switch (event.key.scancode) {
            case SDL_SCANCODE_1:
                keys[0x1] = isDown;
                break;
            case SDL_SCANCODE_2:
                keys[0x2] = isDown;
                break;
            case SDL_SCANCODE_3:
                keys[0x3] = isDown;
                break;
            case SDL_SCANCODE_4:
                keys[0xC] = isDown;
                break;

            case SDL_SCANCODE_Q:
                keys[0x4] = isDown;
                break;
            case SDL_SCANCODE_W:
                keys[0x5] = isDown;
                break;
            case SDL_SCANCODE_E:
                keys[0x6] = isDown;
                break;
            case SDL_SCANCODE_R:
                keys[0xD] = isDown;
                break;

            case SDL_SCANCODE_A:
                keys[0x7] = isDown;
                break;
            case SDL_SCANCODE_S:
                keys[0x8] = isDown;
                break;
            case SDL_SCANCODE_D:
                keys[0x9] = isDown;
                break;
            case SDL_SCANCODE_F:
                keys[0xE] = isDown;
                break;

            case SDL_SCANCODE_Z:
                keys[0xA] = isDown;
                break;
            case SDL_SCANCODE_X:
                keys[0x0] = isDown;
                break;
            case SDL_SCANCODE_C:
                keys[0xB] = isDown;
                break;
            case SDL_SCANCODE_V:
                keys[0xF] = isDown;
                break;

            case SDL_SCANCODE_ESCAPE:
                return true;

            default:
                break;
            }
        }
    }

    return false;
}