#pragma once

#include "Item.h"

class MeleWeapon : public Item {
public:
  MeleWeapon(glm::vec2 position, glm::vec2 size, float transparency, std::string texPath, int zIndex, std::string name);
  MeleWeapon(glm::vec2 position, glm::vec2 size, float transparency, glm::vec3 color, int zIndex, std::string name);

  void use() override;

  float cooldown;
  float damage;
protected:
  void itemUpdate() override;
private:
  float hitTime;
};
