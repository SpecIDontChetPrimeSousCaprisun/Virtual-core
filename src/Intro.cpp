#include "Intro.h"
#include "Window.h"

std::vector<std::string> Intro::coutText;
std::vector<TextElement*> Intro::cout;
float Intro::time = 0.0f;
int Intro::lastCoutText = 0;
UIElement* Intro::background;

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
  coutText.push_back("[LOG] swww initialized");
  coutText.push_back("[LOG] network-manager-applet started");

  coutText.push_back("[LOG] Compositor ready");
  coutText.push_back("[LOG] Entering event loop");

  background = new UIElement(glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), 0.0f, glm::vec3(0.0f, 0.0f, 0.0f), 1);
  background->registerObject();
}

void Intro::update() {
  time += Window::deltaTime;

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
  }
}
