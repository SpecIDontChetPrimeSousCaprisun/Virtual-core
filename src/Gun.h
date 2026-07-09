#pragma once

#include "Item.h"
#include "TextElement.h"

class Gun : public Item {
public:
  Gun(glm::vec2 position, glm::vec2 size, float transparency, std::string texPath, int zIndex, std::string name);
  Gun(glm::vec2 position, glm::vec2 size, float transparency, glm::vec3 color, int zIndex, std::string name);

  float firerate;
  int bullets;
  int maxBullets;
protected:
  void use() override;
  void itemUpdate() override;
private:
  static void initUi();

  static bool initedUi;
  static TextElement* ammoText;

  void init();
  
  float lastShot;
};
