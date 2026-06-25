#include "Enemy.h"
#include "Player.h"
#include "Window.h"
#include <cmath>

Enemy::Enemy(glm::vec2 position, glm::vec2 size, std::string texPath, int zIndex) 
  : Object(position, size, 0.0f, texPath, zIndex), damage(10.0f), speed(100.0f), cooldown(1.0f), lastAttack(0.0f) {
  canCollide = true;
  anchored = false;
}

void Enemy::beforeUpdate() {
  if (!Player::currentPlayer) return;

  if (Player::currentPlayer->position.x - position.x < 1) linearVelocity.x = -speed;
  else linearVelocity.x = speed;

  lastAttack -= Window::deltaTime;

  if (glm::distance(Player::currentPlayer->position, position) <= 100.0f && lastAttack <= 0.0f) {
    lastAttack = cooldown;
    Player::currentPlayer->takeDamage(damage);
  }
}
