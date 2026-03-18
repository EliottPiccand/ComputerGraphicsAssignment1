#include "Entity/Missile.h"

#include "Event.h"
#include "Utils/Constants.h"

using namespace entity;

Missile::Missile(int entityId, glm::vec2 position, glm::vec2 target)
    : Entity(entityId), position(position), target(target)
{
}

void Missile::update(float deltaTime, Input &input, const Camera &camera, EventHandler &events, World &world)
{
    const glm::vec2 previousPosition = position;
    const glm::vec2 direction = glm::normalize(target - position);
    position += direction * deltaTime * MISSILE_SPEED;

    glm::vec2 hitPosition{};
    if (world.checkMissileCollision(previousPosition, position, hitPosition))
    {
        events.post<event::TargetReachedEvent>(id, hitPosition);
        world.moveWater(hitPosition, EXPLOSION_MAX_RADIUS * 0.5f);
        return;
    }

    if (glm::length(position - target) < MISSILE_TARGET_ERROR_MARGIN)
    {
        events.post<event::TargetReachedEvent>(id, target);
        world.moveWater(target, EXPLOSION_MAX_RADIUS * 0.5f);
    }
}

void Missile::render() const
{
    // Missile
    glColor3f(MISSILE_COLOR);
    glPointSize(MISSILE_SIZE);
    glBegin(GL_POINTS);
    glVertex2f(position.x, position.y);
    glEnd();

    // Ray
    glPushAttrib(GL_ENABLE_BIT);

    glLineStipple(1, 0xFF00);
    glEnable(GL_LINE_STIPPLE);

    glColor3f(MISSILE_RAY_COLOR);
    glLineWidth(MISSILE_RAY_WIDTH);

    glBegin(GL_LINES);
    glVertex2f(position.x, position.y);
    glVertex2f(target.x, target.y);
    glEnd();

    glPopAttrib();
}
