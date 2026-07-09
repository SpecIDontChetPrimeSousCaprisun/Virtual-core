#include "Gun.h"
#include "Window.h"
#include "Bullet.h"
#include "Player.h"

#include <sstream>
#include <cmath>

bool Gun::initedUi = false;
TextElement* Gun::ammoText = nullptr;

Gun::Gun(glm::vec2 position, glm::vec2 size, float transparency, std::string texPath, int zIndex, std::string name) :
  Item(position, size, transparency, texPath, zIndex, name) {
  init();
}

Gun::Gun(glm::vec2 position, glm::vec2 size, float transparency, glm::vec3 color, int zIndex, std::string name) :
  Item(position, size, transparency, color, zIndex, name) {
  init();
}

void Gun::init() {
  if (!initedUi) initUi();

  firerate = 0.5f;
  lastShot = 0.0f;
  maxBullets = 10;
  bullets = maxBullets;
}

void Gun::use() {
  if (lastShot <= 0.0f && bullets > 0) {
    double mouseX;
    double mouseY;

    glfwGetCursorPos(Window::window, &mouseX, &mouseY);

    position = Player::currentPlayer->position;

    drawInfo* info = beforeDrawing();

    lastShot = firerate;
    bullets -= 1;
    Bullet* bullet = new Bullet(Player::currentPlayer->position, glm::vec2(mouseX, mouseY) - info->position, zIndex);
    bullet->registerObject();
  }
}

void Gun::itemUpdate() {
  lastShot = std::max((float)(lastShot - Window::deltaTime), 0.0f);

  if (glfwGetKey(Window::window, GLFW_KEY_R) == GLFW_PRESS) {
    lastShot = 1.0f;
    bullets = maxBullets;
  }

  if (Item::equippedItem == nullptr) return;

  Gun* gun = dynamic_cast<Gun*>(Item::equippedItem);
  ammoText->visible = gun != nullptr;

  if (gun == this) {
    std::ostringstream stream;
    stream << bullets << " / " << maxBullets;
    ammoText->text = stream.str();
  }
}

void Gun::initUi() {
  ammoText = new TextElement(glm::vec2(0.0f, 1.0f),
                             glm::vec2(0.15f, 0.025f),
                             1.0f,
                             glm::vec3(0.0f, 0.0f, 0.0f),
                             1,
                             "XX / XX",
                             "fonts/JetBrainsMonoNerdFont-Regular.ttf",
                             glm::vec3(1.0f, 1.0f, 1.0f));
  
  ammoText->anchorPoint = glm::vec2(0.0f, 1.0f);
  ammoText->visible = false;
  ammoText->textCentered = false;
  ammoText->registerObject();
}
