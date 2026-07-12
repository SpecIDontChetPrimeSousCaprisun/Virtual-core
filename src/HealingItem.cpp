#include "HealingItem.h"

HealingItem* HealingItem::equippedHealingItem = nullptr;

HealingItem::HealingItem(glm::vec2 position, glm::vec2 size, float transparency, std::string texPath, int zIndex, std::string name)
  : Item(position, size, transparency, texPath, zIndex, name) {
  initObject();
}

HealingItem::HealingItem(glm::vec2 position, glm::vec2 size, float transparency, glm::vec3 color, int zIndex, std::string name)
  : Item(position, size, transparency, color, zIndex, name) {
  initObject();
}

void HealingItem::initObject() {
  healingAmount = 10.0f;
}

void HealingItem::equip() {
  equippedItem = this;
  visible = true;
  equippedHealingItem = this;
}
