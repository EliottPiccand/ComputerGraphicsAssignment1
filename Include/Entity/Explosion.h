#pragma once

#include <array>

#include "Camera.h"
#include "Entity/Entity.h"
#include "EventHandler.h"
#include "GL.h"
#include "Input.h"
#include "Utils/Constants.h"

namespace entity
{

class Explosion : public Entity
{
  private:
    const glm::vec2 position;
    const std::array<float, EXPLOSION_COLOR_ENDS.size()> layerRotations;

    float radius;

  public:
    Explosion(int entityId, const glm::vec2 &position);

    void update(float deltaTime, Input &input, const Camera &camera, EventHandler &events) override;
    void render() const override;
};

} // namespace entity
