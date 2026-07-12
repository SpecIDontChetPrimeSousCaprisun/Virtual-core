#include "Health.h"
#include "Window.h"
#include "DeathScreen.h"
#include "Player.h"
#include "Particle.h"
#include "HealingItem.h"

#include <cmath>

bool Health::pressed = false;
bool Health::playingOpenAnim = false;
float Health::openAnimTime = 0.0f;
UIElement* Health::background;
std::vector<TextElement*> Health::cout;
std::vector<std::string> Health::coutText;
int Health::lastCoutText = 0;
std::map<std::string, Button*> Health::bodyParts;
std::map<std::string, float> Health::bodyPartHealth;
Container* Health::ui;

void Health::init() {
  std::vector<Object*> uiElements;

  background = new UIElement(glm::vec2(0.5f, 0.5f), glm::vec2(0.75f, 0.75f), 0.05f, glm::vec3(0.0f, 0.0f, 0.0f), 10);

  background->anchorPoint = glm::vec2(0.5f, 0.5f);
 
  uiElements.push_back(background);

  // Body parts
  bodyParts["torso"] = new Button(glm::vec2(0.5f, 0.5f),
                                  glm::vec2(0.05f, 0.15f),
                                  0.0f,
                                  glm::vec3(0.25f, 0.25f, 0.25f),
                                  11,
                                  "",
                                  "fonts/Kenney Future Narrow.ttf",
                                  glm::vec3(0.0f, 0.0f, 0.0f));

  bodyParts["torso"]->anchorPoint = glm::vec2(0.5f, 0.5f);

  bodyParts["head"] = new Button(glm::vec2(0.5f, 0.415f),
                                 glm::vec2(0.025f, 0.05f),
                                 0.0f,
                                 glm::vec3(0.25f, 0.25f, 0.25f),
                                 11,
                                 "",
                                 "fonts/Kenney Future Narrow.ttf",
                                 glm::vec3(0.0f, 0.0f, 0.0f));

  bodyParts["head"]->anchorPoint = glm::vec2(0.5f, 1.0f);

  bodyParts["right forearm"] = new Button(glm::vec2(0.47f, 0.425f),
                                          glm::vec2(0.025f, 0.06f),
                                          0.0f,
                                          glm::vec3(0.25f, 0.25f, 0.25f),
                                          11,
                                          "",
                                          "fonts/Kenney Future Narrow.ttf",
                                          glm::vec3(0.0f, 0.0f, 0.0f));

  bodyParts["right forearm"]->anchorPoint = glm::vec2(1.0f, 0.0f);

  bodyParts["right arm"] = new Button(glm::vec2(0.47f, 0.495f),
                                      glm::vec2(0.025f, 0.06f),
                                      0.0f,
                                      glm::vec3(0.25f, 0.25f, 0.25f),
                                      11,
                                      "",
                                      "fonts/Kenney Future Narrow.ttf",
                                      glm::vec3(0.0f, 0.0f, 0.0f));

  bodyParts["right arm"]->anchorPoint = glm::vec2(1.0f, 0.0f);

  bodyParts["right hand"] = new Button(glm::vec2(0.47f, 0.565f),
                                       glm::vec2(0.025f, 0.02f),
                                       0.0f,
                                       glm::vec3(0.25f, 0.25f, 0.25f),
                                       11,
                                       "",
                                       "fonts/Kenney Future Narrow.ttf",
                                       glm::vec3(0.0f, 0.0f, 0.0f));

  bodyParts["right hand"]->anchorPoint = glm::vec2(1.0f, 0.0f);


  bodyParts["left forearm"] = new Button(glm::vec2(0.53f, 0.425f),
                                         glm::vec2(0.025f, 0.06f),
                                         0.0f,
                                         glm::vec3(0.25f, 0.25f, 0.25f),
                                         11,
                                         "",
                                         "fonts/Kenney Future Narrow.ttf",
                                         glm::vec3(0.0f, 0.0f, 0.0f));

  bodyParts["left arm"] = new Button(glm::vec2(0.53f, 0.495f),
                                     glm::vec2(0.025f, 0.06f),
                                     0.0f,
                                     glm::vec3(0.25f, 0.25f, 0.25f),
                                     11,
                                     "",
                                     "fonts/Kenney Future Narrow.ttf",
                                     glm::vec3(0.0f, 0.0f, 0.0f));

  bodyParts["left hand"] = new Button(glm::vec2(0.53f, 0.565f),
                                      glm::vec2(0.025f, 0.02f),
                                      0.0f,
                                      glm::vec3(0.25f, 0.25f, 0.25f),
                                      11,
                                      "",
                                      "fonts/Kenney Future Narrow.ttf",
                                      glm::vec3(0.0f, 0.0f, 0.0f));

  bodyParts["right thigh"] = new Button(glm::vec2(0.499f, 0.585f),
                                        glm::vec2(0.024f, 0.06f),
                                        0.0f,
                                        glm::vec3(0.25f, 0.25f, 0.25f),
                                        11,
                                        "",
                                        "fonts/Kenney Future Narrow.ttf",
                                        glm::vec3(0.0f, 0.0f, 0.0f));

  bodyParts["right thigh"]->anchorPoint = glm::vec2(1.0f, 0.0f);

  bodyParts["right calf"] = new Button(glm::vec2(0.499f, 0.655f),
                                       glm::vec2(0.024f, 0.06f),
                                       0.0f,
                                       glm::vec3(0.25f, 0.25f, 0.25f),
                                       11,
                                       "",
                                       "fonts/Kenney Future Narrow.ttf",
                                       glm::vec3(0.0f, 0.0f, 0.0f));

  bodyParts["right calf"]->anchorPoint = glm::vec2(1.0f, 0.0f);

  bodyParts["right foot"] = new Button(glm::vec2(0.499f, 0.725f),
                                       glm::vec2(0.024f, 0.02f),
                                       0.0f,
                                       glm::vec3(0.25f, 0.25f, 0.25f),
                                       11,
                                       "",
                                       "fonts/Kenney Future Narrow.ttf",
                                       glm::vec3(0.0f, 0.0f, 0.0f));

  bodyParts["right foot"]->anchorPoint = glm::vec2(1.0f, 0.0f);


  bodyParts["left thigh"] = new Button(glm::vec2(0.501f, 0.585f),
                                       glm::vec2(0.024f, 0.06f),
                                       0.0f,
                                       glm::vec3(0.25f, 0.25f, 0.25f),
                                       11,
                                       "",
                                       "fonts/Kenney Future Narrow.ttf",
                                       glm::vec3(0.0f, 0.0f, 0.0f));

  bodyParts["left calf"] = new Button(glm::vec2(0.501f, 0.655f),
                                      glm::vec2(0.024f, 0.06f),
                                      0.0f,
                                      glm::vec3(0.25f, 0.25f, 0.25f),
                                      11,
                                      "",
                                      "fonts/Kenney Future Narrow.ttf",
                                      glm::vec3(0.0f, 0.0f, 0.0f));

  bodyParts["left foot"] = new Button(glm::vec2(0.501f, 0.725f),
                                      glm::vec2(0.024f, 0.02f),
                                      0.0f,
                                      glm::vec3(0.25f, 0.25f, 0.25f),
                                      11,
                                      "",
                                      "fonts/Kenney Future Narrow.ttf",
                                      glm::vec3(0.0f, 0.0f, 0.0f));

  for (auto& [name, part] : bodyParts) {
    uiElements.push_back(part);
    bodyPartHealth[name] = 100.0f;

    part->setCallback([name]() {
      if (HealingItem::equippedHealingItem == nullptr) return;

      bodyPartHealth[name] = std::min(bodyPartHealth[name] + HealingItem::equippedHealingItem->healingAmount, 100.0f);
      HealingItem::equippedHealingItem->visible = false;
      HealingItem::equippedHealingItem = nullptr;
      Item::equippedItem = nullptr;
      updateEffects();
    });
  }

  ui = new Container(uiElements);
  ui->changeVisibility(false);
  ui->registerObjects();

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

float lerp(float a, float b, float t) {
  return a + t * (b - a);
}

void Health::update() {
  if (!UIElement::shouldDisplayMenus) {
    ui->changeVisibility(false);
    return;
  }

  if (glfwGetKey(Window::window, GLFW_KEY_Q) && !playingOpenAnim && !pressed && UIElement::shouldDisplayMenus) {
    pressed = true;

    if (background->visible) {
      ui->changeVisibility(false);
    } else {
      background->visible = true;
      background->size = glm::vec2(0.0f, 0.0f);
      playingOpenAnim = true;
      openAnimTime = 0.0f;
      lastCoutText = 0;
    }
  } else if (!glfwGetKey(Window::window, GLFW_KEY_Q)) {
    pressed = false;
  }

  for (auto& [name, part] : bodyParts) {
    part->color.x = lerp(0.25f, 1.0f, 1 - (bodyPartHealth[name] / 100.0f));
    part->color.y = lerp(0.25f, 0.0f, 1 - (bodyPartHealth[name] / 100.0f));
    part->color.z = lerp(0.25f, 0.0f, 1 - (bodyPartHealth[name] / 100.0f));
  }

  if (playingOpenAnim) {
    openAnimTime += Window::deltaTime;

    if (openAnimTime <= 0.5f) {
      background->size = glm::vec2(0.75f, 0.75f) * easeOutQuint(openAnimTime / 0.5f);
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
    } else {
      for (TextElement* text : cout) {
        text->pendDelete();
      }

      cout.clear();
      playingOpenAnim = false;
      ui->changeVisibility(true);
    }
  }
}

void Health::dealDmgToBodyPart(std::string bodyPart, float dmg, bool particles) {
  bodyPartHealth[bodyPart] -= dmg;
  if (particles) Particle::createParticles(Player::currentPlayer->position, glm::vec2(25.0f, 25.0f), 0.5f, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, -100.0f), 100.0f, 1.0f, 10);
  
  updateEffects();
}

void Health::setBodyPartHealth(std::string bodyPart, float health) {
  bodyPartHealth[bodyPart] = health;

  updateEffects();
}

void Health::fullyHeal() {
  for (auto& [bodyPart, health] : bodyPartHealth) {
    bodyPartHealth[bodyPart] = 100;
  }
}

void Health::updateEffects() {
  bool dead = false;
  bool blur = false;
  float speedMult = 1.0f;
  float inventoryMult = 1.0f;

  if (bodyPartHealth["head"] <= 0) {
    dead = true;
  } else if (bodyPartHealth["head"] <= 50) {
    blur = true;
  }

  std::vector<std::string> limbs;

  limbs.push_back("left");
  limbs.push_back("right");

  for (std::string limb : limbs) {
    if (bodyPartHealth[limb + " hand"] <= 50) {
      inventoryMult -= 0.5f;
    }

    if (bodyPartHealth[limb + " thigh"] <= 50) {
      speedMult -= 0.1f;
    }

    if (bodyPartHealth[limb + " calf"] <= 50) {
      speedMult -= 0.3f;
    }

    if (bodyPartHealth[limb + " foot"] <= 50) {
      speedMult -= 0.175f;
    }
  }

  if (dead) {
    Object::blurry = false;
    fullyHeal();
    DeathScreen::play();
    return;
  }

  Object::blurry = blur;
  Player::currentPlayer->speedMult = speedMult;
}
