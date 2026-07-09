#include "Bullet.h"
#include "Enemy.h"
#include "Window.h"
#include "Player.h"
#include "Health.h"

Bullet::Bullet(glm::vec2 position, glm::vec2 direction, int zIndex) : 
        direction(direction), damage(10.0f), muzzleVel(1000.0f), time(0.0f), damageTarget("enemies"),
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

    if (damageTarget == "enemies") {
      Enemy* enemy = dynamic_cast<Enemy*>(obj);

      if (!enemy) continue;

      enemy->takeDamage(damage);
    } else if (damageTarget == "player") {
      Player* player = dynamic_cast<Player*>(obj);

      if (!player) continue;

      Health::dealDmgToBodyPart("head", damage, true);
    } else {
      std::cout << "Invalid damage target !\n";
      return;
    }

    pendDelete();
    break;
  }
}
