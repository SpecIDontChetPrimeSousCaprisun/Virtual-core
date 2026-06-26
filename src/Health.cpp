#include "Health.h"
#include "Window.h"

#include <cmath>

bool Health::pressed = false;
bool Health::playingOpenAnim = false;
float Health::openAnimTime = 0.0f;
UIElement* Health::background;
std::vector<TextElement*> Health::cout;
std::vector<std::string> Health::coutText;
int Health::lastCoutText = 0;

void Health::init() {
  background = new UIElement(glm::vec2(0.5f, 0.5f), glm::vec2(0.75f, 0.75f), 0.05f, glm::vec3(0.0f, 0.0f, 0.0f), 10);

  background->anchorPoint = glm::vec2(0.5f, 0.5f);
  background->visible = false;
  background->registerObject();

  coutText.push_back(R"( _   _ _____    _    _   _____ _   _ )");
  coutText.push_back(R"(| | | | ____|  / \  | | |_   _| | | |)");
  coutText.push_back(R"(| |_| |  _|   / _ \ | |   | | | |_| |)");
  coutText.push_back(R"(|  _  | |___ / ___ \| |___| | |  _  |)");
  coutText.push_back(R"(|_| |_|_____/_/   \_\_____|_| |_| |_|)");
  coutText.push_back("");
  coutText.push_back("Wellcome back, Hope !");
  coutText.push_back("Starting heartbeat monitor...");
  coutText.push_back("Getting vital organs' state...");
  coutText.push_back("Checking limbs health...");
  coutText.push_back("Initialising shaders...");
  coutText.push_back("Moving logs to /etc/HEALTH/oe46a67pnr942.log...");
}

float easeOutQuint(float t) {
    float u = 1.0f - t;
    return 1.0f - u * u * u * u * u;
}

void Health::update() {
  if (glfwGetKey(Window::window, GLFW_KEY_Q) && !playingOpenAnim) {
    if (background->visible) {

    } else {
      background->visible = true;
      background->size = glm::vec2(0.0f, 0.0f);
      playingOpenAnim = true;
      openAnimTime = 0.0f;
      lastCoutText = 0;
    }
  }

  if (playingOpenAnim) {
    openAnimTime += Window::deltaTime;

    if (openAnimTime <= 2.0f) {
      background->size = glm::vec2(0.75f, 0.75f) * easeOutQuint(openAnimTime / 2.0f);
    } else if (openAnimTime * 5.0f < coutText.size() + 1) {
      background->size = glm::vec2(0.75f, 0.75f);
      
      if ((std::floor(openAnimTime * 5.0f)) > lastCoutText) { 
        TextElement* text = new TextElement(glm::vec2(0.5f,0.11f + (0.02f * lastCoutText)),
                                           glm::vec2(0.75f, 0.02f),
                                           1.0f,
                                           glm::vec3(0.0f, 0.0f, 0.0f),
                                           11,
                                           coutText[lastCoutText],
                                           "fonts/JetBrainsMonoNerdFont-Regular.ttf",
                                           glm::vec3(1.0f, 1.0f, 1.0f));

        text->anchorPoint = glm::vec2(0.5f, 0.0f);
        text->textCentered = false;
        text->registerObject();

        cout.push_back(text);
        lastCoutText++;
      }
    }
  }
}
