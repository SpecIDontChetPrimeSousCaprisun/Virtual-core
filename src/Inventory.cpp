#include "Inventory.h"
#include "Window.h"
#include "Player.h"

#include <cmath>
#include <sstream>
#include <iomanip>

std::map<InventoryPlaceInfo*, Item*> Inventory::items;
Item* Inventory::selectedItem = nullptr;
Container* Inventory::ui;
Container* Inventory::sideUi;
TextElement* Inventory::itemNameLabel;
TextElement* Inventory::itemSizeLabel;
UIElement* Inventory::itemImage;
UIElement* Inventory::background;
UIElement* Inventory::hoverEffect;
bool Inventory::isPressed = false;
bool Inventory::visible = false;
bool Inventory::rClickPressed = false;
float Inventory::gridSize = 3;
int Inventory::currentKey = 0;

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

  std::vector<Object*> sideUiElements;

  itemNameLabel = new TextElement(glm::vec2(0.0125f, 0.5f),
                                  glm::vec2(0.15f, 0.04f),
                                  1.0f,
                                  glm::vec3(0.0f, 0.0f, 0.0f),
                                  10,
                                  "Item name here",
                                  "fonts/JetBrainsMonoNerdFont-Regular.ttf",
                                  glm::vec3(1.0f, 1.0f, 1.0f));

  itemNameLabel->textCentered = false;

  itemSizeLabel = new TextElement(glm::vec2(0.0125f, 0.52f),
                                  glm::vec2(0.15f, 0.04f),
                                  1.0f,
                                  glm::vec3(0.0f, 0.0f, 0.0f),
                                  10,
                                  "Item size here",
                                  "fonts/JetBrainsMonoNerdFont-Regular.ttf",
                                  glm::vec3(0.7f, 0.7f, 0.7f));

  itemSizeLabel->textCentered = false;

  itemImage = new UIElement(glm::vec2(0.0125f, 0.56f),
                            glm::vec2(0.1f, 0.1f),
                            0.0f,
                            "textures/Wallpaper.jpeg",
                            10);

  sideUiElements.push_back(itemNameLabel);
  sideUiElements.push_back(itemSizeLabel);
  sideUiElements.push_back(itemImage);

  sideUi = new Container(sideUiElements);
  sideUi->changeVisibility(false);
  sideUi->registerObjects();
}

void Inventory::update() {
  if (glfwGetKey(Window::window, GLFW_KEY_E) == GLFW_PRESS && !isPressed) {
    isPressed = true;
    visible = !visible; 
  } else if (glfwGetKey(Window::window, GLFW_KEY_E) != GLFW_PRESS) {
    isPressed = false;
  }

  visible = visible && UIElement::shouldDisplayMenus;

  ui->changeVisibility(visible);

  if (!visible) {
    hoverEffect->visible = false;
    sideUi->changeVisibility(false);
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
          currentKey = GLFW_MOUSE_BUTTON_LEFT;
          selectedItem = item;
          eraseItem(item);
        }
      } else if (glfwGetMouseButton(Window::window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS && !rClickPressed) {
        rClickPressed = true;

        Item* item = getItemAtGridPos(glm::vec2(tileX, tileY));
        
        if (Item::equippedItem != nullptr) {
          selectedItem = Item::equippedItem;
          Item::equippedItem = nullptr;
          currentKey = GLFW_MOUSE_BUTTON_RIGHT;
        }
        
        if (item != nullptr) {
          item->equip();
          eraseItem(item);
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

        if (glfwGetMouseButton(Window::window, currentKey) != GLFW_PRESS) dropCurrentItem();
      } else {
        hoverEffect->color = glm::vec3(1.0f, 1.0f, 1.0f);

        if (glfwGetMouseButton(Window::window, currentKey) != GLFW_PRESS) {
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

    if (glfwGetMouseButton(Window::window, currentKey) != GLFW_PRESS && selectedItem) dropCurrentItem();
  }

  if (glfwGetMouseButton(Window::window, GLFW_MOUSE_BUTTON_RIGHT) != GLFW_PRESS && rClickPressed) rClickPressed = false;

  if (Item::equippedItem != nullptr) {
    sideUi->changeVisibility(visible);
    itemNameLabel->text = Item::equippedItem->name;
    itemImage->changeTexture(Item::equippedItem->texPath);

    std::ostringstream ss;
    ss << std::fixed << std::setprecision(0) << Item::equippedItem->size.x << "x" << Item::equippedItem->size.y;
    std::string str = ss.str();

    itemSizeLabel->text = str;
  } else sideUi->changeVisibility(false);
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

void Inventory::eraseItem(Item* item) {
  for (auto it = items.begin(); it != items.end(); ++it) {
    if (it->second == item) {
      InventoryPlaceInfo* slot = it->first;

      slot->element->pendDelete();          // destroy key object
      items.erase(it);      // remove from map

      break;
    }
  }
}
