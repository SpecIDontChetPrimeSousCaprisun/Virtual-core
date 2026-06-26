#include "Inventory.h"
#include "UIElement.h"
#include "Window.h"

std::map<InventoryPlaceInfo*, Item*> Inventory::items;
Container* Inventory::ui;
bool Inventory::isPressed = false;
bool Inventory::visible = false;

void Inventory::init() {
  std::vector<Object*> uiElements;

  UIElement* background = new UIElement(glm::vec2(0.5f, 0.5f), glm::vec2(0.75f, 0.75f), 0.05f, glm::vec3(0.05f, 0.05f, 0.05f), 10);

  background->anchorPoint = glm::vec2(0.5f, 0.5f);

  uiElements.push_back(background);

  ui = new Container(uiElements);
  ui->changeVisibility(false);
  ui->registerObjects();
}

void Inventory::update() {
  if (glfwGetKey(Window::window, GLFW_KEY_E) == GLFW_PRESS && !isPressed) {
    isPressed = true;
    visible = !visible;
    ui->changeVisibility(visible);
  } else if (glfwGetKey(Window::window, GLFW_KEY_E) != GLFW_PRESS) {
    isPressed = false;
  }
}
