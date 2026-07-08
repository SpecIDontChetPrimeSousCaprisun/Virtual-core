#pragma once

#include "Item.h"

class Gun : public Item {
public:
  Gun(glm::vec2 position, glm::vec2 size, float transparency, std::string texPath, int zIndex, std::string name);
  Gun(glm::vec2 position, glm::vec2 size, float transparency, glm::vec3 color, int zIndex, std::string name);

  float firerate;
protected:
  void use() override;
  void itemUpdate() override;
private:
  void init();
  
  float lastShot;
};
