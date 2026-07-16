#include "Gun.h"
#include "Window.h"
#include "Bullet.h"
#include "Player.h"
#include "Enemy.h"
#include "Particle.h"

#include <sstream>
#include <cmath>

bool Gun::initedUi = false;
TextElement* Gun::ammoText = nullptr;
std::map<Light*, float> Gun::lights;
ScreenEffect* Gun::effect = nullptr;

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
  barrelHeat = 0.0f;
  lastParticle = 0.0f;
}

void Gun::use() {
  if (lastShot <= 0.0f && bullets > 0) {
    double mouseX;
    double mouseY;

    glfwGetCursorPos(Window::window, &mouseX, &mouseY);

    glm::vec2 oldPos = position;

    if (specialOwner != nullptr) position = specialOwner->position;
    else position = Player::currentPlayer->position;

    drawInfo* info = beforeDrawing();

    lastShot = firerate;
    bullets -= 1;
    barrelHeat += 1.0f;
    Bullet* bullet;

    glm::vec2 dir = glm::vec2(mouseX, mouseY) - info->position;

    // screen direction
    glm::vec2 screenDir = glm::vec2(mouseX, mouseY) - info->position;

    // the camera offset between world and screen
    glm::vec2 cameraOffset = (Player::currentPlayer->position / parallaxFactor) 
                            - glm::vec2(Window::fbWidth, Window::fbHeight) / 2.0f
                            + Player::currentPlayer->size / 2.0f;

    // mouse in world space
    glm::vec2 mouseWorld = glm::vec2(mouseX, mouseY) + cameraOffset;

    // world direction from gun to mouse
    glm::vec2 worldDir = mouseWorld - position;

    position = oldPos;

    glm::vec2 rayPos;
    glm::vec2 hitPoint;
    float tHit;
    std::vector<Object*> ignore;
    std::vector<CollisionGroup> masks;

    Object* result;

    if (specialOwner != nullptr) {
      ignore.push_back(specialOwner);
      rayPos = glm::vec2(100000.0f, 100000.0f);
    } else { 
      ignore.push_back(Player::currentPlayer);
      rayPos = position + (glm::normalize(worldDir) * size.x);
      masks.push_back(CollisionGroup::Enemy);
    }

    masks.push_back(CollisionGroup::Default);

    ignore.push_back(this);
    result = Object::raycast(rayPos,
                             glm::normalize(worldDir) * 9999999999999.0f,
                             hitPoint,
                             tHit,
                             ignore,
                             masks);

    if (!result) {
      hitPoint = glm::vec2(0.0f, 0.0f);
    }

    Light* light = new Light(rayPos, glm::vec3(1.0f, 1.0f, 0.0f), 350.0f, 10);
    lights[light] = 0.1f;

    Particle* casing = new Particle(position,
                                    glm::vec2(12.0f, 5.5f),
                                    0.0f,
                                    glm::vec3(1.0f, 1.0f, 0.0f),
                                    glm::vec2(0.0f, -500.0f),
                                    50.0f,
                                    10.0f);

    casing->gravity = 1000.0f;
    casing->registerObject();

    if (!result) return;
    if (specialOwner != nullptr) {

    } else {
      Enemy* enemy = dynamic_cast<Enemy*>(result);

      if (enemy) enemy->takeDamage(10.0f);
      else Particle::createParticles(hitPoint, 
                                     glm::vec2(12.0f, 12.0f), 
                                     0.5f, 
                                     result->color, 
                                     glm::vec2(0.0f, -100.0f), 
                                     100.0f, 
                                     1.0f, 
                                     10);
    }

    if (glm::distance(hitPoint, Player::currentPlayer->position) <= 200) {
      if (effect) effect->visible = false;
      effect = new ScreenEffect(glm::vec3(0.25f, 0.25f, 0.25f), 0.5f);
      effect->registerObject();
    }

    /*if (specialOwner != nullptr) {
      bullet = new Bullet(specialOwner->position, Player::currentPlayer->position - specialOwner->position, zIndex);
      bullet->damageTarget = "player";
    } else bullet = new Bullet(position + (glm::normalize(dir) * info->size.x), glm::vec2(mouseX, mouseY) - info->position, zIndex);*/

    //bullet->registerObject();
  } else if (specialOwner != nullptr && bullets <= 0) {
    lastShot = 1.0f;
    bullets = maxBullets;
  }
}

void Gun::itemUpdate() {
  lastShot = std::max((float)(lastShot - Window::deltaTime), 0.0f);
  barrelHeat = std::max((float)(barrelHeat - Window::deltaTime), 0.0f);
  lastParticle = std::max((float)(lastParticle - Window::deltaTime), 0.0f);

  if (barrelHeat > 2.0f && lastParticle <= 0.0f) {
    Particle* particle = new Particle(position,
                                      glm::vec2(5.0f, 5.0f),
                                      0.1f,
                                      glm::vec3(0.1f, 0.1f, 0.1f),
                                      glm::vec2(0.0f, -100.0f),
                                      10.0f,
                                      1.0f);

    particle->gravity = 0.0f;
    particle->registerObject();
    lastParticle = 0.05f;
  }

  if (Item::equippedItem == nullptr) return;

  Gun* gun = dynamic_cast<Gun*>(Item::equippedItem);
  ammoText->visible = gun != nullptr;

  if (gun == this) {
    if (glfwGetKey(Window::window, GLFW_KEY_R) == GLFW_PRESS) {
      lastShot = 1.0f;
      bullets = maxBullets;
    }

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

void Gun::update() {
  std::vector<Light*> deletedLights;

  for (auto& [light, time] : lights) {
    time -= Window::deltaTime;
    light->intensity = time * 100.0f;
    if (time <= 0.0f) deletedLights.push_back(light);
  }

  for (Light* light : deletedLights) {
    lights.erase(light);
    delete light;
  }

  if (effect) {
    effect->transparency = std::min((float)(effect->transparency + (Window::deltaTime * 2.5)), 1.0f);
  }
}
