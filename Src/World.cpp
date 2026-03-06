#include "World.h"

#include <limits>

#include "Utils/Constants.h"

void World::render()
{
    glColor3f(WATER_COLOR);
    glRectf(0.0f, 0.0f, WORLD_WIDTH, WORLD_HEIGHT);
}

void World::checkCollision(glm::vec2 &position, float orientation)
{
    float left = std::numeric_limits<float>::max();
    float right = std::numeric_limits<float>::min();
    float top = std::numeric_limits<float>::max();
    float bottom = std::numeric_limits<float>::min();

    for (const auto &vertex : SHIP_VERTICES)
    {
        const glm::vec2 vertexPosition = position + glm::rotate(vertex * SHIP_SCALE, glm::radians(orientation));
        if (vertexPosition.x < left)
        {
            left = vertexPosition.x;
        }
        if (vertexPosition.x > right)
        {
            right = vertexPosition.x;
        }

        if (vertexPosition.y < top)
        {
            top = vertexPosition.y;
        }
        if (vertexPosition.y > bottom)
        {
            bottom = vertexPosition.y;
        }
    }

    if (left < 0.0f)
    {
        position.x -= left;
    }
    if (right > WORLD_WIDTH)
    {
        position.x += WORLD_WIDTH - right;
    }
    if (top < 0.0f)
    {
        position.y -= top;
    }
    if (bottom > WORLD_HEIGHT)
    {
        position.y += WORLD_HEIGHT - bottom;
    }
}
