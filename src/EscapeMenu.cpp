#include "EscapeMenu.h"
#include "UIElement.h"
#include "Button.h"
#include "Window.h"
#include "Tutorial.h"

Container* EscapeMenu::ui;
bool EscapeMenu::isPressed = false;
bool EscapeMenu::opened = false;

void EscapeMenu::init() {
  std::vector<Object*> uiElements;

  UIElement* background = new UIElement(glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), 0.1f, glm::vec3(0.0f, 0.0f, 0.0f), 100);

  Button* tutorialsButton = new Button(glm::vec2(0.0f, 0.675f),
                                  glm::vec2(1.0f, 0.075f),
                                  1.0f,
                                  glm::vec3(0.0f, 0.0f, 0.0f),
                                  101,
                                  "Tutorials",
                                  "fonts/JetBrainsMonoNerdFont-Regular.ttf",
                                  glm::vec3(1.0f, 1.0f, 1.0f));

  Button* quitButton = new Button(glm::vec2(0.0f, 0.75f),
                                  glm::vec2(1.0f, 0.075f),
                                  1.0f,
                                  glm::vec3(0.0f, 0.0f, 0.0f),
                                  101,
                                  "Quit",
                                  "fonts/JetBrainsMonoNerdFont-Regular.ttf",
                                  glm::vec3(1.0f, 1.0f, 1.0f));

  tutorialsButton->textCentered = false;
  tutorialsButton->setHoverCallback([tutorialsButton](bool hovered) {
    if (hovered) {
      tutorialsButton->text = "$ Tutorials";
    } else {
      tutorialsButton->text = "Tutorials";
    }
  });
  tutorialsButton->setCallback([]() {
    opened = false;
    ui->changeVisibility(opened);
    Tutorial::open();
  });

  quitButton->textCentered = false;
  quitButton->setHoverCallback([quitButton](bool hovered) {
    if (hovered) {
      quitButton->text = "$ Quit";
    } else {
      quitButton->text = "Quit";
    }
  });
  quitButton->setCallback([]() {
    Window::inGame = false;
  });

  uiElements.push_back(tutorialsButton);
  uiElements.push_back(quitButton);
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
