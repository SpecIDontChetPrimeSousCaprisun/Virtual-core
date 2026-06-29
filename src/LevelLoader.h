#pragma once

#include <vector>
#include <string>
#include <nlohmann/json.hpp>

#include "Object.h"

using json = nlohmann::json;

class LevelLoader {
public:
  static std::vector<Object*> loadLevel(std::string path);
private:
  static std::vector<Object*> loadProceduralLevel(std::string path);
  static std::vector<Object*> loadStaticLevel(std::string path);
  static Object* parseEntry(json entry);

  static std::vector<Object*> currentLevel;
};
