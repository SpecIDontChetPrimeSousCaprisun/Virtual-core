#include "Tutorial.h"
#include "Window.h"

#include <cmath>

std::vector<std::string> Tutorial::tutorials;
Button* Tutorial::currentText;
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
      if (currentText) currentText->textTransparency = 0.0f; 

      std::string text = tutorials[lastTime];

      Button* newButton = new Button(glm::vec2(0.0f, 0.0f),
                                     glm::vec2(1.0f, 0.02f),
                                     1.0f,
                                     glm::vec3(0.0f, 0.0f, 0.0f),
                                     1001,
                                     text,
                                     "fonts/JetBrainsMonoNerdFont-Regular.ttf",
                                     glm::vec3(1.0f, 1.0f, 1.0f));

      currentText = newButton;

      newButton->setHoverCallback([newButton, text](bool hovered) {
        if (hovered) {
          newButton->text = "$ " + text;
        } else {
          newButton->text = text;
        }
      });
      currentText->setCallback([]() {
      
      });

      background->elements.push_back(currentText);
      currentText->textCentered = false;
      currentText->textTransparency = 1.0f;
      currentText->registerObject();

      lastTime = std::floor(time / 0.5f);
    } else if (currentText && (time / 0.5f) - lastTime <= 1.0f) {
      currentText->textTransparency = 1 - ((time / 0.5f) - lastTime);
    }
  }
}
