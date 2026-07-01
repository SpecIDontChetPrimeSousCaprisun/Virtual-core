#pragma once

#include <vector>
#include <string>

#include "Button.h"
#include "ScrollingElement.h"

class Tutorial {
public:
  static void init();
  static void open();
  static void close();
  static void update();
private:
  static std::vector<std::string> tutorials;
  static Button* currentText;
  static ScrollingElement* background;
  static float time;
  static float lastTime;
  static bool inOpenAnimation;
};
