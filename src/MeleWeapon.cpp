#include "MeleWeapon.h"
#include "Window.h"
#include "Player.h"
#include "Enemy.h"

#include <cmath>

MeleWeapon::MeleWeapon(glm::vec2 position, glm::vec2 size, float transparency, std::string texPath, int zIndex, std::string name)
  : Item(position, size, transparency, texPath, zIndex, name), hitTime(0.0f), cooldown(1.0f), damage(10.0f) {}

MeleWeapon::MeleWeapon(glm::vec2 position, glm::vec2 size, float transparency, glm::vec3 color, int zIndex, std::string name)
  : Item(position, size, transparency, color, zIndex, name), hitTime(0.0f), cooldown(1.0f), damage(10.0f) {}

void MeleWeapon::use() {
  if (hitTime > 0.0f) return;
  
  hitTime = cooldown;
  std::vector<Object*> hits = Object::getAllObjectsInBounds(Player::currentPlayer->position, 
                                                            Player::currentPlayer->size * glm::vec2(2.0f, 1.0f),
                                                            0.0f);

  for (Object* hit : hits) {
    Enemy* enemy = dynamic_cast<Enemy*>(hit);
    
    if (!enemy) continue;

    enemy->takeDamage(damage);
  }
}

void MeleWeapon::itemUpdate() {
  hitTime = std::max((float)(hitTime - Window::deltaTime), 0.0f);
}
