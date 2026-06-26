#include "Inventory.h"
#include "Window.h"

#include <cmath>

std::map<InventoryPlaceInfo*, Item*> Inventory::items;
Container* Inventory::ui;
UIElement* Inventory::background;
UIElement* Inventory::hoverEffect;
bool Inventory::isPressed = false;
bool Inventory::visible = false;
float Inventory::gridSize = 10;

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
    ui->changeVisibility(visible);
  } else if (glfwGetKey(Window::window, GLFW_KEY_E) != GLFW_PRESS) {
    isPressed = false;
  }

  if (!visible) {
    hoverEffect->visible = false;
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

    hoverEffect->size = background->size / gridSize;
    hoverEffect->position = (local + gridOrigin) / glm::vec2(Window::fbWidth, Window::fbHeight);
    hoverEffect->visible = true;
  } else {
    hoverEffect->visible = false;
  }
}
