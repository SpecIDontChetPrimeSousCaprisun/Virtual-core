#pragma once

#include "Object.h"

#include <string>

class Bullet : public Object {
public:
  Bullet(glm::vec2 position, glm::vec2 direction, int zIndex);

  glm::vec2 direction;
  std::string damageTarget;
  float damage;
  float muzzleVel;
  float time;
protected:
  void beforeUpdate() override;
  void afterUpdate() override;
};
