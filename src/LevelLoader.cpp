#include "LevelLoader.h"

#include "Object.h"
#include "Player.h"

#include <fstream>

std::vector<Object*> LevelLoader::currentLevel;

std::vector<Object*> LevelLoader::loadLevel(std::string path) {
  std::ifstream dataFile(path + "/data.json");

  if (!dataFile.is_open()) {
    std::cerr << "Couldn't open path " + path + "/data.json\n";
    return currentLevel;
  }

  for (Object* object : currentLevel) {
    object->pendDelete();
  }

  currentLevel.clear();

  json data;
  dataFile >> data;

  std::vector<Object*> level;

  if (data["procedural"]) level = loadProceduralLevel(path);
  else level = loadStaticLevel(path);

  for (Object* object : level) {
    object->registerObject();
  }

  currentLevel = level;
  return level;
}

std::vector<Object*> LevelLoader::loadProceduralLevel(std::string path) {
  std::vector<Object*> level;
  return level;
}

std::vector<Object*> LevelLoader::loadStaticLevel(std::string path) {
  std::vector<Object*> level;

  std::ifstream levelFile(path + "/level.json");

  if (!levelFile.is_open()) {
    std::cerr << "Couldn't open path " + path + "/level.json\n";
    return level;
  }

  json data;
  levelFile >> data;

  for (auto& entry : data) {
    Object* obj = parseEntry(entry);
    if (obj) level.push_back(obj);
  }

  return level;
}

Object* LevelLoader::parseEntry(json entry) {
  std::string type = entry.at("type").get<std::string>();
  glm::vec2 position = glm::vec2(entry["position"][0], entry["position"][1]);
  glm::vec2 size = glm::vec2(entry["size"][0], entry["size"][1]);
  float transparency = entry.value("transparency", 0.0f);
  std::string texture = entry.at("texture").get<std::string>();
  int zIndex = entry.at("zIndex").get<int>();
  bool canCollide = entry.at("canCollide").get<bool>();
  bool anchored = entry.at("anchored").get<bool>();

  Object* obj;

  if (type == "Object") {
    obj = new Object(position, size, transparency, texture, zIndex);
  } else if (type == "Player") {
    obj = new Player(position, size, transparency, texture, zIndex,
                     entry.at("isCurrentPlayer").get<bool>());
  } else {
    std::cerr << "Error in loading level : " + type + "is not a valid object type.\n";
  }

  if (!obj) return nullptr;

  obj->canCollide = canCollide;
  obj->anchored = anchored;

  return obj;
}
