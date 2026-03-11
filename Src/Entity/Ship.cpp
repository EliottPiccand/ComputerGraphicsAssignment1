#include "Entity/Ship.h"

#include "Event.h"
#include "GL.h"
#include "Utils/Constants.h"
#include "Utils/Math.h"
#include "World.h"

using namespace entity;

#define CHECK_ANGLE(angle)                                                                                             \
    while (angle > 360.0f)                                                                                             \
    {                                                                                                                  \
        angle -= 360.0f;                                                                                               \
    }                                                                                                                  \
    while (angle < 0.0f)                                                                                               \
    {                                                                                                                  \
        angle += 360.0f;                                                                                               \
    }

Ship::Ship(int entityId, glm::vec2 position, float orientation, Input &input)
    : Entity(entityId), position(position), orientation(orientation), lastTrailUpdate(now())
{
    CHECK_ANGLE(orientation);

    speedState = SpeedState::Stop;
    aiming = false;
    aimingValidPosition = false;
    targetPosition = {};

    input.bindKey(Input::Action::SpeedUp, GLFW_KEY_W);
    input.bindKey(Input::Action::TurnLeft, GLFW_KEY_A);
    input.bindKey(Input::Action::SpeedDown, GLFW_KEY_S);
    input.bindKey(Input::Action::TurnRight, GLFW_KEY_D);
    input.bindMouseButton(Input::Action::Fire, GLFW_MOUSE_BUTTON_LEFT);
    input.bindMouseButton(Input::Action::CancelFire, GLFW_MOUSE_BUTTON_RIGHT);
}

void Ship::update(float deltaTime, Input &input, const Camera &camera, EventHandler &events)
{
    bool boundingBoxUpdated = false;

    // Handle user inputs
    if (input[Input::Action::SpeedUp] == Input::State::JustPressed)
    {
        switch (speedState)
        {

        case SpeedState::Forward:
            break;
        case SpeedState::Stop:
            speedState = SpeedState::Forward;
            break;
        case SpeedState::Backward:
            speedState = SpeedState::Stop;
            break;
        }
    }

    if (input[Input::Action::SpeedDown] == Input::State::JustPressed)
    {
        switch (speedState)
        {

        case SpeedState::Forward:
            speedState = SpeedState::Stop;
            break;
        case SpeedState::Stop:
            speedState = SpeedState::Backward;
            break;
        case SpeedState::Backward:
            break;
        }
    }

    if (input[Input::Action::TurnLeft] == Input::State::JustPressed)
    {
        orientation -= 15.0f;
        CHECK_ANGLE(orientation);

        boundingBoxUpdated = true;
    }
    if (input[Input::Action::TurnRight] == Input::State::JustPressed)
    {
        orientation += 15.0f;
        CHECK_ANGLE(orientation);

        boundingBoxUpdated = true;
    }

    if (input[Input::Action::Fire] == Input::State::JustPressed)
    {
        aiming = true;
    }
    if (input[Input::Action::CancelFire] == Input::State::JustPressed)
    {
        aiming = false;
    }
    if (input[Input::Action::Fire] == Input::State::JustReleased && aiming)
    {
        aiming = false;
        if (aimingValidPosition)
        {
            events.post<event::FireEvent>(position, targetPosition);
        }
    }

    // Move
    const glm::vec2 direction = glm::rotate(glm::vec2(0.0f, -1.0f), glm::radians(orientation));

    switch (speedState)
    {
    case SpeedState::Forward:
        position += direction * deltaTime * SHIP_SPEED;
        boundingBoxUpdated = true;
        break;
    case SpeedState::Stop:
        break;
    case SpeedState::Backward:
        position -= direction * deltaTime * SHIP_SPEED;
        boundingBoxUpdated = true;
        break;
    }

    // Collisions
    if (boundingBoxUpdated)
    {
        World::checkCollision(position, orientation);
    }

    // Target
    if (aiming)
    {
        targetPosition = camera.toWorldPosition(input.getMousePos());
        glm::vec2 clampedTargetPosition = glm::clamp(targetPosition, {0.0f, 0.0f}, {WORLD_WIDTH, WORLD_HEIGHT});

        aimingValidPosition = targetPosition == clampedTargetPosition;
    }

    // Trail
    if (now() - lastTrailUpdate > TRAIL_UPDATE_INTERVAL)
    {
        lastTrailUpdate = now();
        if (trail.head() != position)
        {
            trail.push(position);
        }
    }
}

void Ship::render() const
{
    // Trail
    size_t i = 0;
    for (const auto &position : trail)
    {
        const float t = static_cast<float>(TRAIL_LENGTH - i) / static_cast<float>(TRAIL_LENGTH);
        glColor4f(TRAIL_COLOR, (1.0f - t) / 5.0f);
        glPointSize(lerp(TRAIL_MAX_SIZE, TRAIL_MIN_SIZE, t));
        glBegin(GL_POINTS);
        glVertex2f(position.x, position.y);
        glEnd();

        i += 1;
    }

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(position.x, position.y, 0.0f);
    glRotatef(orientation + 180.0f, 0.0f, 0.0f, 1.0f);
    glScalef(SHIP_SCALE.x, SHIP_SCALE.y, 1.0f);

    // Background
    glColor3f(SHIP_COLOR);
    glBegin(GL_POLYGON);
    for (const auto &vertex : SHIP_VERTICES)
    {
        glVertex2f(vertex.x, vertex.y);
    }
    glEnd();

    // Outline
    glColor3f(SHIP_OUTLINE_COLOR);
    glLineWidth(SHIP_OUTLINE_WIDTH);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBegin(GL_POLYGON);
    for (const auto &vertex : SHIP_VERTICES)
    {
        glVertex2f(vertex.x, vertex.y);
    }
    glEnd();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Turret
    glColor3f(SHIP_TURRET_COLOR);
    glBegin(GL_POLYGON);
    for (const auto &vertex : SHIP_TURRET_VERTICES)
    {
        glVertex2f(vertex.x, vertex.y);
    }
    glEnd();

    glPopMatrix();

    // Target
    if (aiming && aimingValidPosition)
    {
        glColor3f(1.0, 0.0, 0.0);
        glLineWidth(3.0);
        glBegin(GL_LINES);
        glVertex2f(position.x, position.y);
        glVertex2f(targetPosition.x, targetPosition.y);
        glEnd();
    }
}
