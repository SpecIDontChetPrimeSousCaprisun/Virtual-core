#include "Bullet.h"
#include "Enemy.h"
#include "Window.h"
#include "Player.h"

Bullet::Bullet(glm::vec2 position, glm::vec2 direction, int zIndex) : 
        direction(direction), damage(10.0f), muzzleVel(1000.0f), time(0.0f),
        Object(position, glm::vec2(20.0f, 10.0f), 0.0f, glm::vec3(1.0f, 1.0f, 0.0f), zIndex) {
  collisionMask = CollisionGroup::Enemy;
  collisionGroup = CollisionGroup::Bullet;
  linearVelocity = glm::normalize(direction) * muzzleVel;
  rotation = glm::degrees(std::atan2(direction.y, direction.x));
  anchored = false;
  canCollide = true;
}

void Bullet::beforeUpdate() {
  time += Window::deltaTime;
  linearVelocity = glm::normalize(direction) * muzzleVel;
  linearVelocity += glm::vec2(0.0f, gravity * time);
  rotation = glm::degrees(std::atan2(linearVelocity.y, linearVelocity.x));
}

void Bullet::afterUpdate() {
  for (Object* obj : lastCollides) {
    if (!obj->canCollide) continue;
    if (obj->collisionGroup == CollisionGroup::Default) {
      pendDelete();
      break;
    }

    Enemy* enemy = dynamic_cast<Enemy*>(obj);

    if (!enemy) continue;

    enemy->takeDamage(damage);
    pendDelete();
    break;
  }
}
