#include "Enemy.h"
#include "Player.h"
#include "Window.h"
#include "Gun.h"
#include "Particle.h"

#include <cmath>

Enemy::Enemy(glm::vec2 position, glm::vec2 size, std::string texPath, int zIndex) 
  : Object(position, size, 0.0f, texPath, zIndex), damage(10.0f), speed(100.0f), cooldown(1.0f), lastAttack(0.0f), item(nullptr) {
  canCollide = true;
  anchored = false;
  maxHealth = 100;
  health = maxHealth;
  collisionGroup = CollisionGroup::Enemy;
  /*item = new Gun(position, glm::vec2(50, 25), 0.0f, "textures/Gun.png", zIndex, "Enemy weapon");
  item->specialOwner = this;
  item->registerObject();*/
}

void Enemy::beforeUpdate() {
  if (!Player::currentPlayer) return;

  if (shouldMoveToPlayer()) {
    if (Player::currentPlayer->position.x - position.x < 1) linearVelocity.x = -speed;
    else linearVelocity.x = speed;
  } else {
    linearVelocity.x = 0;
    if (item != nullptr) item->use();
  }

  lastAttack -= Window::deltaTime;

  if (glm::distance(Player::currentPlayer->position, position) <= 100.0f && lastAttack <= 0.0f) {
    lastAttack = cooldown;
  }
}

bool Enemy::shouldMoveToPlayer() {
  bool shouldMove = true;

  glm::vec2 hitPoint;
  float tHit;
  std::vector<Object*> ignore;

  ignore.push_back(this);

  Object* result = Object::raycast(
    position + glm::vec2(size.x * 0.5f, size.y),
    glm::vec2(0.0f, 0.5f),
    hitPoint,
    tHit,
    ignore
  );

  Object* resultR = Object::raycast(
    position + glm::vec2(size.x - 0.1f, size.y),
    glm::vec2(0.0f, 0.5f),
    hitPoint,
    tHit,
    ignore
  );

  Object* resultL = Object::raycast(
    position + glm::vec2(0.1f, size.y),
    glm::vec2(0.0f, 0.5f),
    hitPoint,
    tHit,
    ignore
  );

  if (glm::distance(Player::currentPlayer->position.x, position.x) < 250) shouldMove = false;
  if (Player::currentPlayer->currentGround != resultR &&
      Player::currentPlayer->currentGround != result &&
      Player::currentPlayer->currentGround != resultL) shouldMove = false;

  return shouldMove;
}

void Enemy::takeDamage(float dmg) {
  health = std::min(health - dmg, maxHealth);
  Particle::createParticles(position, 
                            glm::vec2(12.0f, 12.0f), 
                            0.5f, 
                            glm::vec3(1.0f, 0.0f, 0.0f), 
                            glm::vec2(0.0f, -100.0f), 
                            100.0f, 
                            1.0f, 
                            10);

  if (health <= 0) {
    pendDelete();
  }
}
