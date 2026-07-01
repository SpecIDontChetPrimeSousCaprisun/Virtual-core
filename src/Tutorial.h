#pragma once

#include <vector>
#include <string>

#include "Button.h"
#include "ScrollingElement.h"
#include "Player.h"

class Tutorial {
public:
  static void init();
  static void open();
  static void close();
  static void remove();
  static void update();
private:
  static std::vector<Object*> currentTutorial;
  static std::vector<std::string> tutorials;
  static Player* oldPlayer;
  static Button* currentText;
  static ScrollingElement* background;
  static float time;
  static float lastTime;
  static bool inOpenAnimation;
};
