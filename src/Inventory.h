#pragma once

#include <map>

#include "Item.h"
#include "UIElement.h"
#include "Container.h"

struct InventoryPlaceInfo {
public:
  glm::vec2 position, size;
  float rotation;
  UIElement* element;
};

class Inventory {
public:
  static std::map<InventoryPlaceInfo*, Item*> items;
  static Item* selectedItem;
  static bool visible;
  static int currentKey;

  static void init();
  static void update();
private:
  static Container* ui;
  static UIElement* background;
  static UIElement* hoverEffect;
  static bool isPressed;
  static bool rClickPressed;
  static float gridSize; 

  static bool isOccupied(InventoryPlaceInfo* info);
  static Item* getItemAtGridPos(glm::vec2 position);
  static void dropCurrentItem();
  static void eraseItem(Item* item);
};
