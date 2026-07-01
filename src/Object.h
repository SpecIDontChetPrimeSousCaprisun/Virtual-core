#pragma once

#include <map>
#include <iostream>
#include <vector>
#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>

struct drawInfo {
public:
  drawInfo(glm::vec2 position, glm::vec2 size);

  glm::vec2 position, size;
  std::string text;
  glm::vec3 textColor;
  bool shouldDraw = true;
};

class Object {
public:
  Object(const Object&) = delete;
  Object& operator=(const Object&) = delete;

  Object(glm::vec2 position, glm::vec2 size, float transparency, std::string texPath, int zIndex);
  Object(glm::vec2 position, glm::vec2 size, float transparency, glm::vec3 color, int zIndex);
  virtual ~Object();

  static bool blurry;

  static void initShader();
  static void recreateBlurTexture();
  static void updateAll();
  static void drawAll();
  static void registerAll();
  static Object* raycast(
                          glm::vec2 origin,
                          const glm::vec2& direction,
                          glm::vec2& hitPoint,
                          float& tHit,
                          const std::vector<Object*>& ignore
                        );

  void registerObject();
  void pendDelete();
  bool isDeleted();

  glm::vec2 position, size, linearVelocity;
  glm::vec3 color;
  float transparency, rotation, cornerRadius, parallaxFactor;
  bool visible, anchored, canCollide;
  int zIndex;
  std::string texPath;
protected:
  virtual drawInfo* beforeDrawing();
  virtual void afterDrawing(drawInfo* info);
  virtual void beforeUpdate();
  virtual void afterUpdate();

  std::vector<Object*> lastCollides;
  glm::vec3 colorChange;
  glm::vec2 lastCorrection;
  float gravity;
  bool pendingDelete;
private:
  static std::vector<Object*> registerQueue;
  static std::map<int, std::vector<Object*>> objects;
  static unsigned int shaderProgram, blurShaderProgram, verticalBlurShaderProgram, sceneFBO, blurVAO, blurVBO, sceneTexture; 

  static void deletePendingObjects();

  unsigned int VAO, VBO, texture; 
  bool usesColor;

  void init();
  void update();
  void draw(); 
};
