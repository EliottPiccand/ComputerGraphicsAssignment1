#pragma once

#include <array>
#include <memory>
#include <tuple>
#include <vector>

#include "GL.h"
#include "Utils/Constants.h"

class World
{
  private:
    struct ObstacleCollider
    {
        glm::vec2 position;
        float orientation;
        float width;
        float height;
    };

    using WorldGrid = std::array<
        //                   height, vertical velocity
        std::array<std::tuple<float, float>, static_cast<std::size_t>(WORLD_WIDTH / WORLD_SUBDIVISION_SIZE)>,
        static_cast<std::size_t>(WORLD_HEIGHT / WORLD_SUBDIVISION_SIZE)>;
    std::unique_ptr<WorldGrid> water;

    std::vector<ObstacleCollider> obstacles;

  public:
    World();

    bool checkCollision(glm::vec2 &position, float orientation) const;
    void addObstacle(const glm::vec2 &position, float orientation, float width, float height);
    void moveWater(const glm::vec2 &position, float radius);

    void update(float deltaTime);
    void render() const;
};
