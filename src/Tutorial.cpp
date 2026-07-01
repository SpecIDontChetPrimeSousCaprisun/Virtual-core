#include "Tutorial.h"
#include "Window.h"
#include "LevelLoader.h"

#include <cmath>
#include <sstream>
#include <fstream>

std::vector<Object*> Tutorial::currentTutorial;
std::vector<std::string> Tutorial::tutorials;
Player* Tutorial::oldPlayer;
Button* Tutorial::currentText;
ScrollingElement* Tutorial::background;
float Tutorial::time;
float Tutorial::lastTime;
bool Tutorial::inOpenAnimation;

void Tutorial::init() {
  std::ifstream indexStream("levels/tutorials.index");
  std::string line;

  while (std::getline(indexStream, line)) {
    tutorials.push_back(line);
  }

  background = new ScrollingElement(glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), 0.0f, glm::vec3(0.0f, 0.0f, 0.0f), 100);
  background->visible = false;
  background->registerObject();
}

void Tutorial::open() {
  for (UIElement* element : background->elements) {
    element->pendDelete();
  }

  background->elements.clear();

  time = 0.0f;
  lastTime = 0.0f;
  inOpenAnimation = true;
  background->visible = true;
}

void Tutorial::close() {
  for (UIElement* element : background->elements) {
    element->pendDelete();
  }

  background->elements.clear();

  inOpenAnimation = false;
  background->visible = false;
}

void Tutorial::remove() {
  if (!oldPlayer) return;

  for (Object* object : currentTutorial) {
    object->pendDelete();
  }

  currentTutorial.clear();
  Player::currentPlayer = oldPlayer;
}

void Tutorial::update() {
  if (inOpenAnimation) {
    time += Window::deltaTime;
    background->visible = true;

    if (std::floor(time / 0.1f) > lastTime && lastTime < tutorials.size()) { 
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
      newButton->setCallback([text]() {
        oldPlayer = Player::currentPlayer;
        currentTutorial = LevelLoader::addLevel("levels/" + text);
        close();
      });

      background->elements.push_back(currentText);
      currentText->textCentered = false;
      currentText->textTransparency = 1.0f;
      currentText->registerObject();

      lastTime = std::floor(time / 0.1f);
    } else if (currentText && (time / 0.1f) - lastTime <= 1.0f) {
      currentText->textTransparency = 1 - ((time / 0.1f) - lastTime);
    }
  }
}
