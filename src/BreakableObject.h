#pragma once

#include "Object.h"

class BreakableObject : public Object {
public:
  BreakableObject(glm::vec2 position, glm::vec2 size, float transparency, std::string texPath, int zIndex);
  BreakableObject(glm::vec2 position, glm::vec2 size, float transparency, glm::vec3 color, int zIndex);

  void takeDamage(float dmg);
private:
  float health = 100.0f;
};
