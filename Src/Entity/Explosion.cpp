#include "Entity/Explosion.h"

#include "Event.h"
#include "GL.h"
#include "Utils/Constants.h"
#include "Utils/Math.h"

using namespace entity;

Explosion::Explosion(int entityId, const glm::vec2 &position) : Entity(entityId), position(position), radius(0.0f)
{
}

void Explosion::update(float deltaTime, Input &input, const Camera &camera, EventHandler &events)
{
    radius += EXPLOSION_EXPANSION_RATE * deltaTime;
    if (radius > EXPLOSION_MAX_RADIUS)
    {
        events.post<event::ExplosionDoneEvent>(id);
    }
}

void Explosion::render() const
{
    const float t = radius / EXPLOSION_MAX_RADIUS;
    glm::vec3 color = lerp(glm::vec3(EXPLOSION_COLOR_START), glm::vec3(EXPLOSION_COLOR_END), t);
    glColor3f(color.x, color.y, color.y);
    glPointSize(radius);
    glBegin(GL_POINTS);
    glVertex2f(position.x, position.y);
    glEnd();
}
