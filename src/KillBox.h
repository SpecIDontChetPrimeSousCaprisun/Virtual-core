#pragma once

#include "Object.h"

class KillBox : public Object {
public:
  KillBox(glm::vec2 position, glm::vec2 size, int zIndex);
protected:
  void afterUpdate() override;
};
