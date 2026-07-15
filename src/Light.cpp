#include "Light.h"

std::vector<Light*> Light::lights;

Light::Light(glm::vec2 position, glm::vec3 color, float radius, float intensity)
            : position(position), color(color), radius(radius), intensity(intensity) {
  lights.push_back(this);
}

Light::~Light() {
  for (auto it = lights.begin(); it != lights.end();) {
    if (*it == this) {
      it = lights.erase(it);
    } else {
      ++it;
    }
  }
}
