#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

class Light {
public:
  Light(glm::vec2 position, glm::vec3 color, float radius, float intensity);
  virtual ~Light();

  static std::vector<Light*> lights;

  glm::vec2 position;
  glm::vec3 color;
  float radius;
  float intensity;
};
