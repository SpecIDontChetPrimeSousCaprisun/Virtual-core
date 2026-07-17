#include "BreakableObject.h"

BreakableObject::BreakableObject(glm::vec2 position, glm::vec2 size, float transparency, std::string texPath, int zIndex) 
  : Object(position, size, transparency, texPath, zIndex) {}

BreakableObject::BreakableObject(glm::vec2 position, glm::vec2 size, float transparency, glm::vec3 color, int zIndex) 
  : Object(position, size, transparency, color, zIndex) {}

void BreakableObject::takeDamage(float dmg) {
  health -= dmg;

  if (health <= 0.0f) {
    pendDelete();
  }
}
