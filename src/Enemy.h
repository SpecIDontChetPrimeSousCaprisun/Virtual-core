#pragma once

#include "Object.h"

class Enemy : public Object {
public:
   Enemy(glm::vec2 position, glm::vec2 size, std::string texPath, int zIndex);

   float damage, speed, cooldown;
protected:
   void beforeUpdate() override;
private:
   float lastAttack;
};
