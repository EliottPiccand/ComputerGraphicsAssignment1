#pragma once

#include "Camera.h"
#include "Container/CyclicQueue.h"
#include "Entity/Entity.h"
#include "EventHandler.h"
#include "GL.h"
#include "Input.h"
#include "Utils/Constants.h"

namespace entity
{

class Ship : public Entity
{
  private:
    enum class SpeedState
    {
        Forward,
        Stop,
        Backward,
    };

    glm::vec2 position;
    float orientation;
    SpeedState speedState;

    bool aiming;
    bool aimingValidPosition;
    glm::vec2 targetPosition;

    struct TrailParticle
    {
        glm::vec2 position;
        float intensity;
    };

    CyclicQueue<TrailParticle, MAX_TRAIL_PARTICLES> trailParticles;

  public:
    Ship(int id, glm::vec2 position, float orientation, Input &input);

    void update(float deltaTime, Input &input, const Camera &camera, EventHandler &events) override;
    void render() const override;
};

} // namespace entity
