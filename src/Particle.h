#pragma once

#include "Object.h"
#include <random>

class Particle : public Object {
public:
  Particle(glm::vec2 position, glm::vec2 size, float transparency, std::string texPath, 
           glm::vec2 direction, float spread, float lifetime);
  Particle(glm::vec2 position, glm::vec2 size, float transparency, glm::vec3 color, 
           glm::vec2 direction, float spread, float lifetime);


  static void createParticles(glm::vec2 position, glm::vec2 size, float transparency, std::string texPath, 
           glm::vec2 direction, float spread, float lifetime, int amount);
  static void createParticles(glm::vec2 position, glm::vec2 size, float transparency, glm::vec3 color, 
           glm::vec2 direction, float spread, float lifetime, int amount);
protected:
  void afterUpdate() override;
private:
  static std::mt19937 rng;

  glm::vec2 direction;
  float spread, lifetime;
};
