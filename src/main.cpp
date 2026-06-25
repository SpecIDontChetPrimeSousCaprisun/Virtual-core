#include "Window.h"
#include "Object.h"
#include "Player.h"
#include "Sound.h"
#include "UIElement.h"
#include "TextElement.h"
#include "Enemy.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int main() {
  Sound::init();

  if (Window::init() == -1) return -1;
 
  Object::initShader();
  TextElement::initShader();

  Player* plr = new Player(glm::vec2(0.0f, 0.0f), glm::vec2(100.0f, 100.0f), 0.0f, "textures/player.png", 2, true);
  Enemy* enemy = new Enemy(glm::vec2(-500.0f, 0.0f), glm::vec2(100.0f, 100.0f), "textures/player.png", 2);
  Object* obj2 = new Object(glm::vec2(300.0f, 0.0f), glm::vec2(100.0f, 10000.0f), 0.0f, "textures/box.png", 2);
  Object* platform = new Object(glm::vec2(-500.0f, 500.0f), glm::vec2(1000.0f, 1000.0f), 0.0f, "textures/Wallpaper.jpeg", 1);
  Object* background = new Object(glm::vec2(-750.0f, 250.0f), glm::vec2(1500.0f, 1500.0f), 0.0f, "textures/Wallpaper.jpeg", -1);

  plr->registerObject();

  //enemy->registerObject();

  //obj2->anchored = false;
  obj2->canCollide = true;
  obj2->linearVelocity = glm::vec2(-100.f, 0.0f);
  obj2->rotation = 0;
  obj2->registerObject();

  platform->canCollide = true;
  platform->rotation = 15;
  platform->registerObject();

  background->parallaxFactor = 5.0f;
  background->registerObject();
 
  Window::mainLoop();
  Sound::unInit();
}
