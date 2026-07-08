#include "Gun.h"
#include "Window.h"
#include "Bullet.h"
#include "Player.h"

#include <cmath>

Gun::Gun(glm::vec2 position, glm::vec2 size, float transparency, std::string texPath, int zIndex, std::string name) :
  Item(position, size, transparency, texPath, zIndex, name) {
  init();
}

Gun::Gun(glm::vec2 position, glm::vec2 size, float transparency, glm::vec3 color, int zIndex, std::string name) :
  Item(position, size, transparency, color, zIndex, name) {
  init();
}

void Gun::init() {
  firerate = 0.5f;
  lastShot = 0.0f;
}

void Gun::use() {
  if (lastShot <= 0.0f) {
    lastShot = firerate;
    Bullet* bullet = new Bullet(Player::currentPlayer->position, glm::vec2(-1.0f, -1.0f), zIndex);
    bullet->registerObject();
  }
}

void Gun::itemUpdate() {
  lastShot = std::max((float)(lastShot - Window::deltaTime), 0.0f);
}
