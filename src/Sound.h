#pragma once

#include "../include/miniaudio.h"
#include <iostream>

class Sound {
public:
  static void playSound(const char* path);
  static ma_sound* playLoopingSound(const char* path);
  static void init();
  static void unInit();
private:
  static ma_engine engine;
};
