#pragma once

#include "UIElement.h"
#include "TextElement.h"

#include <vector>
#include <string>

class DeathScreen {
public:
  static void init();
  static void play();
  static void update();
private:
  static std::vector<TextElement*> cout;
  static std::vector<std::string> coutText;
  static UIElement* background;
  static bool isPlaying;
  static float time;
  static float lastCoutText;
};
