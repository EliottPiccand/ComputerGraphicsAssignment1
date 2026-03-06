#pragma once

#include <glm/glm.hpp>

#include "Entity.h"
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
    SpeedState SpeedState;

  public:
    Ship(glm::vec2 position, float orientation, Input &input);

    void update(float deltaTime, Input &input) override;
    void render() const override;
};
