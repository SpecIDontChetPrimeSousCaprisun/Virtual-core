#pragma once

#include "Object.h"

#include <nlohmann/json.hpp>
#include <vector>

using json = nlohmann::json;

class AnimatedObject : public Object {
public:
  AnimatedObject(glm::vec2 position, glm::vec2 size, float transparency, std::string animPath, int zIndex);

  void changeAnimation(std::string animPath);

  bool isPlaying;
  bool looped;
protected:
  void beforeUpdate() override;
  virtual void afterTextureUpdate();
private:
  std::vector<unsigned int> textures;

  float time;
  float framerate;
};
