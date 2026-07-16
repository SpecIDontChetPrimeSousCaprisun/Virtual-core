#include "ScreenEffect.h"
#include "FileLoader.h"
#include "Window.h"

unsigned int ScreenEffect::shaderProgram = 0;

ScreenEffect::ScreenEffect(glm::vec3 color, float transparency)
  : Object(glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), transparency, color, 9999999) {}

ScreenEffect::ScreenEffect(std::string texPath, float transparency)
  : Object(glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), transparency, texPath, 9999999) {}

void ScreenEffect::init() {
  if (shaderProgram != 0) return; // Avoid reinitialising
  
  std::string vertexCode = FileLoader::loadFile("shaders/2DVertex.glsl");
  std::string fragCode = FileLoader::loadFile("shaders/ScreenEffectFrag.glsl");

  const char* vertexSource = vertexCode.c_str();
  const char* fragSource = fragCode.c_str();

  // ===== VERTEX SHADER =====
  unsigned int vertexShader =
      glCreateShader(GL_VERTEX_SHADER);

  glShaderSource(
      vertexShader,
      1,
      &vertexSource,
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
      &fragSource,
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
  
  std::cout << "Screen effect shader initialized\n";
}

drawInfo* ScreenEffect::beforeDrawing() {
  glUseProgram(shaderProgram);
  glUniform1f(
      glGetUniformLocation(shaderProgram, "alpha"),
      1 - transparency
  );

  glUniform3f(
      glGetUniformLocation(shaderProgram, "colorChange"),
      colorChange.x, colorChange.y, colorChange.z
  );

  glUniform1i(
      glGetUniformLocation(shaderProgram, "useColor"),
      usesColor ? 1 : 0
  );

  glUniform3f(
      glGetUniformLocation(shaderProgram, "color"),
      color.x, color.y, color.z
  );

  return new drawInfo(glm::vec2(0.0f, 0.0f), glm::vec2(Window::fbWidth, Window::fbHeight));
}

void ScreenEffect::afterDrawing(drawInfo* info) {
  glUseProgram(Object::shaderProgram);
}
