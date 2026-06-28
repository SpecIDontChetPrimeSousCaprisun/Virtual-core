#pragma once

#include <vector>
#include <string>

#include "TextElement.h"
#include "ScrollingElement.h"

class Tutorial {
public:
  static void init();
  static void open();
  static void update();
private:
  static std::vector<std::string> tutorials;
  static std::vector<TextElement*> texts;
  static TextElement* currentText;
  static ScrollingElement* background;
  static float time;
  static float lastTime;
  static bool inOpenAnimation;
};
