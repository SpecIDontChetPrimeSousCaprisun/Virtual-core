#include "Window.h"
#include "UIElement.h"

UIElement::UIElement(glm::vec2 position, glm::vec2 size, float transparency, std::string texPath, int zIndex)
  : Object(position, size, transparency, texPath, 9999 + zIndex) {
  anchorPoint = glm::vec2(0.0f, 0.0f);
}

UIElement::UIElement(glm::vec2 position, glm::vec2 size, float transparency, glm::vec3 color, int zIndex)
  : Object(position, size, transparency, color, 9999 + zIndex) {
  anchorPoint = glm::vec2(0.0f, 0.0f);
}

drawInfo* UIElement::beforeDrawing() {
  return baseBeforeDrawing();
}

drawInfo* UIElement::baseBeforeDrawing() {
  glm::vec2 transformedPos = position - (size * anchorPoint);

  glm::vec2 pixelPos(transformedPos.x * Window::fbWidth, transformedPos.y * Window::fbHeight);
  glm::vec2 pixelSize(size.x * Window::fbWidth, size.y * Window::fbHeight);

  return new drawInfo(pixelPos, pixelSize);
}
