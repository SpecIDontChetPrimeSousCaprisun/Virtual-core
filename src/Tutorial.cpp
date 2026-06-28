#include "Tutorial.h"
#include "Window.h"

#include <cmath>

std::vector<std::string> Tutorial::tutorials;
TextElement* Tutorial::currentText;
ScrollingElement* Tutorial::background;
float Tutorial::time;
float Tutorial::lastTime;
bool Tutorial::inOpenAnimation;

void Tutorial::init() {
  tutorials.push_back("lorem");
  tutorials.push_back("ipsum");
  tutorials.push_back("sit");
  tutorials.push_back("amet");
  tutorials.push_back("dolor");

  background = new ScrollingElement(glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), 0.0f, glm::vec3(0.0f, 0.0f, 0.0f), 100);
  background->visible = false;
  background->registerObject();
}

void Tutorial::open() {
  time = 0.0f;
  lastTime = 0.0f;
  inOpenAnimation = true;
  background->visible = true;
}

void Tutorial::update() {
  if (inOpenAnimation) {
    time += Window::deltaTime;
    background->visible = true;

    if (std::floor(time / 0.5f) > lastTime && lastTime < tutorials.size()) { 
      std::cout << tutorials[lastTime] << "\n";
      currentText = new TextElement(glm::vec2(0.0f, 0.0f),
                                    glm::vec2(1.0f, 0.02f),
                                    1.0f,
                                    glm::vec3(0.0f, 0.0f, 0.0f),
                                    1001,
                                    tutorials[lastTime],
                                    "fonts/JetBrainsMonoNerdFont-Regular.ttf",
                                    glm::vec3(1.0f, 1.0f, 1.0f));

      background->elements.push_back(currentText);
      currentText->textCentered = false;
      currentText->registerObject();

      lastTime = std::floor(time / 0.5f);
    }
  }
}
