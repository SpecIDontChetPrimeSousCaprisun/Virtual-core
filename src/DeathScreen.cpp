#include "DeathScreen.h"
#include "LevelLoader.h"
#include "Window.h"
#include "Intro.h"

std::vector<TextElement*> DeathScreen::cout;
std::vector<std::string> DeathScreen::coutText;
UIElement* DeathScreen::background;
bool DeathScreen::isPlaying = false;
float DeathScreen::time = 0.0f;
float DeathScreen::lastCoutText;

void DeathScreen::init() {
  coutText.push_back("[ critical ] Subject stopped responding");
  coutText.push_back("[ failed ] Running conciousness test…");
  coutText.push_back("[ failed ] Getting heart state…");
  coutText.push_back("[ ok ] Getting brain state…");
  coutText.push_back("[ warning ] No brain blood supply…");
  coutText.push_back("[ failed ] Running defibrilator…");
  coutText.push_back("[ failed ] Trying emergency supply…");
  coutText.push_back("[ failed ] Trying last medical resort…");
  coutText.push_back("You were our only hope…");
  coutText.push_back("It’s not your fault it’s their’s");
  coutText.push_back("Vengance is soon to come");
  coutText.push_back("[ ok ] Initiating self destruction….");

  background = new UIElement(glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), 0.0f, glm::vec3(0.0f, 0.0f, 0.0f), 999);
  background->visible = false;
  background->registerObject();
}

void DeathScreen::play() {
  UIElement::shouldDisplayMenus = false;
  UIElement::menuOpened = false;
  background->visible = true;
  LevelLoader::clearCurrentLevel();
  isPlaying = true;
  time = 0.0f;
  lastCoutText = 0.0f;
}

void DeathScreen::update() {
  if (isPlaying) {
    time += Window::deltaTime;
  
    if ((std::floor(time * 10.0f)) > lastCoutText && lastCoutText < coutText.size()) {
      TextElement* text = new TextElement(glm::vec2(0.0f, 0.02f * lastCoutText),
                                          glm::vec2(1.0f, 0.02f),
                                          1.0f,
                                          glm::vec3(0.0f, 0.0f, 0.0f),
                                          1000,
                                          coutText[lastCoutText],
                                          "fonts/JetBrainsMonoNerdFont-Regular.ttf",
                                          glm::vec3(1.0f, 1.0f, 1.0f));

      text->textCentered = false;
      text->registerObject();

      cout.push_back(text);

      lastCoutText++;
    } else if (lastCoutText >= coutText.size()) {
      isPlaying = false;
      Intro::open();
      background->visible = false;
      for (TextElement* text : cout) {
        text->pendDelete();
      }
      cout.clear();
    }
  }
}
