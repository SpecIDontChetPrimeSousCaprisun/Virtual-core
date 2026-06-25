#include "Item.h"
#include "Window.h"

TextElement* Item::interactionElement;

Item::Item(glm::vec2 position, glm::vec2 size, float transparency, std::string texPath, int zIndex, std::string name) 
  : Object(position, size, transparency, texPath, zIndex), name(name) {
  initObject();
}

Item::Item(glm::vec2 position, glm::vec2 size, float transparency, glm::vec3 color, int zIndex, std::string name) 
  : Object(position, size, transparency, color, zIndex), name(name) {
  initObject();
}

void Item::initObject() {
  anchored = true;
  canCollide = false;
}

void Item::init() {
  interactionElement = new TextElement(glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 0.1f), 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), 1,"ITEM NAME GOES HERE", "fonts/Kenney Future Narrow.ttf", glm::vec3(1.0f, 1.0f, 1.0f));

  interactionElement->visible = false;
  interactionElement->textCentered = false;
  interactionElement->registerObject();
}

void Item::beforeUpdate() {
  double mouseX;
  double mouseY;

  glfwGetCursorPos(Window::window, &mouseX, &mouseY);

  drawInfo* info = beforeDrawing();

  if (mouseX >= info->position.x &&
      mouseX <= info->position.x + info->size.x &&
      mouseY >= info->position.y &&
      mouseY >= info->position.y + info->size.y) {
    interactionElement->visible = true;
    interactionElement->text = name;
  }
}
