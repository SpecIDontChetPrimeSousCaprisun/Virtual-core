#include "Inventory.h"
#include "Window.h"
#include "Player.h"

#include <cmath>

std::map<InventoryPlaceInfo*, Item*> Inventory::items;
Item* Inventory::selectedItem = nullptr;
Container* Inventory::ui;
UIElement* Inventory::background;
UIElement* Inventory::hoverEffect;
bool Inventory::isPressed = false;
bool Inventory::visible = false;
float Inventory::gridSize = 3;

void Inventory::init() {
  std::vector<Object*> uiElements;

  background = new UIElement(glm::vec2(0.5f, 0.5f), glm::vec2(0.75f, 0.75f), 0.05f, glm::vec3(0.05f, 0.05f, 0.05f), 10);
  hoverEffect = new UIElement(glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), 0.5f, glm::vec3(1.0f, 1.0f, 1.0f), 11);

  background->anchorPoint = glm::vec2(0.5f, 0.5f);

  hoverEffect->visible = false;
  hoverEffect->registerObject();

  uiElements.push_back(background);

  ui = new Container(uiElements);
  ui->changeVisibility(false);
  ui->registerObjects();
}

void Inventory::update() {
  if (glfwGetKey(Window::window, GLFW_KEY_E) == GLFW_PRESS && !isPressed) {
    isPressed = true;
    visible = !visible; 
  } else if (glfwGetKey(Window::window, GLFW_KEY_E) != GLFW_PRESS) {
    isPressed = false;
  }

  ui->changeVisibility(visible);

  if (!visible) {
    hoverEffect->visible = false;
    selectedItem = nullptr;
    return;
  }

  double mouseX;
  double mouseY;

  glfwGetCursorPos(Window::window, &mouseX, &mouseY);
  glm::vec2 pos = background->getActualPosition();
  glm::vec2 size = background->getActualSize();

  glm::vec2 cellSize = size / gridSize;
  glm::vec2 gridOrigin = background->getActualPosition();

  if (mouseX >= pos.x &&
      mouseX <= pos.x + size.x &&
      mouseY >= pos.y &&
      mouseY <= pos.y + size.y) { 
    glm::vec2 local = glm::vec2(mouseX, mouseY) - gridOrigin;

    local.x = std::floor(local.x / cellSize.x) * cellSize.x;
    local.y = std::floor(local.y / cellSize.y) * cellSize.y;

    int tileX = (int)(local.x / cellSize.x);
    int tileY = (int)(local.y / cellSize.y);

    if (selectedItem == nullptr) {
      hoverEffect->size = background->size / gridSize;
      hoverEffect->color = glm::vec3(1.0f, 1.0f, 1.0f);

      if (glfwGetMouseButton(Window::window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        Item* item = getItemAtGridPos(glm::vec2(tileX, tileY));

        if (item != nullptr) {
          selectedItem = item;
          for (auto it = items.begin(); it != items.end(); ++it) {
            if (it->second == item) {
              InventoryPlaceInfo* slot = it->first;

              slot->element->pendDelete();          // destroy key object
              items.erase(it);      // remove from map

              break;
            }
          }
        }
      }
    } else {
      hoverEffect->size = (background->size / gridSize) * selectedItem->size; 

      if ((tileX + selectedItem->size.x) > gridSize || (tileY + selectedItem->size.y) > gridSize) {
        hoverEffect->visible = false;
        return;
      }

      InventoryPlaceInfo occupiedInfo = InventoryPlaceInfo();

      occupiedInfo.position = glm::vec2(tileX, tileY);
      occupiedInfo.size = selectedItem->size;
      occupiedInfo.rotation = 0.0f;

      if (isOccupied(&occupiedInfo)) {
        hoverEffect->color = glm::vec3(1.0f, 0.0f, 0.0f);

        if (glfwGetMouseButton(Window::window, GLFW_MOUSE_BUTTON_LEFT) != GLFW_PRESS) dropCurrentItem();
      } else {
        hoverEffect->color = glm::vec3(1.0f, 1.0f, 1.0f);

        if (glfwGetMouseButton(Window::window, GLFW_MOUSE_BUTTON_LEFT) != GLFW_PRESS) {
          InventoryPlaceInfo* info = new InventoryPlaceInfo();

          info->position = glm::vec2(tileX, tileY);
          info->size = selectedItem->size;
          info->rotation = 0.0f;
          info->element = new UIElement(hoverEffect->position, hoverEffect->size, 0.0f, selectedItem->texPath, 11);
          info->element->registerObject();

          ui->objects.push_back(info->element);

          items[info] = selectedItem;
          selectedItem->visible = false;
          selectedItem = nullptr;
        }
      }
    }

    hoverEffect->position = (local + gridOrigin) / glm::vec2(Window::fbWidth, Window::fbHeight);
    hoverEffect->visible = true;
  } else {
    hoverEffect->visible = false;

    if (glfwGetMouseButton(Window::window, GLFW_MOUSE_BUTTON_LEFT) != GLFW_PRESS && selectedItem) dropCurrentItem();
  }
}

bool Inventory::isOccupied(InventoryPlaceInfo* info) {
  bool occupied = false;

  for (auto& [other, item] : items) {
    if (info->position.x < other->position.x + other->size.x &&
        info->position.x + info->size.x > other->position.x &&
        info->position.y < other->position.y + other->size.y &&
        info->position.y + info->size.y > other->position.y) {
      occupied = true;
      break;
    }
  }

  return occupied;
}

Item* Inventory::getItemAtGridPos(glm::vec2 position) {
  Item* item = nullptr;

  for (auto& [other, loopedItem] : items) {
    if (position.x >= other->position.x &&
        position.x < other->position.x + other->size.x &&
        position.y >= other->position.y &&
        position.y < other->position.y + other->size.y) {
      item = loopedItem;
      break;
    }
  }

  return item;
}

void Inventory::dropCurrentItem() {
  double mouseX;
  double mouseY;

  glfwGetCursorPos(Window::window, &mouseX, &mouseY);

  selectedItem->position = Player::currentPlayer->position;
  selectedItem->visible = true;
  selectedItem = nullptr;
}
