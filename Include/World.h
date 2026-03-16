#pragma once

#include <array>
#include <memory>
#include <tuple>

#include "GL.h"
#include "Utils/Constants.h"

class World
{
  private:
    using WorldGrid = std::array<
        //                   height, previous height
        std::array<std::tuple<float, float>, static_cast<std::size_t>(WORLD_WIDTH / WORLD_SUBDIVISION_SIZE)>,
        static_cast<std::size_t>(WORLD_HEIGHT / WORLD_SUBDIVISION_SIZE)>;
    std::unique_ptr<WorldGrid> water;

  public:
    World();

    void checkCollision(glm::vec2 &position, float orientation) const;
    void moveWater(const glm::vec2 &position, float radius);

    void update(float deltaTime);
    void render() const;
};
