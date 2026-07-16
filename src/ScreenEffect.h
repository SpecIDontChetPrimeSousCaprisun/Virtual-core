#pragma once

#include "Object.h"

class ScreenEffect : public Object {
public:
  ScreenEffect(glm::vec3 color, float transparency);
  ScreenEffect(std::string texPath, float transparency);

  static void init();
protected:
  drawInfo* beforeDrawing() override;
  void afterDrawing(drawInfo* info) override;
private:
  static unsigned int shaderProgram;
};
