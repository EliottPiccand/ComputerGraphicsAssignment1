#include "World.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <limits>
#include <ranges>


namespace
{
constexpr size_t SHIP_VERTEX_COUNT = SHIP_VERTICES.size();
constexpr size_t OBSTACLE_VERTEX_COUNT = 4;
constexpr float SHIP_OBSTACLE_CLEARANCE = 3.0f;

std::array<glm::vec2, SHIP_VERTEX_COUNT> buildShipPolygon(const glm::vec2 &position, float orientation)
{
    std::array<glm::vec2, SHIP_VERTEX_COUNT> vertices{};
    for (size_t i = 0; i < SHIP_VERTEX_COUNT; ++i)
    {
        auto tmp = SHIP_VERTICES[i];
        vertices[i] = position + glm::rotate(tmp * SHIP_SCALE, glm::radians(orientation));
    }
    return vertices;
}

std::array<glm::vec2, OBSTACLE_VERTEX_COUNT> buildObstaclePolygon(const glm::vec2 &position, float orientation,
                                                                  float width, float depth)
{
    const float halfWidth = width * 0.5f;
    const float halfDepth = depth * 0.5f;

    const std::array<glm::vec2, OBSTACLE_VERTEX_COUNT> localVertices = {
        glm::vec2{-halfWidth, -halfDepth},
        glm::vec2{halfWidth, -halfDepth},
        glm::vec2{halfWidth, halfDepth},
        glm::vec2{-halfWidth, halfDepth},
    };

    std::array<glm::vec2, OBSTACLE_VERTEX_COUNT> worldVertices{};
    for (size_t i = 0; i < OBSTACLE_VERTEX_COUNT; ++i)
    {
        auto tmp = localVertices[i];
        worldVertices[i] = position + glm::rotate(tmp, glm::radians(orientation));
    }

    return worldVertices;
}

template <size_t N>
void projectPolygonOnAxis(const std::array<glm::vec2, N> &polygon, const glm::vec2 &axis, float &outMin, float &outMax)
{
    outMin = glm::dot(polygon[0], axis);
    outMax = outMin;
    for (size_t i = 1; i < N; ++i)
    {
        const float projection = glm::dot(polygon[i], axis);
        outMin = std::min(outMin, projection);
        outMax = std::max(outMax, projection);
    }
}

template <size_t N, size_t M>
bool overlapOnAxis(const std::array<glm::vec2, N> &polygonA, const std::array<glm::vec2, M> &polygonB,
                   const glm::vec2 &axis, float clearance)
{
    float minA, maxA, minB, maxB;
    projectPolygonOnAxis(polygonA, axis, minA, maxA);
    projectPolygonOnAxis(polygonB, axis, minB, maxB);
    return !(maxA + clearance < minB || maxB + clearance < minA);
}

template <size_t N, size_t M>
bool polygonsIntersect(const std::array<glm::vec2, N> &polygonA, const std::array<glm::vec2, M> &polygonB,
                       float clearance)
{
    auto hasSeparatingAxis = [&](const auto &polygon) {
        for (size_t i = 0; i < polygon.size(); ++i)
        {
            const glm::vec2 p0 = polygon[i];
            const glm::vec2 p1 = polygon[(i + 1) % polygon.size()];
            const glm::vec2 edge = p1 - p0;
            const glm::vec2 axis = glm::normalize(glm::vec2{-edge.y, edge.x});

            if (!overlapOnAxis(polygonA, polygonB, axis, clearance))
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

void World::addObstacle(const glm::vec2 &position, float orientation, float width, float depth)
{
    obstacles.push_back(ObstacleCollider{
        .position = position,
        .orientation = orientation,
        .width = width,
        .depth = depth,
    });
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
            const float heightRight = x + 1 < row.size() ? std::get<0>(water[y][x + 1]) : 0.0f;
            const float heightUp = 0 <= y - 1 ? std::get<0>(water[y - 1][x]) : 0.0f;
            const float heightDown = y + 1 < water.size() ? std::get<0>(water[y + 1][x]) : 0.0f;

            const float laplacian = (heightLeft + heightRight + heightUp + heightDown - 4.0f * height);
            velocity += laplacian * WATER_WAVES_SPEED * deltaTime;
            velocity -= velocity * WATER_WAVES_DAMPING * deltaTime;
        }
    }

    for (size_t y = 0; y < water.size(); ++y)
    {
        auto &row = water[y];
        for (size_t x = 0; x < row.size(); ++x)
        {
            auto &data = row[x];
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

bool World::checkCollision(glm::vec2 &position, float orientation) const
{
    bool obstacleCollision = false;

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
            buildObstaclePolygon(obstacle.position, obstacle.orientation, obstacle.width, obstacle.depth);
        if (polygonsIntersect(shipPolygon, obstaclePolygon, SHIP_OBSTACLE_CLEARANCE))
        {
            obstacleCollision = true;
            break;
        }
    }

    return obstacleCollision;
}
