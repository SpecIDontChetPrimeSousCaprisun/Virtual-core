#include "Container.h"

std::vector<Container*> Container::containers;

void Container::deletePendingObjects() {
  for (auto it = containers.begin(); it != containers.end(); ) {
    Container* container = *it;

    if (container->pendingDelete) {
      it = containers.erase(it);
      delete container;
    }
    else {
      ++it;
    }
  }
}

Container::Container(std::vector<Object*> objects) : objects(objects), pendingDelete(false) {}

void Container::changeVisibility(bool visible) {
  for (Object* object : objects) {
    object->visible = visible;
  }
}

void Container::changeCornerRadius(float radius) {
  for (Object* object : objects) {
    object->cornerRadius = radius;
  }
}

void Container::registerObjects() {
  for (Object* object : objects) {
    object->registerObject();
  }
}

void Container::clear() {
  for (Object* object : objects) {
    object->pendDelete();
  }

  objects.clear();
}

void Container::pendDelete() {
  pendingDelete = true;
}

void Container::cancelDelete() {
  pendingDelete = false;
}

bool Container::isDeleted() {
  return pendingDelete;
}
