module;

#include "SDL3/SDL_error.h"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_log.h"
#include "SDL3/SDL_pixels.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_scancode.h"
#include "SDL3/SDL_video.h"
#include <SDL3/SDL.h>
#include <array>
#include <cstddef>
#include <cstdint>
#include <format>
#include <iostream>
#include <memory>

export module Display;

namespace chip8 {

export constexpr size_t Keypad_Size{16};

struct WindowDestroy {
  void operator()(SDL_Window *window) const { SDL_DestroyWindow(window); }
};

struct RendererDestroy {
  void operator()(SDL_Renderer *renderer) const {
    SDL_DestroyRenderer(renderer);
  }
};

struct TextureDestroy {
  void operator()(SDL_Texture *texture) const { SDL_DestroyTexture(texture); }
};

export class Display {

public:
  Display() { init_display(); }

  /**
   * @brief Initializes a window and renderer.
   *
   * Creates a 64x32 resolution window (as per CHIP-8 specifications,) then uses
   * the renderer to clear the window, setting the contents to a black screen.
   * Places the allocated SDL_Window and SDL_Renderer objects into the class's
   * smart pointers.
   *
   * @return Zero on success, non-zero on failure.
   */
  int init_display();

  int clear_display();

  /**
   * @brief Polls for SDL_Events and handles them accordingly.
   *
   * @return True if SDL_EVENT_QUIT is not triggered, else false.
   */
  bool poll_events(std::array<bool, 16> key_pad);

  bool draw_line(const uint8_t &sprite_data, int x_coord, int y_coord);

  void convert_pixels_rgba8888();

  // TEST: Render a single point to window for testing purposes
  int draw_dot();

private:
  static constexpr size_t Height{32};
  static constexpr size_t Width{64};
  std::unique_ptr<SDL_Window, WindowDestroy> window{};
  std::unique_ptr<SDL_Renderer, RendererDestroy> renderer{};
  std::unique_ptr<SDL_Texture, TextureDestroy> texture{};
  std::array<uint8_t, Width * Height> pixels{};
  std::array<uint32_t, Width * Height> translated_pixels{}; // RGBA8888
};

int Display::init_display() {
  SDL_Window *raw_window{nullptr};
  SDL_Renderer *raw_renderer{nullptr};
  SDL_Texture *raw_texture{nullptr};

  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s",
                 SDL_GetError());
    return 3;
  }

  if (!SDL_CreateWindowAndRenderer("CHIP-8", Width, Height,
                                   SDL_WINDOW_RESIZABLE, &raw_window,
                                   &raw_renderer)) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s",
                 SDL_GetError());
    return 3;
  }

  raw_texture = SDL_CreateTexture(raw_renderer, SDL_PIXELFORMAT_RGBA8888,
                                  SDL_TEXTUREACCESS_STREAMING, Width, Height);

  window.reset(raw_window);
  renderer.reset(raw_renderer);
  texture.reset(raw_texture);

  clear_display();

  return 0;
}

bool Display::poll_events(std::array<bool, Keypad_Size> key_pad) {
  SDL_Event event{};
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_EVENT_QUIT) {
      return false;
    }

    if (event.type == SDL_EVENT_KEY_DOWN) {
      std::cout << std::format("{}", event.key.key) << " pressed!\n";
      switch (event.key.scancode) {
      case SDL_SCANCODE_1:
        key_pad[0x0] = true;
        break;
      case SDL_SCANCODE_2:
        key_pad[0x1] = true;
        break;
      case SDL_SCANCODE_3:
        key_pad[0x2] = true;
        break;
      case SDL_SCANCODE_4:
        key_pad[0x3] = true;
        break;

      case SDL_SCANCODE_Q:
        key_pad[0x4] = true;
        break;
      case SDL_SCANCODE_W:
        key_pad[0x5] = true;
        break;
      case SDL_SCANCODE_E:
        key_pad[0x6] = true;
        break;
      case SDL_SCANCODE_R:
        key_pad[0x7] = true;
        break;

      case SDL_SCANCODE_A:
        key_pad[0x8] = true;
        break;
      case SDL_SCANCODE_S:
        key_pad[0x9] = true;
        break;
      case SDL_SCANCODE_D:
        key_pad[0xA] = true;
        break;
      case SDL_SCANCODE_F:
        key_pad[0xB] = true;
        break;

      case SDL_SCANCODE_Z:
        key_pad[0xC] = true;
        break;
      case SDL_SCANCODE_X:
        key_pad[0xD] = true;
        break;
      case SDL_SCANCODE_C:
        key_pad[0xE] = true;
        break;
      case SDL_SCANCODE_V:
        key_pad[0xF] = true;
        break;

      default:
        // TODO: Remove
        std::cout << "Invalid key!\n";
        break;
      }
    }

    if (event.type == SDL_EVENT_KEY_UP) {
      switch (event.key.scancode) {
      case SDL_SCANCODE_1:
        key_pad[0x0] = false;
        break;
      case SDL_SCANCODE_2:
        key_pad[0x1] = false;
        break;
      case SDL_SCANCODE_3:
        key_pad[0x2] = false;
        break;
      case SDL_SCANCODE_4:
        key_pad[0x3] = false;
        break;

      case SDL_SCANCODE_Q:
        key_pad[0x4] = false;
        break;
      case SDL_SCANCODE_W:
        key_pad[0x5] = false;
        break;
      case SDL_SCANCODE_E:
        key_pad[0x6] = false;
        break;
      case SDL_SCANCODE_R:
        key_pad[0x7] = false;
        break;

      case SDL_SCANCODE_A:
        key_pad[0x8] = false;
        break;
      case SDL_SCANCODE_S:
        key_pad[0x9] = false;
        break;
      case SDL_SCANCODE_D:
        key_pad[0xA] = false;
        break;
      case SDL_SCANCODE_F:
        key_pad[0xB] = false;
        break;

      case SDL_SCANCODE_Z:
        key_pad[0xC] = false;
        break;
      case SDL_SCANCODE_X:
        key_pad[0xD] = false;
        break;
      case SDL_SCANCODE_C:
        key_pad[0xE] = false;
        break;
      case SDL_SCANCODE_V:
        key_pad[0xF] = false;
        break;

      default:
        // TODO: Remove
        std::cout << "Invalid key!\n";
        break;
      }
    }
  }

  return true;
}

bool Display::draw_line(const uint8_t &sprite_data, int x_coord, int y_coord) {
  bool has_collision{false};

  for (uint8_t i{0}; i < 8; i++) {
    uint8_t bit{sprite_data};
    uint8_t screen_pixel{pixels[(y_coord * Width) + x_coord]};

    // Extract i_th bit starting from the left
    bit = (bit >> (7 - i)) & 1;

    if (screen_pixel == 1 && bit == 1) {
      has_collision = true;
    }

    screen_pixel ^= bit;
    pixels[(y_coord * Width) + x_coord] = screen_pixel;

    x_coord++;
  }

  convert_pixels_rgba8888();

  int pitch{Width * 4}; // Screen width * bytes in uint32_t
  SDL_UpdateTexture(texture.get(), nullptr, translated_pixels.data(), pitch);
  SDL_RenderTexture(renderer.get(), texture.get(), nullptr, nullptr);
  SDL_RenderPresent(renderer.get());

  return has_collision;
}

void Display::convert_pixels_rgba8888() {
  uint32_t white{0xFFFFFFFF};
  uint32_t black{0x000000FF};

  for (size_t i{0}; i < pixels.size(); i++) {
    if (pixels[i] == 1) {
      translated_pixels[i] = white;
    } else {
      translated_pixels[i] = black;
    }
  }
}

int Display::clear_display() {
  // TODO: Actually handle SDL errors

  // Set renderer to color black
  SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer.get());

  if (!SDL_RenderPresent(renderer.get())) {
    SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Failed to present render: %s",
                 SDL_GetError());
    return 3;
  }

  for (uint8_t &pixel : pixels) {
    pixel = 0;
  }

  return 0;
}

// TEST: Render a single point to window for testing purposes
int Display::draw_dot() {
  SDL_SetRenderDrawColor(renderer.get(), 255, 255, 255, 255);
  SDL_RenderPoint(renderer.get(), 32 / 2.0, 16 / 2.0);
  SDL_RenderPresent(renderer.get());

  return 0;
}

} // namespace chip8
