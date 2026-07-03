module;

#include "SDL3/SDL_error.h"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_log.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"
#include <SDL3/SDL.h>
#include <memory>

export module Display;

namespace chip8 {

struct WindowDestroy {
  void operator()(SDL_Window *window) const { SDL_DestroyWindow(window); }
};

struct RendererDestroy {
  void operator()(SDL_Renderer *renderer) const {
    SDL_DestroyRenderer(renderer);
  }
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

  // TEST: Render a single point to window for testing purposes
  int draw_dot();

private:
  std::unique_ptr<SDL_Window, WindowDestroy> window{};
  std::unique_ptr<SDL_Renderer, RendererDestroy> renderer{};
};

int Display::init_display() {
  SDL_Window *raw_window = nullptr;
  SDL_Renderer *raw_renderer = nullptr;

  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s",
                 SDL_GetError());
    return 3;
  }

  if (!SDL_CreateWindowAndRenderer("CHIP-8", 64, 32, SDL_WINDOW_RESIZABLE,
                                   &raw_window, &raw_renderer)) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s",
                 SDL_GetError());
    return 3;
  }

  window.reset(raw_window);
  renderer.reset(raw_renderer);

  clear_display();

  return 0;
}

int Display::clear_display() {
  // TODO: Actually handle SDL errors

  // Set renderer to color black
  SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 255);
  SDL_RenderClear(renderer.get());

  if (!SDL_RenderPresent(renderer.get())) {
    SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Failed to present render: %s",
                 SDL_GetError());
    return 3;
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
