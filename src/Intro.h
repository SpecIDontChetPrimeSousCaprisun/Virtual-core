#pragma once

#include <vector>
#include <string>

#include "UIElement.h"
#include "TextElement.h"
#include "Container.h"

class Intro {
public:
  static void init();
  static void update();
private:
  static std::vector<std::string> coutText;
  static std::vector<TextElement*> cout;
  static float time;
  static int lastCoutText;
  static UIElement* background;
  static Container* ui;
  static bool finishedIntro;
};
