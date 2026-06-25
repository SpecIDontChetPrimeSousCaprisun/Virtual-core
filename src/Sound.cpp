#include "Sound.h"

ma_engine Sound::engine;

void Sound::init() {
  if (ma_engine_init(NULL, &engine) != MA_SUCCESS) {
    std::cout << "Failed to initialize audio\n";
    return;
  }
}

void Sound::unInit() {
  ma_engine_uninit(&engine);
}

void Sound::playSound(const char* path) {
  ma_result result =
  ma_engine_play_sound(&engine, path, NULL);

  if (result != MA_SUCCESS) {
      std::cout << "Error: " << result << '\n';
  }
}

ma_sound* Sound::playLoopingSound(const char* path) {
  auto* sound = new ma_sound;

  if (ma_sound_init_from_file(
          &engine,
          path,
          0,
          nullptr,
          nullptr,
          sound) != MA_SUCCESS) {
      delete sound;
      return nullptr;
  }

  ma_sound_set_looping(sound, MA_TRUE);
  ma_sound_start(sound);

  return sound;
}
