#pragma once

#include "Item.h"

class HealingItem : public Item {
public:
  HealingItem(glm::vec2 position, glm::vec2 size, float transparency, std::string texPath, int zIndex, std::string name);
  HealingItem(glm::vec2 position, glm::vec2 size, float transparency, glm::vec3 color, int zIndex, std::string name);

  static HealingItem* equippedHealingItem;

  void equip() override;

  float healingAmount;
private:
  void initObject();
};
