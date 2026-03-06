#pragma once

#include <glm/glm.hpp>

class World
{
  public:
    static glm::vec2 checkCollision(glm::vec2 &old_pos, glm::vec2 &new_pos);
    static void render();
};
