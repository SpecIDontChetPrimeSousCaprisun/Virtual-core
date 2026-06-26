#pragma once

#include "UIElement.h"

class Health {
public:
  static void init();
  static void update();
private:
  static bool pressed;
  static bool playingOpenAnim;
  static float openAnimTime;
  static UIElement* background;
};
