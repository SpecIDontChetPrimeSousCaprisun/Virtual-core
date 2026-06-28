#include "EscapeMenu.h"
#include "UIElement.h"
#include "Window.h"

Container* EscapeMenu::ui;
bool EscapeMenu::isPressed = false;
bool EscapeMenu::opened = false;

void EscapeMenu::init() {
  std::vector<Object*> uiElements;

  UIElement* background = new UIElement(glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), 0.1f, glm::vec3(0.0f, 0.0f, 0.0f), 100);

  uiElements.push_back(background);

  ui = new Container(uiElements);
  ui->changeVisibility(false);
  ui->registerObjects();
}

void EscapeMenu::update() {
  if (glfwGetKey(Window::window, GLFW_KEY_ESCAPE) && !isPressed) {
    opened = !opened;
    ui->changeVisibility(opened);
    isPressed = true;
  } else if (!glfwGetKey(Window::window, GLFW_KEY_ESCAPE)) {
    isPressed = false;
  }
}
