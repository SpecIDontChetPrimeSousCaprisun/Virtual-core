#pragma once

#include "Particle.h"

class UIParticle : public Particle {
public:
  UIParticle(glm::vec2 position, glm::vec2 size, float transparency, std::string texPath, 
           glm::vec2 direction, float spread, float lifetime);
  UIParticle(glm::vec2 position, glm::vec2 size, float transparency, glm::vec3 color, 
           glm::vec2 direction, float spread, float lifetime);

  static void createParticles(glm::vec2 position, glm::vec2 size, float transparency, std::string texPath, 
           glm::vec2 direction, float spread, float lifetime, int amount);
  static void createParticles(glm::vec2 position, glm::vec2 size, float transparency, glm::vec3 color, 
           glm::vec2 direction, float spread, float lifetime, int amount);
protected:
  drawInfo* beforeDrawing() override;
  void beforeUpdate() override;
};
