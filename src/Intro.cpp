#include "Intro.h"
#include "Window.h"
#include "Button.h"
#include "Player.h"
#include "Item.h"
#include "LevelLoader.h"

std::vector<std::string> Intro::coutText;
std::vector<std::string> Intro::quitCoutText;
std::vector<TextElement*> Intro::cout;
float Intro::time = 0.0f;
int Intro::lastCoutText = 0;
UIElement* Intro::background;
Container* Intro::ui;
bool Intro::finishedIntro = false;
bool Intro::quitAnimation = false;

void Intro::init() {
  coutText.push_back("Loading Linux kernel...");
  coutText.push_back("EFI stub: Booting Linux Kernel...");

  coutText.push_back("[    0.000000] Linux version 6.x.x");
  coutText.push_back("[    0.012345] Command line: root=/dev/nvme0n1p2 ro quiet");

  coutText.push_back("Mounting temporary root filesystem...");
  coutText.push_back("Switching to initramfs...");

  coutText.push_back("[  OK  ] Loaded kernel modules: nvme, ext4, cryptd");
  coutText.push_back("[  OK  ] Detected root device /dev/nvme0n1p2");
  coutText.push_back("[  OK  ] Mounted root filesystem");

  coutText.push_back("Switching to real root...");
  coutText.push_back("Starting init system: OpenRC");

  coutText.push_back("[  OK  ] Mounting /proc, /sys, /dev");
  coutText.push_back("[  OK  ] Starting udev daemon");
  coutText.push_back("[  OK  ] Starting Network Manager");
  coutText.push_back("[ WARN ] wlan0: carrier not detected");

  coutText.push_back("[  OK  ] Reached target multi-user environment");
  coutText.push_back("[  OK  ] Starting display manager");

  coutText.push_back("Rebelion OS (vm-linuz-rebelion-dist-bin)");
  coutText.push_back("--------------------------------------------------");
  coutText.push_back("Auto-login: Hope");
  coutText.push_back(R"( ____  _____ ____  _____ _     ___ ___  _   _    ___  ____  )");
  coutText.push_back(R"(|  _ \| ____| __ )| ____| |   |_ _/ _ \| \ | |  / _ \/ ___| )");
  coutText.push_back(R"(| |_) |  _| |  _ \|  _| | |    | | | | |  \| | | | | \___ \ )");
  coutText.push_back(R"(|  _ <| |___| |_) | |___| |___ | | |_| | |\  | | |_| |___) |)");
  coutText.push_back(R"(|_| \_\_____|____/|_____|_____|___\___/|_| \_|  \___/|____/ )");
  coutText.push_back("");
  coutText.push_back("Wellcome back Hope !");
  coutText.push_back("[LOG] Starting Hyprland 0.45.0");
  coutText.push_back("[LOG] Runtime dir: /run/user/1000/hypr");

  coutText.push_back("[LOG] Initializing Wayland display server");
  coutText.push_back("[LOG] Backend: DRM / KMS");

  coutText.push_back("[LOG] Found GPU: NVIDIA GeForce RTX 3060");
  coutText.push_back("[LOG] DRM device opened: /dev/dri/card0");

  coutText.push_back("[LOG] Loading wlroots renderer");
  coutText.push_back("[LOG] OpenGL ES 3.2 detected");

  coutText.push_back("[LOG] Detecting monitors...");
  coutText.push_back("[LOG] eDP-1: 2560x1440@165Hz (primary)");
  coutText.push_back("[LOG] HDMI-A-1: disconnected");

  coutText.push_back("[LOG] Loading config: ~/.config/hypr/hyprland.conf");
  coutText.push_back("[WARN] deprecated option: animations:curve");

  coutText.push_back("[LOG] Spawning core services...");
  coutText.push_back("[LOG] waybar started");
  coutText.push_back("[LOG] hyprpaper initialized");
  coutText.push_back("[LOG] network-manager-applet started");

  coutText.push_back("[LOG] Compositor ready");
  coutText.push_back("[LOG] Entering event loop");

  //Quit cout text

  quitCoutText.push_back("[LOG] Initiating session termination...");
  quitCoutText.push_back("[LOG] Received SIGTERM from user session");

  quitCoutText.push_back("[LOG] Stopping Hyprland compositor");
  quitCoutText.push_back("[LOG] Unbinding Wayland display");
  quitCoutText.push_back("[LOG] Destroying XDG shell surfaces");

  quitCoutText.push_back("[LOG] Stopping user services...");
  quitCoutText.push_back("[OK] waybar terminated");
  quitCoutText.push_back("[OK] swww-daemon stopped");
  quitCoutText.push_back("[OK] network-manager-applet exited");

  quitCoutText.push_back("[LOG] Saving session state");
  quitCoutText.push_back("[LOG] Flushing input buffers");

  quitCoutText.push_back("[OK] Stopping OpenRC user services");
  quitCoutText.push_back("[OK] Reached target graphical session stopped");

  quitCoutText.push_back("[LOG] Unmounting user filesystems...");
  quitCoutText.push_back("[OK] Unmounted /home");
  quitCoutText.push_back("[OK] Unmounted /run/user/1000");

  quitCoutText.push_back("[LOG] Syncing disks...");
  quitCoutText.push_back("[OK] I/O buffers flushed to storage");

  quitCoutText.push_back("[LOG] Releasing DRM devices");
  quitCoutText.push_back("[OK] GPU session released");

  quitCoutText.push_back("");
  quitCoutText.push_back("[  OK  ] Switching to system shutdown target");

  quitCoutText.push_back("[  OK  ] Stopping system services...");
  quitCoutText.push_back("[  OK  ] Stopped NetworkManager");
  quitCoutText.push_back("[  OK  ] Stopped system logging daemon");
  quitCoutText.push_back("[  OK  ] Stopped cron scheduler");
  quitCoutText.push_back("[  OK  ] Stopped udev event handler");

  quitCoutText.push_back("[  OK  ] Terminating remaining processes...");
  quitCoutText.push_back("[  OK  ] All user processes exited");

  quitCoutText.push_back("[  OK  ] Syncing filesystem buffers...");
  quitCoutText.push_back("[  OK  ] EXT4-fs (nvme0n1p2): clean shutdown completed");

  quitCoutText.push_back("[  LOG  ] Unmounting system filesystems...");
  quitCoutText.push_back("[  OK  ] Unmounted /var");
  quitCoutText.push_back("[  OK  ] Unmounted /run");
  quitCoutText.push_back("[  OK  ] Unmounted /proc");
  quitCoutText.push_back("[  OK  ] Unmounted /sys");

  quitCoutText.push_back("[LOG] Kernel shutdown in progress...");
  quitCoutText.push_back("[OK] Freeing unused kernel memory");
  quitCoutText.push_back("[OK] Stopping CPU cores");

  quitCoutText.push_back("[  LOG  ] Reached system power state S5");
  quitCoutText.push_back("[  LOG  ] ACPI: Powering off system");
  quitCoutText.push_back("Power off");

  background = new UIElement(glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), 0.0f, glm::vec3(0.0f, 0.0f, 0.0f), 1);
  background->registerObject();

  std::vector<Object*> uiElements;

  TextElement* title = new TextElement(glm::vec2(0.0f, 0.0f),
                                      glm::vec2(1.0f, 0.2f),
                                      1.0f,
                                      glm::vec3(0.0f, 0.0f, 0.0f),
                                      2,
                                      "Virtual core",
                                      "fonts/JetBrainsMonoNerdFont-Regular.ttf",
                                      glm::vec3(1.0f, 1.0f, 1.0f));
  
  Button* startButton = new Button(glm::vec2(0.0f, 0.6f),
                                  glm::vec2(1.0f, 0.075f),
                                  1.0f,
                                  glm::vec3(0.0f, 0.0f, 0.0f),
                                  2,
                                  "Start",
                                  "fonts/JetBrainsMonoNerdFont-Regular.ttf",
                                  glm::vec3(1.0f, 1.0f, 1.0f));

  Button* continueButton = new Button(glm::vec2(0.0f, 0.675f),
                                  glm::vec2(1.0f, 0.075f),
                                  1.0f,
                                  glm::vec3(0.0f, 0.0f, 0.0f),
                                  2,
                                  "Continue",
                                  "fonts/JetBrainsMonoNerdFont-Regular.ttf",
                                  glm::vec3(1.0f, 1.0f, 1.0f));

  Button* quitButton = new Button(glm::vec2(0.0f, 0.75f),
                                  glm::vec2(1.0f, 0.075f),
                                  1.0f,
                                  glm::vec3(0.0f, 0.0f, 0.0f),
                                  2,
                                  "Quit",
                                  "fonts/JetBrainsMonoNerdFont-Regular.ttf",
                                  glm::vec3(1.0f, 1.0f, 1.0f));



  title->textCentered = false;

  startButton->textCentered = false;
  startButton->setHoverCallback([startButton](bool hovered) {
    if (hovered) {
      startButton->text = "$ Start";
    } else {
      startButton->text = "Start";
    }
  });
  startButton->setCallback([]() {
    ui->changeVisibility(false);
    background->visible = false;
    
    LevelLoader::loadLevel("levels/testLevel");
  });

  continueButton->textCentered = false;

  quitButton->textCentered = false;
  quitButton->setHoverCallback([quitButton](bool hovered) {
    if (hovered) {
      quitButton->text = "$ Quit";
    } else {
      quitButton->text = "Quit";
    }
  });
  quitButton->setCallback([]() {
    quitAnimation = true;
    lastCoutText = 0;
    time = 0.0f;
    ui->changeVisibility(false);
  });

  uiElements.push_back(title);
  uiElements.push_back(startButton);
  uiElements.push_back(continueButton);
  uiElements.push_back(quitButton);

  ui = new Container(uiElements);
  ui->changeVisibility(false);
  ui->registerObjects();
}

void Intro::open() {
  time = 0;
  background->visible = true;
  finishedIntro = false;
}

void Intro::update() {
  time += Window::deltaTime;

  if (!finishedIntro) {
    if ((std::floor(time * 10.0f)) > lastCoutText && lastCoutText < coutText.size()) {
      TextElement* text = new TextElement(glm::vec2(0.0f, 0.02f * lastCoutText),
                                          glm::vec2(1.0f, 0.02f),
                                          1.0f,
                                          glm::vec3(0.0f, 0.0f, 0.0f),
                                          11,
                                          coutText[lastCoutText],
                                          "fonts/JetBrainsMonoNerdFont-Regular.ttf",
                                          glm::vec3(1.0f, 1.0f, 1.0f));

      text->textCentered = false;
      text->registerObject();

      cout.push_back(text);

      lastCoutText++;
    } else if (lastCoutText >= coutText.size()) {
      for (TextElement* text : cout) {
        text->pendDelete();
      }

      cout.clear();

      ui->changeVisibility(true);
      finishedIntro = true;
    }
  } else if (quitAnimation) {
    if ((std::floor(time * 10.0f)) > lastCoutText && lastCoutText < quitCoutText.size()) {
      TextElement* text = new TextElement(glm::vec2(0.0f, 0.02f * lastCoutText),
                                          glm::vec2(1.0f, 0.02f),
                                          1.0f,
                                          glm::vec3(0.0f, 0.0f, 0.0f),
                                          11,
                                          quitCoutText[lastCoutText],
                                          "fonts/JetBrainsMonoNerdFont-Regular.ttf",
                                          glm::vec3(1.0f, 1.0f, 1.0f));

      text->textCentered = false;
      text->registerObject();

      cout.push_back(text);

      lastCoutText++;
    } else if (lastCoutText >= quitCoutText.size()) {
      Window::inGame = false;
      quitAnimation = false;
    }
  }
}
