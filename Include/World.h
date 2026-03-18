#pragma once

#include <array>
#include <memory>
#include <tuple>
#include <vector>

#include "GL.h"
#include "Utils/Constants.h"

namespace entity
{
class Obstacle;
}

class World
{
  private:
    using WorldGrid = std::array<
        //                   height, vertical velocity
        std::array<std::tuple<float, float>, static_cast<std::size_t>(WORLD_WIDTH / WORLD_SUBDIVISION_SIZE)>,
        static_cast<std::size_t>(WORLD_HEIGHT / WORLD_SUBDIVISION_SIZE)>;
    std::unique_ptr<WorldGrid> water;
    std::vector<std::shared_ptr<entity::Obstacle>> obstacles;

  public:
    World();

    bool checkCollision(glm::vec2 &position, float orientation) const;
    bool canPlaceObstacle(const std::shared_ptr<entity::Obstacle> &obstacle) const;
    bool checkMissileCollision(const glm::vec2 &start, const glm::vec2 &end, glm::vec2 &hitPosition) const;
    void addObstacle(const std::shared_ptr<entity::Obstacle> &obstacle);
    void moveWater(const glm::vec2 &position, float radius);

    void update(float deltaTime);
    void render() const;
};
