#pragma once

#include "Object.h"
#include "TextElement.h"

class Item : public Object {
public:
  Item(glm::vec2 position, glm::vec2 size, float transparency, std::string texPath, int zIndex, std::string name);
  Item(glm::vec2 position, glm::vec2 size, float transparency, glm::vec3 color, int zIndex, std::string name);

  static void init();

  std::string name;
protected:
  void beforeUpdate() override;
private:
  void initObject();

  static TextElement* interactionElement;
};
