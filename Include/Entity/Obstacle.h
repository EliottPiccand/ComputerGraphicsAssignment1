#pragma once

#include "Entity/Entity.h"

namespace entity
{

class Obstacle : public Entity
{
  private:
    glm::vec2 position;
    float orientation;
    float width;
    float depth;

  public:
    Obstacle(int entityId, glm::vec2 position);

    [[nodiscard]] const glm::vec2 &getPosition() const;
    [[nodiscard]] float getOrientation() const;
    [[nodiscard]] float getWidth() const;
    [[nodiscard]] float getDepth() const;

    void update(float deltaTime, Input &input, const Camera &camera, EventHandler &events, World &world) override;
    void render() const override;
};

} // namespace entity