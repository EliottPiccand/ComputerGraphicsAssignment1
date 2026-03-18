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

std::pair<float, float> randomObstacleDimensions()
{
    float width = Random::random(MIN_OBSTACLE_WIDTH, MAX_OBSTACLE_WIDTH);
    float height = Random::random(MIN_OBSTACLE_DEPTH, MAX_OBSTACLE_DEPTH);

    while (std::fabs(width - height) < MIN_OBSTACLE_SIDE_DELTA)
    {
        height = Random::random(MIN_OBSTACLE_DEPTH, MAX_OBSTACLE_DEPTH);
    }

    return {width, height};
}
} // namespace

Obstacle::Obstacle(int entityId, glm::vec2 position)
    : Entity(entityId), position(position), orientation(Random::random(0.0f, 360.0f)), width(0.0f), height(0.0f)
{
    const auto [randomWidth, randomHeight] = randomObstacleDimensions();
    width = randomWidth;
    height = randomHeight;
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

float Obstacle::getHeight() const
{
    return height;
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
    const float halfHeight = height * 0.5f;

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(position.x, position.y, 0.0f);
    glRotatef(orientation, 0.0f, 0.0f, 1.0f);

    glColor3f(OBSTACLE_COLOR);
    glRectf(-halfWidth, -halfHeight, halfWidth, halfHeight);

    glPopMatrix();
}
