#pragma once

#include "UIElement.h"
#include "TextElement.h"
#include "Container.h"

#include <vector>
#include <string>

class Health {
public:
  static void init();
  static void update();
private:
  static bool pressed;
  static bool playingOpenAnim;
  static float openAnimTime;
  static UIElement* background;
  static std::vector<TextElement*> cout;
  static std::vector<std::string> coutText;
  static int lastCoutText;
  static Container* ui;
};
