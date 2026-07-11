#pragma once

#include "AnimatedObject.h"
#include "UIElement.h"
#include "Light.h"

class Player : public AnimatedObject {
public:
  Player(glm::vec2 position, glm::vec2 size, float transparency, int zIndex, bool isCurrentPlayer);
  
  static void update();

  static UIElement* healthBar;
  static UIElement* healthBarBackground;
  static Player* currentPlayer;

  float getHealth();
  void takeDamage(float damage);
  void setHealth(float health);

  float maxHealth;
  float speedMult;
  Object* currentGround;
protected:
  void afterTextureUpdate() override;
private:
  static bool dealtFallDamage;

  float health;
  float lastJump;
  std::string state;
  Light* light;
};
