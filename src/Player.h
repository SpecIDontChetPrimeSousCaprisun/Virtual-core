#pragma once

#include "Object.h"
#include "UIElement.h"

class Player : public Object {
public:
  Player(glm::vec2 position, glm::vec2 size, float transparency, std::string texPath, int zIndex, bool isCurrentPlayer);
  
  static void update();

  static UIElement* healthBar;
  static UIElement* healthBarBackground;
  static Player* currentPlayer;

  float getHealth();
  void takeDamage(float damage);
  void setHealth(float health);

  float maxHealth;
protected:
  void beforeUpdate() override;
private:
  float health;
  float lastJump;
  std::string state;
};
