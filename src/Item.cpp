#include "Item.h"
#include "Window.h"
#include "Inventory.h"
#include "Player.h"

TextElement* Item::interactionElement;
bool Item::wasSelected;
Item* Item::equippedItem = nullptr;

Item::Item(glm::vec2 position, glm::vec2 size, float transparency, std::string texPath, int zIndex, std::string name) 
  : Object(position, size, transparency, texPath, zIndex), name(name), specialOwner(nullptr) {
  initObject();
}

Item::Item(glm::vec2 position, glm::vec2 size, float transparency, glm::vec3 color, int zIndex, std::string name) 
  : Object(position, size, transparency, color, zIndex), name(name), specialOwner(nullptr) {
  initObject();
}

void Item::initObject() {
  anchored = false;
  canCollide = true;
  collisionGroup = CollisionGroup::Item;
  size = glm::vec2(1.0f, 2.0f);
}

void Item::init() {
  interactionElement = new TextElement(glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 0.01f), 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), 1,"ITEM NAME GOES HERE", "fonts/Kenney Future Narrow.ttf", glm::vec3(1.0f, 1.0f, 1.0f));

  interactionElement->visible = false;
  interactionElement->textCentered = false;
  interactionElement->registerObject();
}

void Item::update() {
  interactionElement->visible = wasSelected;
  wasSelected = false;
}

void Item::beforeUpdate() {
  itemUpdate();
  canCollide = !(equippedItem == this);
  anchored = equippedItem == this;

  double mouseX;
  double mouseY;

  glfwGetCursorPos(Window::window, &mouseX, &mouseY);
  drawInfo* info = beforeDrawing();

  if (equippedItem == this) {
    if (glfwGetMouseButton(Window::window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) use();
    glm::vec2 dir = glm::vec2(mouseX, mouseY) - info->position;

    position = (Player::currentPlayer->position + (Player::currentPlayer->size / 2.0f) - (info->size / 2.0f)) + (glm::normalize(dir) * (info->size.x / 2.0f));
    rotation = glm::degrees(std::atan2(dir.y, dir.x));
    
    if (rotation > 90 || rotation < -90) {
      rotation -= 180;
      flipH = true;
    } else flipH = false;

    return;
  }

  if (mouseX >= info->position.x &&
      mouseX <= info->position.x + info->size.x &&
      mouseY >= info->position.y &&
      mouseY <= info->position.y + info->size.y &&
      visible) {
    interactionElement->visible = true;
    interactionElement->text = name;
    interactionElement->position = glm::vec2(mouseX / Window::fbWidth, mouseY / Window::fbHeight);
    wasSelected = true;

    if (glfwGetMouseButton(Window::window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
      Inventory::currentKey = GLFW_MOUSE_BUTTON_LEFT;
      Inventory::selectedItem = this;
      Inventory::visible = true;
    }
  }
}

void Item::equip() {
  equippedItem = this;
  visible = true;
}

void Item::use() {}

void Item::itemUpdate() {}
