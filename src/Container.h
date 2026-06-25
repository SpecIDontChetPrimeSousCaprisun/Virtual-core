#pragma once

#include "Object.h"
#include <vector>

class Container {
public:
  Container(std::vector<Object*> objects);
  
  static void deletePendingObjects();

  void changeVisibility(bool visible);
  void changeCornerRadius(float radius);
  void registerObjects();
  void clear();
  void pendDelete();
  void cancelDelete();
  bool isDeleted();

  std::vector<Object*> objects;
private:
  static std::vector<Container*> containers;

  bool pendingDelete;
};
