#pragma once

#include "Entity.h"
#include "EventHandler.h"
#include "GL.h"
#include "Input.h"

class Ship : public Entity
{
  public:
    enum class SpeedState
    {
        Forward,
        Stop,
        Backward,
    };

  private:
    glm::vec2 position;
    float orientation;
    SpeedState speedState;

    bool aiming;
    bool aimingValidPosition;
    glm::vec2 targetPosition;

  public:
    Ship(int id, glm::vec2 position, float orientation, Input &input);

    void update(float deltaTime, Input &input, Camera &camera, EventHandler &events) override;
    void render() const override;
};
