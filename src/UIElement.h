#pragma once

#include "Object.h"

class UIElement : public Object {
public:
  UIElement(glm::vec2 position, glm::vec2 size, float transparency, std::string texPath, int zIndex);
  UIElement(glm::vec2 position, glm::vec2 size, float transparency, glm::vec3 color, int zIndex);

  static bool shouldDisplayMenus;

  glm::vec2 getActualPosition();
  glm::vec2 getActualSize();

  glm::vec2 anchorPoint;
protected:
  virtual drawInfo* beforeDrawing() override;
  drawInfo* baseBeforeDrawing(); 
};
