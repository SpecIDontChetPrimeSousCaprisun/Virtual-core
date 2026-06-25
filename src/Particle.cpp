#include "Particle.h"
#include "Window.h"

std::mt19937 Particle::rng(std::random_device{}());

Particle::Particle(glm::vec2 position, glm::vec2 size, float transparency, std::string texPath, 
                   glm::vec2 direction, float spread, float lifetime) 
  : Object(position, size, transparency, texPath, 999999), direction(direction), spread(spread), lifetime(lifetime) {
  anchored = false;

  std::uniform_real_distribution<float> dist(-spread, spread);

  linearVelocity = direction + glm::vec2(dist(rng), dist(rng));
}

Particle::Particle(glm::vec2 position, glm::vec2 size, float transparency, glm::vec3 color, 
                   glm::vec2 direction, float spread, float lifetime) 
  : Object(position, size, transparency, color, 999999), direction(direction), spread(spread), lifetime(lifetime) {
  anchored = false;

  std::uniform_real_distribution<float> dist(-spread, spread);
 
  linearVelocity = direction + glm::vec2(dist(rng), dist(rng));
}


void Particle::createParticles(glm::vec2 position, glm::vec2 size, float transparency, std::string texPath, 
           glm::vec2 direction, float spread, float lifetime, int amount) {
  for (int i = 0; i <= amount; i++) {
    Particle* particle = new Particle(position, size, transparency, texPath, direction, spread, lifetime);
    particle->registerObject();
  }
}

void Particle::createParticles(glm::vec2 position, glm::vec2 size, float transparency, glm::vec3 color, 
           glm::vec2 direction, float spread, float lifetime, int amount) {
  for (int i = 0; i <= amount; i++) { 
    Particle* particle = new Particle(position, size, transparency, color, direction, spread, lifetime);
    particle->registerObject();
  }
}

void Particle::afterUpdate() {
  lifetime -= Window::deltaTime;

  if (lifetime <= 0) {
    pendingDelete = true;
  }
}
