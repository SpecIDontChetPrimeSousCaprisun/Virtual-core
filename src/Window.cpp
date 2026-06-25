#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif
#include "Window.h"
#include "Object.h"
#include "Player.h"
#include "UIElement.h"
#include "Button.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <sstream>
#include <iomanip>

// REQUIRED definition (missing in your case)
GLFWwindow* Window::window = nullptr;
double Window::deltaTime = 0;
int Window::fbWidth = 600;
int Window::fbHeight = 480;
bool Window::inGame = true;
TextElement* Window::fpsLabel;
Container* Window::menu;
ma_sound* Window::music;

double lastFrame = 0.0;
bool toggledMenu = false;
bool resizing = false;

int Window::init() {
  glfwSetErrorCallback([](int error, const char* description) {
      std::cout << "GLFW ERROR: " << description << "\n";
  }); 

  /* Initialize the library */
  if (!glfwInit())
      return -1; 

  glfwWindowHint(GLFW_DEPTH_BITS, 24);

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(fbWidth, fbHeight, "Game", NULL, NULL);

  if (!window)
  {
      glfwTerminate();
      return -1;
  } 

  /* Make the window's context current */
  glfwMakeContextCurrent(window);
  glfwSwapInterval(0); 

  #ifndef __EMSCRIPTEN__
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
  #endif

  glClearColor(0.2f, 0.4f, 0.6f, 1.0f);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glm::mat4 projection = glm::ortho(
      0.0f, (float)fbWidth,
      (float)fbHeight, 0.0f,
      -1.0f, 1.0f
  );
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 

  glfwSetFramebufferSizeCallback(window,
  [](GLFWwindow*, int width, int height)
  {
      Window::fbWidth = width;
      Window::fbHeight = height;

      resizing = (width == 0 || height == 0);

      glViewport(0, 0, width, height);
  });

  //music = Sound::playLoopingSound("sfx/Music.mp3");

  return 0;
}

void Window::updateFrame() {
  if (resizing) {
    glfwPollEvents();
    glfwSwapBuffers(window);
    return;
  }

  glfwPollEvents();

  double currentFrame = glfwGetTime();
  deltaTime = currentFrame - lastFrame;
  lastFrame = currentFrame;

  double fps = 1.0 / deltaTime; 

  if (deltaTime > 0.1) {
    deltaTime = 0.1;
  } 

  std::ostringstream ss;
  ss << std::fixed << std::setprecision(1) << fps;
  std::string fpsString = ss.str();

  fpsLabel->text = "FPS : " + fpsString; 

  #ifdef __EMSCRIPTEN__
  double cssWidth, cssHeight;

  emscripten_get_element_css_size(
      "#canvas",
      &cssWidth,
      &cssHeight
  );

  double devicePixelRatio =
      emscripten_get_device_pixel_ratio();

  int width =
      (int)(cssWidth * devicePixelRatio);

  int height =
      (int)(cssHeight * devicePixelRatio);

  emscripten_set_canvas_element_size(
      "#canvas",
      width,
      height
  );
  #endif

  glfwGetFramebufferSize(window, &fbWidth, &fbHeight);

  if (fbWidth == 0 || fbHeight == 0) {
    glfwSwapBuffers(window);
    return;
  }
 
  glViewport(0, 0, fbWidth, fbHeight);

  glClear(GL_COLOR_BUFFER_BIT);
 
  #ifdef __EMSCRIPTEN__
  if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS && !toggledMenu) {
    menu->changeVisibility(!menu->objects[0]->visible);
    toggledMenu = true;
  } else if (glfwGetKey(window, GLFW_KEY_TAB) != GLFW_PRESS) {
    toggledMenu = false;
  }
  #else
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && !toggledMenu) {
    menu->changeVisibility(!menu->objects[0]->visible);
    toggledMenu = true;
  } else if (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS) {
    toggledMenu = false;
  }
  #endif

  Object::registerAll();

  Player::update();
  Object::updateAll();

  Object::drawAll();

  glfwSwapBuffers(window);
}

#ifdef __EMSCRIPTEN__
void em_loop() {
  Window::updateFrame();
}

void Window::mainLoop() {
  std::vector<Object*> menuElements;
  
  UIElement* menuBackground = new UIElement(glm::vec2(0.5f, 0.5f), glm::vec2(0.5f, 0.75f), 0.0f, "textures/Wallpaper.jpeg", 1);
  TextElement* nameLabel = new TextElement(glm::vec2(0.5f, 0.1f), glm::vec2(0.5f, 0.1f), 1.0f, "textures/Wallpaper.jpeg", 2, "GAME NAME HERE", "fonts/Kenney Future Narrow.ttf", glm::vec3(0.0f, 0.0f, 0.0f));
  Button* exitButton = new Button(glm::vec2(0.5f, 0.9f), glm::vec2(0.5f, 0.1f), 0.0f, "textures/Wallpaper.jpeg", 2, "Exit", "fonts/Kenney Future Narrow.ttf", glm::vec3(1.0f, 0.0f, 0.0f));
  Button* muteButton = new Button(glm::vec2(0.5f, 0.8f), glm::vec2(0.5f, 0.1f), 0.0f, "textures/Wallpaper.jpeg", 2, "Mute music", "fonts/Kenney Future Narrow.ttf", glm::vec3(0.0f, 0.0f, 0.0f));

  exitButton->setCallback([]() {
    inGame = false;
  });

  muteButton->setCallback([]() {
    ma_sound_stop(music);
  });

  menuBackground->anchorPoint = glm::vec2(0.5f, 0.5f);
  nameLabel->anchorPoint = glm::vec2(0.5f, 0.0f);
  exitButton->anchorPoint = glm::vec2(0.5f, 1.0f);
  muteButton->anchorPoint = glm::vec2(0.5f, 1.0f);
  menuElements.push_back(menuBackground);
  menuElements.push_back(nameLabel);
  menuElements.push_back(exitButton);
  menuElements.push_back(muteButton);

  menu = new Container(menuElements);
  menu->changeVisibility(false);
  menu->registerObjects();
 
  fpsLabel = new TextElement(glm::vec2(0.0f, 0.0f), glm::vec2(0.25f, 0.1f), 1.0f, "textures/Wallpaper.jpeg", 2, "FPS : 0", "fonts/Kenney Future Narrow.ttf", glm::vec3(0.0f, 0.0f, 0.0f));
  fpsLabel->registerObject();

  emscripten_set_main_loop(em_loop, 0, 1);
}
#else
void Window::mainLoop() { 
  std::vector<Object*> menuElements;
  
  UIElement* menuBackground = new UIElement(glm::vec2(0.5f, 0.5f), glm::vec2(0.5f, 0.75f), 0.0f, "textures/Wallpaper.jpeg", 1);
  TextElement* nameLabel = new TextElement(glm::vec2(0.5f, 0.1f), glm::vec2(0.5f, 0.1f), 1.0f, "textures/Wallpaper.jpeg", 2, "GAME NAME HERE", "fonts/Kenney Future Narrow.ttf", glm::vec3(0.0f, 0.0f, 0.0f));
  Button* exitButton = new Button(glm::vec2(0.5f, 0.9f), glm::vec2(0.5f, 0.1f), 0.0f, "textures/Wallpaper.jpeg", 2, "Exit", "fonts/Kenney Future Narrow.ttf", glm::vec3(1.0f, 0.0f, 0.0f));
  Button* muteButton = new Button(glm::vec2(0.5f, 0.8f), glm::vec2(0.5f, 0.1f), 0.0f, "textures/Wallpaper.jpeg", 2, "Mute music", "fonts/Kenney Future Narrow.ttf", glm::vec3(0.0f, 0.0f, 0.0f));

  exitButton->setCallback([]() {
    inGame = false;
  });

  muteButton->setCallback([]() {
    ma_sound_stop(music);
  });

  menuBackground->anchorPoint = glm::vec2(0.5f, 0.5f);
  nameLabel->anchorPoint = glm::vec2(0.5f, 0.0f);
  exitButton->anchorPoint = glm::vec2(0.5f, 1.0f);
  muteButton->anchorPoint = glm::vec2(0.5f, 1.0f);
  menuElements.push_back(menuBackground);
  menuElements.push_back(nameLabel);
  menuElements.push_back(exitButton);
  menuElements.push_back(muteButton);

  menu = new Container(menuElements);
  menu->changeVisibility(false);
  menu->registerObjects();

  fpsLabel = new TextElement(glm::vec2(0.0f, 0.0f), glm::vec2(0.25f, 0.1f), 1.0f, "textures/Wallpaper.jpeg", 2, "FPS : 0", "fonts/Kenney Future Narrow.ttf", glm::vec3(0.0f, 0.0f, 0.0f));
  fpsLabel->registerObject();

  while (!glfwWindowShouldClose(window) && inGame){
    updateFrame();
  }

  glfwTerminate();
}
#endif

