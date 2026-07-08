#include "Bullet.h"
#include "Enemy.h"

Bullet::Bullet(glm::vec2 position, glm::vec2 direction, int zIndex) : direction(direction), damage(10.0f), muzzleVel(1000.0f),
        Object(position, glm::vec2(20.0f, 10.0f), 0.0f, glm::vec3(1.0f, 1.0f, 0.0f), zIndex) {
  collisionMask = CollisionGroup::Enemy;
  collisionGroup = CollisionGroup::Bullet;
  linearVelocity = glm::normalize(direction) * muzzleVel;
  anchored = false;
  canCollide = false;
}

void Bullet::beforeUpdate() {
  linearVelocity = glm::normalize(direction) * muzzleVel;
}

void Bullet::afterUpdate() {
  for (Object* obj : lastCollides) {
    if (obj->collisionGroup == CollisionGroup::Default) {
      pendDelete();
      break;
    }

    Enemy* enemy = dynamic_cast<Enemy*>(obj);

    if (!enemy) continue;

    pendDelete();
    break;
  }
}
