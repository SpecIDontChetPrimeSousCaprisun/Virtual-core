#pragma once

#include <iostream>
#include "../include/glad/glad.h"
#include "TextElement.h"
#include "Container.h"
#include <GLFW/glfw3.h>
#include <vector>
#include "Sound.h"

class Window {
public:
  static int init();
  static void mainLoop();
  static void updateFrame();
  static GLFWwindow* window;

  static double deltaTime;
  static int fbWidth, fbHeight;
  static bool inGame;
  static TextElement* fpsLabel;
  static Container* menu;
  static ma_sound* music;
};
