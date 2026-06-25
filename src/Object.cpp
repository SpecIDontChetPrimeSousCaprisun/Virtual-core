#include "Object.h"
#include "FileLoader.h"
#include "Window.h"
#include "Player.h"
#include <cmath>

unsigned int Object::shaderProgram = 0;
std::vector<Object*> Object::registerQueue;
std::map<int, std::vector<Object*>> Object::objects;

drawInfo::drawInfo(glm::vec2 position, glm::vec2 size)
  : position(position), size(size) {}

void Object::deletePendingObjects() {
  for (auto& [zIndex, objectsVector] : objects) {
    for (auto it = objectsVector.begin(); it != objectsVector.end(); ) {
      Object* object = *it;

      if (object->pendingDelete) {
        it = objectsVector.erase(it);
        delete object;
      }
      else {
        ++it;
      }
    }
  }
}

void Object::drawAll() {
  for (auto& [zIndex, objectsVector] : objects) {
    for (Object* object : objectsVector) {
      object->draw();
    }
  }

  deletePendingObjects();
}

void Object::updateAll() {
  for (auto& [zIndex, objectsVector] : objects) {
    for (Object* object : objectsVector) {
      object->update();
    }
  }

  deletePendingObjects();
}

void Object::registerAll() {
  for (Object* object : registerQueue) {
    objects[object->zIndex].push_back(object);
  }

  registerQueue.clear();
}

void Object::initShader() {
  // already initialized
  if (shaderProgram != 0)
      return;

  std::string vertexCode =
      FileLoader::loadFile("shaders/2DVertex.glsl");

  std::string fragmentCode =
      FileLoader::loadFile("shaders/2DFragment.glsl");

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

  std::cout << "2D shader initialized\n";
}

void Object::init() { 
  float vertices[] = {
      // positions   // UVs
      0.0f, 0.0f,    0.0f, 0.0f,
      1.0f, 0.0f,    1.0f, 0.0f,
      1.0f, 1.0f,    1.0f, 1.0f,

      1.0f, 1.0f,    1.0f, 1.0f,
      0.0f, 1.0f,    0.0f, 1.0f,
      0.0f, 0.0f,    0.0f, 0.0f
  };

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(
      GL_ARRAY_BUFFER,
      sizeof(vertices),
      vertices,
      GL_STATIC_DRAW
  );

  // position
  glVertexAttribPointer(
      0,
      2,
      GL_FLOAT,
      GL_FALSE,
      4 * sizeof(float),
      (void*)0
  );
  glEnableVertexAttribArray(0);

  // UV
  glVertexAttribPointer(
      1,
      2,
      GL_FLOAT,
      GL_FALSE,
      4 * sizeof(float),
      (void*)(2 * sizeof(float))
  );
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  colorChange = glm::vec3(0.0f, 0.0f, 0.0f);
  lastCorrection = glm::vec2(0.0f, 0.0f);
  gravity = 500.0f;
  cornerRadius = 0.0f;
  parallaxFactor = 1.0f;
}

void Object::registerObject() {
  registerQueue.push_back(this);
}

Object::Object(glm::vec2 position, glm::vec2 size, float transparency, std::string texPath, int zIndex) 
  : position(position), size(size), linearVelocity(glm::vec2(0.0f, 0.0f)), transparency(transparency), texture(FileLoader::loadTexture(texPath)), zIndex(zIndex), visible(true), anchored(true), canCollide(false), rotation(0), pendingDelete(false), usesColor(false) {
    init();
}

Object::Object(glm::vec2 position, glm::vec2 size, float transparency, glm::vec3 color, int zIndex) 
  : position(position), size(size), linearVelocity(glm::vec2(0.0f, 0.0f)), transparency(transparency), color(color), zIndex(zIndex), visible(true), anchored(true), canCollide(false), rotation(0), pendingDelete(false), usesColor(true) {
    init();
}

Object::~Object() {
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);

  if (!usesColor) {
    glDeleteTextures(1, &texture);
  }
}

drawInfo* Object::beforeDrawing() {
  drawInfo* info = new drawInfo(position, size);

  if (Player::currentPlayer == nullptr) {
    info->shouldDraw = false;
    return info;
  }

  glm::vec2 visiblePosition = position;

  visiblePosition -= ((Player::currentPlayer->position / parallaxFactor) - (glm::vec2(Window::fbWidth, Window::fbHeight) / 2.0f)) + (Player::currentPlayer->size / 2.0f);

  info->position = visiblePosition;

  return info;
}

void Object::afterDrawing(drawInfo* info) {}

void Object::draw() {
  if (!visible) return;
  
  glUseProgram(shaderProgram);

  // ===== MODEL =====
  glm::mat4 model = glm::mat4(1.0f);

  drawInfo* newInfo = beforeDrawing();

  if (!newInfo->shouldDraw) return;

  model = glm::translate(
      model,
      glm::vec3(newInfo->position, 0.0f)
  );

  // rotate around center
  model = glm::translate(
      model,
      glm::vec3(newInfo->size * 0.5f, 0.0f)
  );

  model = glm::rotate(
      model,
      glm::radians(rotation),
      glm::vec3(0.0f, 0.0f, 1.0f)
  );

  model = glm::translate(
      model,
      glm::vec3(-newInfo->size * 0.5f, 0.0f)
  );

  model = glm::scale(
      model,
      glm::vec3(newInfo->size, 1.0f)
  );

  // ===== ORTHOGRAPHIC PROJECTION =====
  glm::mat4 projection = glm::ortho(
      0.0f,
      (float)Window::fbWidth,
      (float)Window::fbHeight,
      0.0f,
      -1.0f,
      1.0f
  );

  // ===== SEND MATRICES =====
  glUniformMatrix4fv(
      glGetUniformLocation(shaderProgram, "model"),
      1,
      GL_FALSE,
      glm::value_ptr(model)
  );

  glUniformMatrix4fv(
      glGetUniformLocation(shaderProgram, "projection"),
      1,
      GL_FALSE,
      glm::value_ptr(projection)
  );

  // ===== COLOR =====
  glUniform2f(
    glGetUniformLocation(shaderProgram, "objectSize"),
    size.x, size.y
  );


  glUniform1f(
    glGetUniformLocation(shaderProgram, "cornerRadius"),
    cornerRadius
  );

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

  // ===== TEXTURE =====
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);

  glUniform1i(
    glGetUniformLocation(shaderProgram, "tex"),
    0
  );

  // ===== DRAW =====
  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLES, 0, 6);

  glBindVertexArray(0);

  afterDrawing(newInfo);

  delete newInfo;
}

void Object::beforeUpdate() {}
void Object::afterUpdate() {}

void Object::update() {
  beforeUpdate();

  if (!anchored) {
    glm::vec2 hitPoint;
    float tHit;
    std::vector<Object*> ignore;

    ignore.push_back(this);

    Object* result = raycast(
      position + glm::vec2(size.x * 0.5f, size.y),
      glm::vec2(0.0f, 0.5f),
      hitPoint,
      tHit,
      ignore
    );

    Object* resultR = raycast(
      position + glm::vec2(size.x - 1.0f, size.y),
      glm::vec2(0.0f, 0.5f),
      hitPoint,
      tHit,
      ignore
    );

    Object* resultL = raycast(
      position + glm::vec2(1.0f, size.y),
      glm::vec2(0.0f, 0.5f),
      hitPoint,
      tHit,
      ignore
    );

    if (!result && !resultR & !resultL) {
      linearVelocity += glm::vec2(0.0f, gravity * (float)Window::deltaTime);
    }

    position += glm::vec2(linearVelocity.x * Window::deltaTime, linearVelocity.y * Window::deltaTime);

    if (canCollide) {
      for (auto& [zIndex, objectsVector] : objects) {
        for (Object* object : objectsVector) {
          if (object == this) continue;
          if (!object->canCollide) continue;
          float aRadians = rotation * glm::pi<float>() / 180.0f;

          glm::vec2 Ax(
            std::cos(aRadians),
            std::sin(aRadians)
          );

          glm::vec2 Ay(
            -std::sin(aRadians),
            std::cos(aRadians)
          );

          float bRadians = object->rotation * glm::pi<float>() / 180.0f;

          glm::vec2 Bx(
            std::cos(bRadians),
            std::sin(bRadians)
          );

          glm::vec2 By(
            -std::sin(bRadians),
            std::cos(bRadians)
          );
 
          glm::vec2 WH(size.x / 2, size.y / 2);
          glm::vec2 WHb(object->size.x / 2, object->size.y / 2);
          glm::vec2 T = (position + WH) - (object->position + WHb);

          auto getOverlap = [&](glm::vec2 axis) -> float {
            float ra =
                WH.x * std::abs(glm::dot(Ax, axis)) +
                WH.y * std::abs(glm::dot(Ay, axis));

            float rb =
                WHb.x * std::abs(glm::dot(Bx, axis)) +
                WHb.y * std::abs(glm::dot(By, axis));

            float distance = std::abs(glm::dot(T, axis));

            return (ra + rb) - distance;
          };

          float o1 = getOverlap(Ax);
          float o2 = getOverlap(Ay);
          float o3 = getOverlap(Bx);
          float o4 = getOverlap(By);

          float minOverlap = o1;
          glm::vec2 bestAxis = Ax;

          if (o2 < minOverlap) { minOverlap = o2; bestAxis = Ay; }
          if (o3 < minOverlap) { minOverlap = o3; bestAxis = Bx; }
          if (o4 < minOverlap) { minOverlap = o4; bestAxis = By; }
          
          if (!(o1 <= 0 || o2 <= 0 || o3 <= 0 || o4 <= 0)) {
            glm::vec2 correction = bestAxis * minOverlap;
            glm::vec2 centerA = position + WH;
            glm::vec2 centerB = object->position + WHb;

            glm::vec2 dir = centerA - centerB;

            if (glm::dot(dir, bestAxis) < 0.0f)
                correction = -correction;

            if (object->anchored) {
              position += correction;
              lastCorrection = correction;
            } else {
              position += correction * 0.5f;
              object->position -= correction * 0.5f;
              lastCorrection = correction * 0.5f;
              object->lastCorrection = correction * 0.5f;
            }

            float vn = glm::dot(linearVelocity, bestAxis);

            if (vn < 0.0f)
                linearVelocity -= vn * bestAxis;

            float ovn = glm::dot(object->linearVelocity, bestAxis);

            if (ovn < 0.0f)
                object->linearVelocity -= ovn * bestAxis;
          } 
        }
      }
    }
  }

  afterUpdate();
}

Object* Object::raycast(
    glm::vec2 rayOrigin,
    const glm::vec2& direction,
    glm::vec2& hitPoint,
    float& tHit,
    const std::vector<Object*>& ignore
) {
    Object* closestObject = nullptr;
    float closestT = 1.0f;

    glm::vec2 dirNorm = glm::normalize(direction);

    for (auto& [zIndex, objectsVector] : objects) {
        for (Object* object : objectsVector) {
            if (std::find(ignore.begin(), ignore.end(), object) != ignore.end())
                continue;

            if (!object->canCollide)
                continue;

            // =========================
            // Build OBB basis
            // =========================
            float rad = object->rotation * glm::pi<float>() / 180.0f;

            glm::vec2 Ax(std::cos(rad), std::sin(rad));
            glm::vec2 Ay(-std::sin(rad), std::cos(rad));

            // half extents
            glm::vec2 half = object->size * 0.5f;

            // center of box
            glm::vec2 center = object->position + half;

            // =========================
            // Transform ray into OBB space
            // =========================
            glm::vec2 relOrigin = rayOrigin - center;

            glm::vec2 localOrigin(
                glm::dot(relOrigin, Ax),
                glm::dot(relOrigin, Ay)
            );

            glm::vec2 localDir(
                glm::dot(dirNorm, Ax),
                glm::dot(dirNorm, Ay)
            );

            // =========================
            // AABB slab test in local space
            // =========================
            glm::vec2 min(-half.x, -half.y);
            glm::vec2 max( half.x,  half.y);

            float tMin = 0.0f;
            float tMax = 1.0f;

            for (int i = 0; i < 2; i++) {

                float o = (i == 0) ? localOrigin.x : localOrigin.y;
                float d = (i == 0) ? localDir.x : localDir.y;
                float mn = (i == 0) ? min.x : min.y;
                float mx = (i == 0) ? max.x : max.y;

                if (fabs(d) < 1e-8f) {
                    if (o < mn || o > mx) {
                        tMin = 2.0f;
                        break;
                    }
                } else {
                    float invD = 1.0f / d;
                    float t1 = (mn - o) * invD;
                    float t2 = (mx - o) * invD;

                    if (t1 > t2) std::swap(t1, t2);

                    tMin = std::max(tMin, t1);
                    tMax = std::min(tMax, t2);

                    if (tMin > tMax)
                        break;
                }
            }

            if (tMin > tMax) continue;
            if (tMax < 0.0f) continue;

            float t = std::max(tMin, 0.0f);

            if (t <= 1.0f && t < closestT) {
                closestT = t;
                closestObject = object;
            }
        }
    }

    if (closestObject) {
        tHit = closestT;
        hitPoint = rayOrigin + dirNorm * closestT;
        return closestObject;
    }

    return nullptr;
}

void Object::pendDelete() {
  pendingDelete = true;
}

bool Object::isDeleted() {
  return pendingDelete;
}
