#pragma once

#include "UIElement.h"
#include "Font.h"

#include <vector>

class TextElement : public UIElement {
public:
  TextElement(glm::vec2 position, glm::vec2 size, float transparency, std::string texPath, int zIndex,
              std::string text, std::string fontPath, glm::vec3 textColor);
  TextElement(glm::vec2 position, glm::vec2 size, float transparency, glm::vec3 color, int zIndex,
              std::string text, std::string fontPath, glm::vec3 textColor);

  virtual ~TextElement();

  static void initShader();
  static void reloadAllFonts();

  void recalculateTextWidth();
  void reloadFont(std::string fontPath);

  std::string text;
  Font* font;
  glm::vec3 textColor;
  float textWidth, textTransparency;
  bool textCentered;
protected:
  drawInfo* baseTextBeforeDrawing();
  drawInfo* beforeDrawing() override;
  void afterDrawing(drawInfo* info) override;
private:
  static unsigned int shaderProgram;
  static std::vector<TextElement*> elements;

  unsigned int VAO, VBO;
  std::string fontPath;
};
