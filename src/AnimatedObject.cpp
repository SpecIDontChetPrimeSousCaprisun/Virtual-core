#include "AnimatedObject.h"
#include "Window.h"
#include "FileLoader.h"

#include <fstream>
#include <sstream>
#include <cmath>

AnimatedObject::AnimatedObject(glm::vec2 position, glm::vec2 size, float transparency, std::string animPath, int zIndex) 
  : Object(position, size, transparency, glm::vec3(0.0f, 0.0f, 0.0f), zIndex), time(0.0f), isPlaying(false) {
  std::ifstream infoFile(animPath + "/info.json");
  json info;

  infoFile >> info;

  looped = info.value("looped", false);
  framerate = info.value("framerate", 1.0f);

  std::string format = info.value("format", ".png");
  int textureAmount = info["frameAmount"];

  for (int i = 0; i < textureAmount; i++) {
    std::ostringstream ss;

    ss << animPath << "/frames/" << i << format;

    textures.push_back(FileLoader::loadTexture(ss.str()));
  }

  usesColor = false;
  texture = textures[0];
}

void AnimatedObject::afterTextureUpdate() {}

void AnimatedObject::beforeUpdate() {
  if (!isPlaying) {
    afterTextureUpdate();
    return;
  }

  time += Window::deltaTime;

  if ((int)std::floor(time / framerate) >= (int)textures.size()) {
    time = 0;

    if (!looped) {
      isPlaying = false;
      texture = textures[0];
    }
  }

  texture = textures[std::floor(time / framerate)];

  afterTextureUpdate();
}
