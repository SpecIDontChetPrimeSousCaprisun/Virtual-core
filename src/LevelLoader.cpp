#include "LevelLoader.h"

#include "Object.h"
#include "Player.h"
#include "Item.h"
#include "KillBox.h"
#include "Enemy.h"
#include "Gun.h"

#include <fstream>

std::vector<Object*> LevelLoader::currentLevel;
std::mt19937 LevelLoader::rng(std::random_device{}());

std::vector<Object*> LevelLoader::loadLevel(std::string path) {
  clearCurrentLevel();
  return addLevel(path);
}

std::vector<Object*> LevelLoader::addLevel(std::string path) {
  std::ifstream dataFile(path + "/data.json");

  if (!dataFile.is_open()) {
    std::cerr << "Couldn't open path " + path + "/data.json\n";
    return currentLevel;
  }

  json data;
  dataFile >> data;

  std::vector<Object*> level;

  if (data["procedural"]) level = loadProceduralLevel(path, data);
  else {
    std::ifstream levelFile(path + "/level.json");

    if (!levelFile.is_open()) {
      std::cerr << "Couldn't open path " + path + "/level.json\n";
      return level;
    }

    json lvlData;
    levelFile >> lvlData;
    level = loadStaticLevel(lvlData);
  }

  for (Object* object : level) {
    object->registerObject();
  }

  currentLevel = level;
  return level;
}

void LevelLoader::clearCurrentLevel() {
  for (Object* object : currentLevel) {
    object->pendDelete();
  }

  currentLevel.clear();
}

std::vector<Object*> LevelLoader::loadProceduralLevel(std::string path, json data) {  
  std::vector<Object*> level;

  std::uniform_int_distribution<int> roomDist(1, data["poolSize"]);
  std::uniform_int_distribution<int> lenDist(data["minLength"], data["maxLength"]);
  int len = lenDist(rng);
  glm::vec2 pos = glm::vec2(0.0f, 0.0f);

  std::ifstream firstRoomFile(path + "/start.json");

  if (!firstRoomFile.is_open()) {
    std::cerr << "Couldn't open path " + path + "/start.json";
    return level;
  }

  json firstRoomData;
  firstRoomFile >> firstRoomData;

  std::vector<Object*> newObjects = loadStaticLevel(firstRoomData["objects"]);

  for (Object* obj : newObjects) {
    level.push_back(obj);
  }

  pos += glm::vec2(firstRoomData["endPos"][0], firstRoomData["endPos"][1]);

  for (int i = 0; i < len; i++) {
    int roomNum = roomDist(rng);

    std::ostringstream ss;
    ss << path << "/roomPool/" << roomNum << ".json";
    std::ifstream roomFile(ss.str());

    if (!roomFile.is_open()) {
      std::cerr << "Couldn't open path " + ss.str();
      continue;
    }

    json roomData;
    roomFile >> roomData;

    newObjects = loadStaticLevel(roomData["objects"]);

    for (Object* obj : newObjects) {
      obj->position += pos;
      level.push_back(obj);
    }

    pos += glm::vec2(roomData["endPos"][0], roomData["endPos"][1]);
  }

  return level;
}

std::vector<Object*> LevelLoader::loadStaticLevel(json data) { 
  std::vector<Object*> level;

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
  float parallax = entry.at("parallaxFactor").get<float>();

  Object* obj = nullptr;

  if (type == "Object") {
    obj = new Object(position, size, transparency, texture, zIndex);
  } else if (type == "Player") {
    obj = new Player(position, size, transparency, texture, zIndex,
                     entry.at("isCurrentPlayer").get<bool>());
  } else if (type == "Item") {
    obj = new Item(position, size, transparency, texture, zIndex,
                   entry.at("name").get<std::string>());
  } else if (type == "Gun") {
    obj = new Gun(position, size, transparency, texture, zIndex,
                   entry.at("name").get<std::string>());
  } else if (type == "KillBox" || type == "Killbox") {
    obj = new KillBox(position, size, zIndex);
  } else if (type == "Enemy") {
    obj = new Enemy(position, size, texture, zIndex);
  } else {
    std::cerr << "Error in loading level : " + type + " is not a valid object type.\n";
  }

  if (!obj) return nullptr;

  obj->canCollide = canCollide;
  obj->anchored = anchored;
  obj->parallaxFactor = parallax;

  return obj;
}
