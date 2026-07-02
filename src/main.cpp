#include "Window.h"
#include "Object.h"
#include "Player.h"
#include "Sound.h"
#include "UIElement.h"
#include "TextElement.h"
#include "Enemy.h"
#include "Item.h"
#include "Inventory.h"
#include "Health.h"
#include "Intro.h"
#include "EscapeMenu.h"
#include "Tutorial.h"
#include "DeathScreen.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int main() {
  Sound::init();

  if (Window::init() == -1) return -1;
 
  Object::initShader();
  TextElement::initShader();
  Item::init();
  Inventory::init();
  Health::init();
  Intro::init();
  EscapeMenu::init();
  Tutorial::init();
  DeathScreen::init();

  Window::mainLoop();
  Sound::unInit();
}
