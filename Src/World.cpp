#include "World.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <ranges>

World::World()
{
    water = std::make_unique<World::WorldGrid>();

    for (auto &row : *water)
    {
        row.fill(std::make_tuple(0.0f, 0.0f));
    }
}

void World::moveWater(const glm::vec2 &position, float radius)
{
    for (auto &&[y, row] : *water | std::views::enumerate)
    {
        for (auto &&[x, data] : row | std::views::enumerate)
        {
            auto &[height, previousHeight] = data;

            const glm::vec2 cellPosition{x * WORLD_SUBDIVISION_SIZE, y * WORLD_SUBDIVISION_SIZE};
            const float distance = glm::length(cellPosition - position);
            if (distance <= radius)
            {
                const float normalizedDistance = distance / radius;
                height = 0.5f * std::exp(1.0f - 1.0f / (1.0f - normalizedDistance * normalizedDistance));
                height *= radius;
                height = std::clamp(height, -WORLD_SUBDIVISION_SIZE * 10.0f, WORLD_SUBDIVISION_SIZE * 10.0f);
                previousHeight = height;
                // Todo : compute the total losed volume and readd ot through waves
            }
        }
    }
}

void World::update(float deltaTime)
{
    auto &water = *this->water;

    if (deltaTime > 1.0f / 30.0f)
    {
        // note: if the time step is too big, the CLF stability conditions are not met, causing the simulation to
        // diverge.
        return;
    }

    for (auto &&[y, row] : water | std::views::enumerate)
    {
        for (auto &&[x, data] : row | std::views::enumerate)
        {
            auto &[height, velocity] = data;

            const float heightLeft = 0 <= x - 1 ? std::get<0>(water[y][x - 1]) : 0.0f;
            const float heightRight = x + 1 < static_cast<long long>(row.size()) ? std::get<0>(water[y][x + 1]) : 0.0f;
            const float heightUp = 0 <= y - 1 ? std::get<0>(water[y - 1][x]) : 0.0f;
            const float heightDown = y + 1 < static_cast<long long>(water.size()) ? std::get<0>(water[y + 1][x]) : 0.0f;

            const float laplacian = (heightLeft + heightRight + heightUp + heightDown - 4.0f * height);
            velocity += laplacian * WATER_WAVES_SPEED * deltaTime;
            velocity -= velocity * WATER_WAVES_DAMPING * deltaTime;
        }
    }

    for (auto &&[y, row] : water | std::views::enumerate)
    {
        for (auto &&[x, data] : row | std::views::enumerate)
        {
            auto &[height, velocity] = data;
            height += velocity * deltaTime;
        }
    }
}

static glm::vec3 waterColor(float height, float x, float y)
{
    return glm::mix(WATER_COLOR, glm::vec3(FOAM_COLOR), height / (WORLD_SUBDIVISION_SIZE * 3.0f));
}

void World::render() const
{
    const auto &water = *this->water;

    for (const auto &[y, row] : water | std::views::enumerate)
    {
        for (const auto &[x, data] : row | std::views::enumerate)
        {
            const auto &[height, _] = data;

            const glm::vec3 color = waterColor(height, static_cast<float>(x) / static_cast<float>(row.size()),
                                               static_cast<float>(y) / static_cast<float>(water.size()));

            const float xFloat = static_cast<float>(x);
            const float yFloat = static_cast<float>(y);

            glColor3f(color.r, color.g, color.b);
            glRectf(xFloat * WORLD_SUBDIVISION_SIZE, yFloat * WORLD_SUBDIVISION_SIZE,
                    (xFloat + 1.0f) * WORLD_SUBDIVISION_SIZE, (yFloat + 1.0f) * WORLD_SUBDIVISION_SIZE);
        }
    }
}

void World::checkCollision(glm::vec2 &position, float orientation) const
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
