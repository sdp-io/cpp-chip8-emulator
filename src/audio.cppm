module;
#include "SDL3/SDL_audio.h"
#include "SDL3/SDL_error.h"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_log.h"
#include <algorithm>
#include <array>
#include <memory>
export module Audio;

namespace chip8 {

struct AudioDestroy {
  void operator()(SDL_AudioStream *audio) const {
    SDL_DestroyAudioStream(audio);
  }
};

export class Audio {
public:
  Audio() { init_audio(); }

  bool play_buzzer(void);

private:
  static constexpr size_t Wave_Size{100};
  std::unique_ptr<SDL_AudioStream, AudioDestroy> audio{};
  std::array<int, Wave_Size> square_wave{};

  int init_audio(void);
};

int Audio::init_audio(void) {
  SDL_AudioStream *raw_audio{nullptr};

  if (!SDL_Init(SDL_INIT_AUDIO)) {
    SDL_LogError(SDL_LOG_CATEGORY_AUDIO, "Couldn't initialize SDL Audio: %s",
                 SDL_GetError());
    return 3;
  }

  raw_audio = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL,
                                        NULL, NULL);

  if (!raw_audio) {
    SDL_LogError(SDL_LOG_CATEGORY_AUDIO,
                 "Couldn't open SDL Audio Device Stream: %s", SDL_GetError());
    return 3;
  }

  if (!SDL_ResumeAudioStreamDevice(raw_audio)) {
    SDL_LogError(SDL_LOG_CATEGORY_AUDIO,
                 "Couldn't resume SDL Audio Stream Device: %s", SDL_GetError());

    return 3;
  }

  // Fill square wave with amplitude values
  std::fill(square_wave.begin(), square_wave.end() - 50, 3000);

  std::fill(square_wave.begin() + 50, square_wave.end(), -3000);

  audio.reset(raw_audio);

  return 0;
}

bool Audio::play_buzzer(void) {
  // TODO: Error handling
  SDL_PutAudioStreamData(audio.get(), square_wave.data(), Wave_Size);
  return true;
}

} // namespace chip8
