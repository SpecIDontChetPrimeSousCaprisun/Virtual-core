#include "TextElement.h"
#include "Window.h"
#include "FileLoader.h"

unsigned int TextElement::shaderProgram = 0;

void TextElement::initShader() {
  // already initialized
  if (shaderProgram != 0)
      return;

  std::string vertexCode =
      FileLoader::loadFile("shaders/textVertex.glsl");

  std::string fragmentCode =
      FileLoader::loadFile("shaders/textFragment.glsl");

  const char* vertexShaderSource = vertexCode.c_str();
  const char* fragmentShaderSource = fragmentCode.c_str();

  // ===== VERTEX SHADER =====
  unsigned int vertexShader =
      glCreateShader(GL_VERTEX_SHADER);

  glShaderSource(
      vertexShader,
      1,
      &vertexShaderSource,
      NULL
  );

  glCompileShader(vertexShader);

  int success;
  char infoLog[512];

  glGetShaderiv(
      vertexShader,
      GL_COMPILE_STATUS,
      &success
  );

  if (!success) {
      glGetShaderInfoLog(
          vertexShader,
          512,
          NULL,
          infoLog
      );

      std::cout
          << "VERTEX SHADER ERROR:\n"
          << infoLog
          << std::endl;
  }

  // ===== FRAGMENT SHADER =====
  unsigned int fragmentShader =
      glCreateShader(GL_FRAGMENT_SHADER);

  glShaderSource(
      fragmentShader,
      1,
      &fragmentShaderSource,
      NULL
  );

  glCompileShader(fragmentShader);

  glGetShaderiv(
      fragmentShader,
      GL_COMPILE_STATUS,
      &success
  );

  if (!success) {
      glGetShaderInfoLog(
          fragmentShader,
          512,
          NULL,
          infoLog
      );

      std::cout
          << "FRAGMENT SHADER ERROR:\n"
          << infoLog
          << std::endl;
  }

  // ===== SHADER PROGRAM =====
  shaderProgram = glCreateProgram();

  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);

  glLinkProgram(shaderProgram);

  glGetProgramiv(
      shaderProgram,
      GL_LINK_STATUS,
      &success
  );

  if (!success) {
      glGetProgramInfoLog(
          shaderProgram,
          512,
          NULL,
          infoLog
      );

      std::cout
          << "PROGRAM LINK ERROR:\n"
          << infoLog
          << std::endl;
  }

  // cleanup
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  std::cout << "Text shader initialized\n";
}

TextElement::TextElement(glm::vec2 position, glm::vec2 size, float transparency, std::string texPath, int zIndex, std::string text, std::string fontPath, glm::vec3 textColor)
  : UIElement(position, size, transparency, texPath, zIndex), text(text), font(Font::getFont(fontPath, size.y * Window::fbHeight)), textColor(textColor), textCentered(true), textWidth(0.0f), textTransparency(0.0f) {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);

  // position
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // uv
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);
}

TextElement::TextElement(glm::vec2 position, glm::vec2 size, float transparency, glm::vec3 color, int zIndex, std::string text, std::string fontPath, glm::vec3 textColor)
  : UIElement(position, size, transparency, color, zIndex), text(text), font(Font::getFont(fontPath, size.y * Window::fbHeight)), textColor(textColor), textCentered(true), textWidth(0.0f), textTransparency(0.0f) {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);

  // position
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // uv
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);
}

drawInfo* TextElement::baseTextBeforeDrawing() {
  drawInfo* info = baseBeforeDrawing();

  info->text = text;
  info->textColor = textColor;

  return info;
}

drawInfo* TextElement::beforeDrawing() {
  return baseTextBeforeDrawing();
}

void TextElement::afterDrawing(drawInfo* info) {
  glUseProgram(shaderProgram);

  glm::mat4 projection = glm::ortho(
      0.0f, (float)Window::fbWidth,
      (float)Window::fbHeight, 0.0f,
      -1.0f, 1.0f
  );

  glUniformMatrix4fv(
      glGetUniformLocation(shaderProgram, "projection"),
      1,
      GL_FALSE,
      glm::value_ptr(projection)
  );

  glUniform3f(
      glGetUniformLocation(shaderProgram, "color"),
      info->textColor.r, info->textColor.g, info->textColor.b
  );

  glUniform3f(
      glGetUniformLocation(shaderProgram, "colorChange"),
      colorChange.r, colorChange.g, colorChange.b
  );

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, font->texture);

  glUniform1i(
      glGetUniformLocation(shaderProgram, "tex"),
      0
  );

  glUniform1f(
      glGetUniformLocation(shaderProgram, "transparency"),
      textTransparency
  );

  glUniform1f(
    glGetUniformLocation(shaderProgram, "rotation"),
    rotation
  );

  glm::vec2 pivot(
    info->position.x + info->size.x * 0.5f,
    info->position.y + info->size.y * 0.5f
  );

  glUniform2f(
    glGetUniformLocation(shaderProgram, "pivot"),
    pivot.x,
    pivot.y
  );

  glBindVertexArray(VAO);

  float startX = info->position.x;

  if (textCentered) {
    startX += info->size.x / 2;
    startX -= textWidth / 2;
  }

  float x = startX;
  float y = info->position.y + (info->size.y / 2) + (font->height / 4);

  for (unsigned char c : info->text) {
    if (c < 32 || c >= 128)
        continue;

    int glyphIndex = c - 32;

    if (glyphIndex < 0 || glyphIndex >= 96)
        continue;

    stbtt_aligned_quad q;

    stbtt_GetBakedQuad(
        font->cdata,
        512, 512,
        glyphIndex,
        &x, &y,
        &q,
        1
    );

    float vertices[6][4] = {
        { q.x0, q.y0, q.s0, q.t0 },
        { q.x1, q.y0, q.s1, q.t0 },
        { q.x1, q.y1, q.s1, q.t1 },

        { q.x1, q.y1, q.s1, q.t1 },
        { q.x0, q.y1, q.s0, q.t1 },
        { q.x0, q.y0, q.s0, q.t0 }
    };

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

    glDrawArrays(GL_TRIANGLES, 0, 6);
  }
  
  textWidth = x - startX;

  glBindVertexArray(0);
}

void TextElement::recalculateTextWidth() {
  drawInfo* info = beforeDrawing();

  float startX = info->position.x;

  if (textCentered) {
    startX += info->size.x / 2;
    startX -= textWidth / 2;
  }

  float x = startX;
  float y = info->position.y + (info->size.y / 2) + (font->height / 4);

  for (unsigned char c : text) {
    if (c < 32 || c >= 128)
        continue;

    int glyphIndex = c - 32;

    if (glyphIndex < 0 || glyphIndex >= 96)
        continue;

    stbtt_aligned_quad q;

    stbtt_GetBakedQuad(
        font->cdata,
        512, 512,
        glyphIndex,
        &x, &y,
        &q,
        1
    );

    float vertices[6][4] = {
        { q.x0, q.y0, q.s0, q.t0 },
        { q.x1, q.y0, q.s1, q.t0 },
        { q.x1, q.y1, q.s1, q.t1 },

        { q.x1, q.y1, q.s1, q.t1 },
        { q.x0, q.y1, q.s0, q.t1 },
        { q.x0, q.y0, q.s0, q.t0 }
    };
  }
  
  textWidth = x - startX;
}

void TextElement::reloadFont(std::string fontPath) {
  font = Font::getFont(fontPath, size.y * Window::fbHeight);
}
