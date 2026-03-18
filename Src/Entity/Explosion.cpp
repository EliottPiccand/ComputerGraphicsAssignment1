#include "Entity/Explosion.h"

#include "Event.h"
#include "Utils/Random.h"

using namespace entity;

Explosion::Explosion(int entityId, const glm::vec2 &position)
    : Entity(entityId), position(position), layerRotations([] {
          std::array<float, EXPLOSION_COLOR_ENDS.size()> rotations;

          for (auto &rotation : rotations)
          {
              rotation = Random::random(0.0f, 360.0f);
          }

          return rotations;
      }()),
      radius(0.0f)
{
}

void Explosion::update(float deltaTime, Input &input, const Camera &camera, EventHandler &events, World &world)
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

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(position.x, position.y, 0.0f);

    for (size_t layerIndex = 0; layerIndex < layerRotations.size(); ++layerIndex)
    {
        const float rotation = layerRotations[layerIndex];
        const glm::vec3 &baseColor = EXPLOSION_COLOR_ENDS[layerIndex];
        const float scaleFactor = radius / static_cast<float>(1 << layerIndex);

        glPushMatrix();
        glRotatef(rotation, 0.0f, 0.0f, 1.0f);
        glScalef(scaleFactor, scaleFactor, 1.0f);

        const glm::vec3 color = glm::mix(EXPLOSION_COLOR_START, baseColor, t);
        glColor3f(color.r, color.g, color.b);

        glBegin(GL_TRIANGLES);
        for (const auto &vertex : EXPLOSION_VERTICES)
        {
            glVertex2f(vertex.x, vertex.y);
        }
        glEnd();

        glPopMatrix();
    }

    glPopMatrix();
}
