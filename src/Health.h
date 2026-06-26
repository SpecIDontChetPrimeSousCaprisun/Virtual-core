#pragma once

#include "UIElement.h"
#include "TextElement.h"
#include "Button.h"
#include "Container.h"

#include <vector>
#include <map>
#include <string>

class Health {
public:
  static void init();
  static void update();
  static void dealDmgToBodyPart(std::string bodyPart, float dmg);
private:
  static bool pressed;
  static bool playingOpenAnim;
  static float openAnimTime;
  static UIElement* background;
  static std::vector<TextElement*> cout;
  static std::vector<std::string> coutText;
  static int lastCoutText;
  static std::map<std::string, Button*> bodyParts;
  static std::map<std::string, float> bodyPartHealth;
  static Container* ui;
};
