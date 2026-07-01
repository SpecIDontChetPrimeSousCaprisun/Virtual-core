#include "DeathScreen.h"
#include "LevelLoader.h"

std::vector<TextElement*> DeathScreen::cout;
std::vector<std::string> DeathScreen::coutText;
UIElement* DeathScreen::background;

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
}

void DeathScreen::play() {
  background->visible = true;
  LevelLoader::clearCurrentLevel();
}

void DeathScreen::update() {

}
