#include "Player.h"
#include "Window.h"
#include "Particle.h"
#include "Sound.h"
#include "UIParticle.h"
#include "Health.h"

#include <cmath>
#include <thread>

Player* Player::currentPlayer;
UIElement* Player::healthBar;
UIElement* Player::healthBarBackground;
bool Player::dealtFallDamage = false;

Player::Player(glm::vec2 position, glm::vec2 size, float transparency, std::string texPath, int zIndex, bool isCurrentPlayer) 
  : Object(position, size, transparency, texPath, zIndex), health(100), maxHealth(100), state("idle"), lastJump(0.0f) {
  anchored = false;
  canCollide = true;

  if (isCurrentPlayer) currentPlayer = this;
}

void Player::update() {
  if (!currentPlayer) return;
  if (!healthBar) {
    healthBar = new UIElement(glm::vec2(0.02f, 0.98f), glm::vec2(0.33f, 0.03f), 0.0f, glm::vec3(1.0f, 0.0f, 0.0f), 2);
    healthBar->anchorPoint = glm::vec2(0.0f, 1.0f);
    healthBar->registerObject();

    healthBarBackground = new UIElement(glm::vec2(0.01f, 0.99f), glm::vec2(0.35f, 0.05f), 0.25f, glm::vec3(0.25f, 0.25f, 0.25f), 1);
    healthBarBackground->anchorPoint = glm::vec2(0.0f, 1.0f);
    healthBarBackground->registerObject();
  } else {
    healthBar->size.x = std::max((currentPlayer->health / currentPlayer->maxHealth) * 0.33f, 0.0f);
  }

  glm::vec2 hitPoint;
  float tHit;
  std::vector<Object*> ignore;

  ignore.push_back(currentPlayer);

  Object* result = Object::raycast(
    currentPlayer->position + glm::vec2(currentPlayer->size.x * 0.5f, currentPlayer->size.y),
    glm::vec2(0.0f, 0.5f),
    hitPoint,
    tHit,
    ignore
  );

  Object* resultR = Object::raycast(
    currentPlayer->position + glm::vec2(currentPlayer->size.x - 0.1f, currentPlayer->size.y),
    glm::vec2(0.0f, 0.5f),
    hitPoint,
    tHit,
    ignore
  );

  Object* resultL = Object::raycast(
    currentPlayer->position + glm::vec2(0.1f, currentPlayer->size.y),
    glm::vec2(0.0f, 0.5f),
    hitPoint,
    tHit,
    ignore
  );

  Object* resultWallRight = Object::raycast(
    currentPlayer->position + glm::vec2(currentPlayer->size.x, currentPlayer->size.y - 0.1f),
    glm::vec2(0.5f, 0.0f),
    hitPoint,
    tHit,
    ignore
  );

  Object* resultWallLeft = Object::raycast(
    currentPlayer->position  + glm::vec2(0.0f, currentPlayer->size.y - 0.1f),
    glm::vec2(-0.5f, 0.0f),
    hitPoint,
    tHit,
    ignore
  );

  if (glfwGetKey(Window::window, GLFW_KEY_SPACE) == GLFW_PRESS && (result || resultR || resultL) && currentPlayer->state == "idle") {
    currentPlayer->linearVelocity = glm::normalize(currentPlayer->lastCorrection) * 500.0f;
    currentPlayer->lastJump = 1.0f;
    currentPlayer->state = "jumping";

    glm::vec2 particlePos = currentPlayer->position;
    particlePos += glm::vec2(currentPlayer->size.x / 2, currentPlayer->size.y);

    Sound::playSound("sfx/jump.wav");
    Particle::createParticles(particlePos, glm::vec2(25.0f, 25.0f), 0.5f, "textures/Wallpaper.jpeg", glm::vec2(0.0f, -100.0f), 100.0f, 1.0f, 10);
  } else if ((!(result || resultR || resultL) || currentPlayer->lastJump <= 0.0f) && currentPlayer->state == "jumping") {
    currentPlayer->state = "idle"; 
  } else if ((resultWallRight || resultWallLeft) && !(result || resultR || resultL) && currentPlayer->state != "wallJumping" && currentPlayer->lastJump <= 0.0f) {
    Object* wall;
    float wallDir;

    if (resultWallRight != nullptr) {
      wall = resultWallRight;
      wallDir = 1.0f;
    } else {
      wall = resultWallLeft;
      wallDir = 0.0f;
    }

    currentPlayer->state = "wallJumping";
    currentPlayer->gravity = 0.0f;
    currentPlayer->linearVelocity = glm::vec2(0.0f, 100.0f);
  } else if (currentPlayer->state == "wallJumping" && ((result || resultR || resultL) || !(resultWallRight || resultWallLeft))) {
    currentPlayer->state = "idle";
    currentPlayer->gravity = 500.0f;
  } else if (currentPlayer->state == "wallJumping" && glfwGetKey(Window::window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    Object* wall = resultWallLeft;
    float wallDir = 1.0f;

    if (resultWallRight != nullptr) {
      wall = resultWallRight;
      wallDir = -1.0f;
    }

    currentPlayer->state = "jumping";
    currentPlayer->lastJump = 1.0f;
    currentPlayer->gravity = 500.0f;
    currentPlayer->linearVelocity = glm::vec2(wallDir, -3.0f) * 150.0f;
    currentPlayer->position.x += currentPlayer->linearVelocity.x * Window::deltaTime;
  } else if (result || resultR || resultL) {
    currentPlayer->linearVelocity.x = 0.0f;

    if (currentPlayer->linearVelocity.y > 10000.0f) {
      Health::dealDmgToBodyPart("left foot", currentPlayer->linearVelocity.y / 1000.0f);
      currentPlayer->linearVelocity.y = 0.0f;
    }
  }

  if ((result || resultR || resultL) && currentPlayer->linearVelocity.y > 500.0f && !dealtFallDamage) {
    dealtFallDamage = true;
    Health::dealDmgToBodyPart("left foot", currentPlayer->linearVelocity.y / 50.0f);
    Health::dealDmgToBodyPart("right foot", currentPlayer->linearVelocity.y / 50.0f);
    Health::dealDmgToBodyPart("left calf", currentPlayer->linearVelocity.y / 100.0f);
    Health::dealDmgToBodyPart("right calf", currentPlayer->linearVelocity.y / 100.0f);
    Health::dealDmgToBodyPart("left thigh", currentPlayer->linearVelocity.y / 250.0f);
    Health::dealDmgToBodyPart("right thigh", currentPlayer->linearVelocity.y / 250.0f);
  } else if (!(result || resultR || resultL)) {
    dealtFallDamage = false;
  }


  currentPlayer->lastJump -= Window::deltaTime;

  if (glfwGetKey(Window::window, GLFW_KEY_D) == GLFW_PRESS && currentPlayer->state != "wallJumping") {
    currentPlayer->linearVelocity.x = 250.0f;
  } else if (glfwGetKey(Window::window, GLFW_KEY_A) == GLFW_PRESS && currentPlayer->state != "wallJumping") {
    currentPlayer->linearVelocity.x = -250.0f;
  } else if (currentPlayer->state != "wallJumping" && (currentPlayer->linearVelocity.x == -250.0f || currentPlayer->linearVelocity.x == 250.0f)) {
    currentPlayer->linearVelocity.x = 0.0f;
  }

  if (currentPlayer->position.y >= 5000.0f) {
    currentPlayer->position = glm::vec2(0.0f, 0.0f);
  }
}

float Player::getHealth() {
  return health;
}

void Player::takeDamage(float damage) {
  health -= damage;
  colorChange.x = 1.0f;

  if (!(this == currentPlayer)) return;

  glm::vec2 particlePos(healthBar->position.x + std::max((currentPlayer->health / currentPlayer->maxHealth) * 0.33f, 0.0f), healthBar->position.y - (healthBar->size.y / 2));
  UIParticle::createParticles(particlePos, glm::vec2(0.01f, 0.01f), 0.0f, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.1f, 0.0f), 0.1f, 2.5f, 10);
}

void Player::setHealth(float health) {
  this->health = health;
}

void Player::beforeUpdate() {
  colorChange.x = std::max(
      colorChange.x - (float)Window::deltaTime,
      0.0f
  );
}
