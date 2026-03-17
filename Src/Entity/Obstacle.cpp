#include "Entity/Obstacle.h"

#include <cmath>

#include "Utils/Random.h"

using namespace entity;

namespace
{
constexpr float MIN_OBSTACLE_WIDTH = 60.0f;
constexpr float MAX_OBSTACLE_WIDTH = 180.0f;
constexpr float MIN_OBSTACLE_DEPTH = 30.0f;
constexpr float MAX_OBSTACLE_DEPTH = 90.0f;
constexpr float MIN_OBSTACLE_SIDE_DELTA = 8.0f;

constexpr float OBSTACLE_R = 139.0f / 255.0f;
constexpr float OBSTACLE_G = 69.0f / 255.0f;
constexpr float OBSTACLE_B = 19.0f / 255.0f;

std::pair<float, float> randomObstacleDimensions()
{
    float randomWidth = Random::random(MIN_OBSTACLE_WIDTH, MAX_OBSTACLE_WIDTH);
    float randomDepth = Random::random(MIN_OBSTACLE_DEPTH, MAX_OBSTACLE_DEPTH);

    while (std::fabs(randomWidth - randomDepth) < MIN_OBSTACLE_SIDE_DELTA)
    {
        randomDepth = Random::random(MIN_OBSTACLE_DEPTH, MAX_OBSTACLE_DEPTH);
    }

    return {randomWidth, randomDepth};
}
} // namespace

Obstacle::Obstacle(int entityId, glm::vec2 position)
    : Entity(entityId),
      position(position),
      orientation(Random::random(0.0f, 360.0f)),
            width(0.0f),
            depth(0.0f)
{
        const auto [randomWidth, randomDepth] = randomObstacleDimensions();
        width = randomWidth;
        depth = randomDepth;
}

const glm::vec2 &Obstacle::getPosition() const
{
    return position;
}

float Obstacle::getOrientation() const
{
    return orientation;
}

float Obstacle::getWidth() const
{
    return width;
}

float Obstacle::getDepth() const
{
    return depth;
}

void Obstacle::update(float deltaTime, Input &input, const Camera &camera, EventHandler &events, World &world)
{
    (void)deltaTime;
    (void)input;
    (void)camera;
    (void)events;
    (void)world;
}

void Obstacle::render() const
{
    const float halfWidth = width * 0.5f;
    const float halfDepth = depth * 0.5f;

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(position.x, position.y, 0.0f);
    glRotatef(orientation, 0.0f, 0.0f, 1.0f);

    glColor3f(OBSTACLE_R, OBSTACLE_G, OBSTACLE_B);
    glRectf(-halfWidth, -halfDepth, halfWidth, halfDepth);

    glPopMatrix();
}
