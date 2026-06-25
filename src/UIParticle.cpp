#include "UIParticle.h"
#include "Window.h"

UIParticle::UIParticle(glm::vec2 position, glm::vec2 size, float transparency, std::string texPath, 
                   glm::vec2 direction, float spread, float lifetime) 
  : Particle(position, size, transparency, texPath, direction, spread, lifetime) {}

UIParticle::UIParticle(glm::vec2 position, glm::vec2 size, float transparency, glm::vec3 color, 
                   glm::vec2 direction, float spread, float lifetime) 
  : Particle(position, size, transparency, color, direction, spread, lifetime) {
  anchored = true;
}


void UIParticle::createParticles(glm::vec2 position, glm::vec2 size, float transparency, std::string texPath, 
           glm::vec2 direction, float spread, float lifetime, int amount) {
  for (int i = 0; i <= amount; i++) {
    UIParticle* particle = new UIParticle(position, size, transparency, texPath, direction, spread, lifetime);
    particle->registerObject();
  }
}

void UIParticle::createParticles(glm::vec2 position, glm::vec2 size, float transparency, glm::vec3 color, 
           glm::vec2 direction, float spread, float lifetime, int amount) {
  for (int i = 0; i <= amount; i++) { 
    UIParticle* particle = new UIParticle(position, size, transparency, color, direction, spread, lifetime);
    particle->registerObject();
  }
}

drawInfo* UIParticle::beforeDrawing() {
  glm::vec2 transformedPos = position - (size * glm::vec2(0.5f, 0.5f));

  glm::vec2 pixelPos(transformedPos.x * Window::fbWidth, transformedPos.y * Window::fbHeight);
  glm::vec2 pixelSize(size.x * Window::fbWidth, size.y * Window::fbHeight);

  return new drawInfo(pixelPos, pixelSize);
}

void UIParticle::beforeUpdate() {
  linearVelocity += glm::vec2(0.0f, 0.1f * Window::deltaTime);

  position.x += linearVelocity.x * Window::deltaTime;
  position.y += linearVelocity.y * Window::deltaTime;
}
