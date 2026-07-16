#pragma once

#include "Item.h"
#include "TextElement.h"
#include "Light.h"
#include "ScreenEffect.h"

#include <map>

class Gun : public Item {
public:
  Gun(glm::vec2 position, glm::vec2 size, float transparency, std::string texPath, int zIndex, std::string name);
  Gun(glm::vec2 position, glm::vec2 size, float transparency, glm::vec3 color, int zIndex, std::string name);

  static void update();

  float firerate;
  float barrelHeat;
  float lastParticle;
  int bullets;
  int maxBullets;
protected:
  void use() override;
  void itemUpdate() override;
private:
  static void initUi();

  static bool initedUi;
  static TextElement* ammoText;
  static std::map<Light*, float> lights;
  static ScreenEffect* effect;

  void init();
  
  float lastShot;
};
