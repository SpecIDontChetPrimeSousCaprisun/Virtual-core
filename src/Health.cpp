#include "Health.h"
#include "Window.h"

bool Health::pressed = false;
bool Health::playingOpenAnim = false;
float Health::openAnimTime = 0.0f;
UIElement* Health::background;

void Health::init() {
  background = new UIElement(glm::vec2(0.5f, 0.5f), glm::vec2(0.75f, 0.75f), 0.05f, glm::vec3(0.0f, 0.0f, 0.0f), 10);

  background->anchorPoint = glm::vec2(0.5f, 0.5f);
  background->visible = false;
  background->registerObject();
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
    }
  }

  if (playingOpenAnim) {
    openAnimTime += Window::deltaTime;

    if (openAnimTime <= 2.0f) {
      background->size = glm::vec2(0.75f, 0.75f) * easeOutQuint(openAnimTime / 2.0f);
    }
  }
}
