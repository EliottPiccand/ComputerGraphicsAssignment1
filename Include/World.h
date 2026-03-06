#pragma once

#include "GL.h"

class World
{
  public:
    static void checkCollision(glm::vec2 &position, float orientation);
    static void render();
};
