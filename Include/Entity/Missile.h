#pragma once

#include "Camera.h"
#include "Entity/Entity.h"
#include "EventHandler.h"
#include "GL.h"
#include "Input.h"

namespace entity
{

class Missile : public Entity
{
  private:
    glm::vec2 position;
    glm::vec2 target;

  public:
    Missile(int entityId, glm::vec2 position, glm::vec2 target);

    void update(float deltaTime, Input &input, const Camera &camera, EventHandler &events) override;
    void render() const override;
};

} // namespace entity
