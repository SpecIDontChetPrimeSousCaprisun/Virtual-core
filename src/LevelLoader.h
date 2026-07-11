#pragma once

#include <vector>
#include <string>
#include <nlohmann/json.hpp>
#include <random>

#include "Object.h"

using json = nlohmann::json;

class LevelLoader {
public:
  static std::vector<Object*> loadLevel(std::string path);
  static std::vector<Object*> addLevel(std::string path);
  static void clearCurrentLevel();

  static std::vector<Object*> currentLevel;
private:
  static std::vector<Object*> loadProceduralLevel(std::string path, json);
  static std::vector<Object*> loadStaticLevel(json);
  static Object* parseEntry(json entry); 

  static std::mt19937 rng;
};
