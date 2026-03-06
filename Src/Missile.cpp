#include "Missile.h"

#include "Event.h"
#include "Utils/Constants.h"

Missile::Missile(int entityId, glm::vec2 position, glm::vec2 target)
    : Entity(entityId), position(position), target(target)
{
}

void Missile::update(float deltaTime, Input &input, Camera &camera, EventHandler &events)
{
    const glm::vec2 direction = glm::normalize(target - position);
    position += direction * deltaTime * MISSILE_SPEED;

    if (glm::length(position - target) < MISSILE_TARGET_ERROR_MARGIN)
    {
        events.post<event::TargetReachedEvent>(id);
    }
}

void Missile::render() const
{
    glColor3f(1.0, 1.0, 0.0);
    glRectf(position.x - 10.0f, position.y - 10.0f, position.x + 10.0f, position.y + 10.0f);
}
