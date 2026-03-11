#pragma once

#include "Camera.h"
#include "Container/CyclicArray.h"
#include "Entity/Entity.h"
#include "EventHandler.h"
#include "GL.h"
#include "Input.h"
#include "Utils/Constants.h"
#include "Utils/Time.h"

namespace entity
{

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

    CyclicArray<glm::vec2, TRAIL_LENGTH> trail;
    Instant lastTrailUpdate;

  public:
    Ship(int id, glm::vec2 position, float orientation, Input &input);

    void update(float deltaTime, Input &input, const Camera &camera, EventHandler &events) override;
    void render() const override;
};

} // namespace entity
