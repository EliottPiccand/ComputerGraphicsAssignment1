#include "World.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <ranges>

#include "Utils/Constants.h"

namespace
{

std::array<glm::vec2, SHIP_VERTICES.size()> buildShipPolygon(const glm::vec2 &position, float orientation)
{
    std::array<glm::vec2, SHIP_VERTICES.size()> vertices;
    for (auto &&[shipVertex, vertex] : std::views::zip(SHIP_VERTICES, vertices))
    {
        vertex = position + glm::rotate(shipVertex * SHIP_SCALE, glm::radians(orientation));
    }

    return vertices;
}

std::array<glm::vec2, OBSTACLE_VERTEX_COUNT> buildObstaclePolygon(const glm::vec2 &position, float orientation,
                                                                  float width, float height)
{
    const float halfWidth = width * 0.5f;
    const float halfHeight = height * 0.5f;

    const std::array localVertices = {
        glm::vec2{-halfWidth, -halfHeight},
        glm::vec2{halfWidth, -halfHeight},
        glm::vec2{halfWidth, halfHeight},
        glm::vec2{-halfWidth, halfHeight},
    };

    std::array<glm::vec2, OBSTACLE_VERTEX_COUNT> worldVertices;
    for (auto &&[worldVertex, localVertex] : std::views::zip(worldVertices, localVertices))
    {
        worldVertex = position + glm::rotate(localVertex, glm::radians(orientation));
    }

    return worldVertices;
}

template <size_t N>
std::pair<float, float> projectPolygonOnAxis(const std::array<glm::vec2, N> &polygon, const glm::vec2 &axis)
{
    float outMin = std::numeric_limits<float>::max();
    float outMax = std::numeric_limits<float>::min();

    for (const auto &point : polygon)
    {
        const float projection = glm::dot(point, axis);
        outMin = std::min(outMin, projection);
        outMax = std::max(outMax, projection);
    }

    return {outMin, outMax};
}

template <size_t N, size_t M>
bool overlapOnAxis(const std::array<glm::vec2, N> &polygonA, const std::array<glm::vec2, M> &polygonB,
                   const glm::vec2 &axis)
{
    const auto [minA, maxA] = projectPolygonOnAxis(polygonA, axis);
    const auto [minB, maxB] = projectPolygonOnAxis(polygonB, axis);
    return !(maxA + SHIP_OBSTACLE_CLEARANCE < minB || maxB + SHIP_OBSTACLE_CLEARANCE < minA);
}

template <size_t N, size_t M>
bool polygonsIntersect(const std::array<glm::vec2, N> &polygonA, const std::array<glm::vec2, M> &polygonB)
{
    auto hasSeparatingAxis = [&](const auto &polygon) {
        for (const auto &[i, p0] : polygon | std::views::enumerate)
        {
            const glm::vec2 p1 = polygon[(i + 1) % polygon.size()];
            const glm::vec2 edge = p1 - p0;
            const glm::vec2 axis = glm::normalize(glm::vec2{-edge.y, edge.x});

            if (!overlapOnAxis(polygonA, polygonB, axis))
            {
                return true;
            }
        }
        return false;
    };

    return !hasSeparatingAxis(polygonA) && !hasSeparatingAxis(polygonB);
}

} // namespace

World::World()
{
    water = std::make_unique<World::WorldGrid>();

    for (auto &row : *water)
    {
        row.fill(std::make_tuple(0.0f, 0.0f));
    }
}

void World::addObstacle(const glm::vec2 &position, float orientation, float width, float height)
{
    obstacles.push_back(ObstacleCollider{
        .position = position,
        .orientation = orientation,
        .width = width,
        .height = height,
    });
}

void World::moveWater(const glm::vec2 &position, float radius)
{
    for (auto &&[y, row] : *water | std::views::enumerate)
    {
        for (auto &&[x, data] : row | std::views::enumerate)
        {
            auto &[height, velocity] = data;

            const glm::vec2 cellPosition{x * WORLD_SUBDIVISION_SIZE, y * WORLD_SUBDIVISION_SIZE};
            const float distance = glm::length(cellPosition - position);
            if (distance <= radius)
            {
                const float normalizedDistance = distance / radius;
                height = 0.5f * std::exp(1.0f - 1.0f / (1.0f - normalizedDistance * normalizedDistance));
                height *= radius;
                height = std::clamp(height, -WORLD_SUBDIVISION_SIZE * 10.0f, WORLD_SUBDIVISION_SIZE * 10.0f);
                velocity = height;
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
            const float heightRight = x + 1 < row.size() ? std::get<0>(water[y][x + 1]) : 0.0f;
            const float heightUp = 0 <= y - 1 ? std::get<0>(water[y - 1][x]) : 0.0f;
            const float heightDown = y + 1 < water.size() ? std::get<0>(water[y + 1][x]) : 0.0f;

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

namespace
{

glm::vec3 waterColor(float height, float x, float y)
{
    return glm::mix(WATER_COLOR, glm::vec3(FOAM_COLOR), height / (WORLD_SUBDIVISION_SIZE * 3.0f));
}

} // namespace

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

bool World::checkCollision(glm::vec2 &position, float orientation) const
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

    const auto shipPolygon = buildShipPolygon(position, orientation);
    for (const auto &obstacle : obstacles)
    {
        const auto obstaclePolygon =
            buildObstaclePolygon(obstacle.position, obstacle.orientation, obstacle.width, obstacle.height);
        if (polygonsIntersect(shipPolygon, obstaclePolygon))
        {
            return true;
        }
    }

    return false;
}
