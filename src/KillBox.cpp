#include "KillBox.h"
#include "Player.h"
#include "Health.h"

KillBox::KillBox(glm::vec2 position, glm::vec2 size, int zIndex) 
        : Object(position, size, 0.5f, glm::vec3(1.0f, 0.0f, 0.0f), zIndex), wasColliding(false) {}

void KillBox::afterUpdate() {
  bool colliding = false;

  for (Object* obj : lastCollides) {
    Player* player = dynamic_cast<Player*>(obj);

    if (player) colliding = true;
    if (player && !wasColliding) {
      Health::dealDmgToBodyPart("head", 50, true);
    }
  }

  wasColliding = colliding;
}
