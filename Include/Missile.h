#pragma once

#include "Entity.h"
#include "EventHandler.h"

class Missile : public Entity
{
  private:
    glm::vec2 position;
    glm::vec2 target;

  public:
    Missile(int entityId, glm::vec2 position, glm::vec2 target);

    void update(float deltaTime, Input &input, Camera &camera, EventHandler &events) override;
    void render() const override;
};
