#include "ScrollingElement.h"
#include "Window.h"
#include <cmath>

std::vector<ScrollingElement*> ScrollingElement::scrolls;

ScrollingElement::ScrollingElement(glm::vec2 position, glm::vec2 size, float transparency, std::string texPath, int zIndex)
  : UIElement(position, size, transparency, texPath, zIndex), scrollAmount(0.0f), padding(0.0f) {
  scrolls.push_back(this);
}

ScrollingElement::ScrollingElement(glm::vec2 position, glm::vec2 size, float transparency, glm::vec3 color, int zIndex)
  : UIElement(position, size, transparency, color, zIndex), scrollAmount(0.0f), padding(0.0f) {
  scrolls.push_back(this);
}

void ScrollingElement::beforeUpdate() {
  float y = 0;

  for (UIElement* element : elements) {
    element->size.x = size.x;
    element->position = glm::vec2(position.x + padding, position.y + y + padding + scrollAmount);
    element->anchorPoint = anchorPoint;

    if (!visible) element->visible = false;
    if (element->visible) y += element->size.y; 
  }
}

void ScrollingElement::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
  double mouseX;
  double mouseY;

  glfwGetCursorPos(window, &mouseX, &mouseY);

  for (ScrollingElement* scroll : scrolls) {
    std::cout << "a\n";
    drawInfo* newInfo = scroll->beforeDrawing(new drawInfo(glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f)));

    std::cout << mouseX << " " << mouseY << "\n";
    std::cout << newInfo->position.x << " " << newInfo->position.y << "\n";

    if (!(mouseX >= newInfo->position.x &&
          mouseX <= newInfo->position.x + newInfo->size.x &&
          mouseY >= newInfo->position.y &&
          mouseY <= newInfo->position.y + newInfo->size.y)) continue;

    scroll->scrollAmount = std::min(scroll->scrollAmount + ((yoffset * 10) / Window::fbHeight), 0.0);
    std::cout << scroll->scrollAmount << "\n";
  }
}
