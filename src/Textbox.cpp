#include "Textbox.h"
#include "Window.h"

std::vector<Textbox*> Textbox::textboxes;

void Textbox::init() {
  glfwSetCharCallback(Window::window,
  [](GLFWwindow*, unsigned int codepoint) {
    for (Textbox* textbox : textboxes) {
      if (!textbox->focused) continue;
      textbox->text += static_cast<char>(codepoint);
    }
  });

  glfwSetKeyCallback(Window::window,
  [](GLFWwindow*, int key, int, int action, int) {
    if (key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS) {
      for (Textbox* textbox : textboxes) {
        if (!textbox->focused) continue;
        if (textbox->text.empty()) continue;
        textbox->text.pop_back();
      }
    }
  });
}

void Textbox::objectInit() {
  textboxes.push_back(this);
  placeholder = "";
  placeholderColor = glm::vec3(0.25f, 0.25f, 0.25f);
  textAnimCd = 0.5f;
  textAnim = true;
}

Textbox::Textbox(glm::vec2 position, glm::vec2 size, float transparency, std::string texPath, int zIndex,
        std::string fontPath, glm::vec3 textColor)
  : TextElement(position, size, transparency, texPath, zIndex, "", fontPath, textColor), focused(false) {
  objectInit();
}

Textbox::Textbox(glm::vec2 position, glm::vec2 size, float transparency, glm::vec3 color, int zIndex,
        std::string fontPath, glm::vec3 textColor)
  : TextElement(position, size, transparency, color, zIndex, "", fontPath, textColor), focused(false) {
  objectInit();
}

drawInfo* Textbox::beforeDrawing(drawInfo* info) {
  drawInfo* newInfo = baseTextBeforeDrawing();

  if (text == "") {
    newInfo->text = placeholder;
    newInfo->textColor = placeholderColor;
  }

  if (textAnim && focused) {
    newInfo->text += "|";
  }

  return newInfo;
}

void Textbox::beforeUpdate() {
  double mouseX;
  double mouseY;

  glfwGetCursorPos(Window::window, &mouseX, &mouseY);

  drawInfo* newInfo = baseBeforeDrawing(new drawInfo(glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f)));

  if (glfwGetMouseButton(Window::window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && !changedFocus) {
    changedFocus = true;
    focused = mouseX >= newInfo->position.x &&
              mouseX <= newInfo->position.x + newInfo->size.x &&
              mouseY >= newInfo->position.y &&
              mouseY <= newInfo->position.y + newInfo->size.y;
  } else if (glfwGetMouseButton(Window::window, GLFW_MOUSE_BUTTON_LEFT) != GLFW_PRESS) {
    changedFocus = false;
  }

  if (focused) {
    colorChange = glm::vec3(-0.1f, -0.1f, -0.1f);
    textAnimCd -= Window::deltaTime;

    if (textAnimCd <= 0.0f) {
      textAnimCd = 0.5f;
      textAnim = !textAnim;
    }
  } else {
    colorChange = glm::vec3(0.0f, 0.0f, 0.0f);
    textAnimCd = 0.5f;
    textAnim = true;
  }
}

void Textbox::setFocused(bool focused) {
  focused = focused;
  changedFocus = true;
}

bool Textbox::isFocused() {
  return focused;
}
