#pragma once

#include "Container.h"

class EscapeMenu {
public:
  static void init();
  static void update();
private:
  static Container* ui;
  static bool isPressed;
  static bool opened;
};
