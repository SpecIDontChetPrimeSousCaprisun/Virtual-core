#pragma once

#include "TextElement.h"
#include <vector>

class Textbox : public TextElement {
public:
  Textbox(glm::vec2 position, glm::vec2 size, float transparency, std::string texPath, int zIndex,
          std::string fontPath, glm::vec3 textColor);
  Textbox(glm::vec2 position, glm::vec2 size, float transparency, glm::vec3 color, int zIndex,
          std::string fontPath, glm::vec3 textColor);

  static void init();

  void setFocused(bool focused);
  bool isFocused();

  std::string placeholder;
  glm::vec3 placeholderColor;
protected:
  drawInfo* beforeDrawing(drawInfo* info) override;
  void beforeUpdate() override;
private:
  static std::vector<Textbox*> textboxes;
 
  bool focused, changedFocus, textAnim;
  float textAnimCd;

  void objectInit();
};
