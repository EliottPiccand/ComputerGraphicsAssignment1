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

    const glm::vec2 &getPosition() const;
    float getOrientation() const;
    float getWidth() const;
    float getDepth() const;

    void update(float deltaTime, Input &input, const Camera &camera, EventHandler &events, World &world) override;
    void render() const override;
};

} // namespace entity