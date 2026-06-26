#pragma once

#include <map>

#include "Item.h"
#include "Container.h"

struct InventoryPlaceInfo {
public:
  glm::vec2 position, size;
  float rotation;
};

class Inventory {
public:
  static std::map<InventoryPlaceInfo*, Item*> items;

  static void init();
  static void update();
private:
  static Container* ui;
  static bool isPressed;
  static bool visible;
};
