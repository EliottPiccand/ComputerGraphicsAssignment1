#include "Entity/Missile.h"

#include "Event.h"
#include "Utils/Constants.h"

using namespace entity;

Missile::Missile(int entityId, glm::vec2 position, glm::vec2 target)
    : Entity(entityId), position(position), target(target)
{
}

void Missile::update(float deltaTime, Input &input, const Camera &camera, EventHandler &events)
{
    const glm::vec2 direction = glm::normalize(target - position);
    position += direction * deltaTime * MISSILE_SPEED;

    if (glm::length(position - target) < MISSILE_TARGET_ERROR_MARGIN)
    {
        events.post<event::TargetReachedEvent>(id, target);
    }
}

void Missile::render() const
{
    glColor3f(MISSILE_COLOR);
    glPointSize(MISSILE_SIZE);
    glBegin(GL_POINTS);
        glVertex2f(position.x, position.y);
    glEnd();
}
