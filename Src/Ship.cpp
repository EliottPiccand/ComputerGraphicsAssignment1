#include "Ship.h"

#include "GL.h"
#include "Input.h"
#include "Utils/Constants.h"

Ship::Ship(glm::vec2 position, float orientation, Input &input) : position(position), orientation(orientation)
{
    speedState = SpeedState::Stop;

    input.bind(Input::Action::SpeedUp, GLFW_KEY_W);
    input.bind(Input::Action::TurnLeft, GLFW_KEY_A);
    input.bind(Input::Action::SpeedDown, GLFW_KEY_S);
    input.bind(Input::Action::TurnRight, GLFW_KEY_D);
}

#define CHECK_ANGLE(angle)                                                                                             \
    while (angle > 360.0f)                                                                                             \
    {                                                                                                                  \
        angle -= 360.0f;                                                                                               \
    }                                                                                                                  \
    while (angle < 0.0f)                                                                                               \
    {                                                                                                                  \
        angle += 360.0f;                                                                                               \
    }

void Ship::update(float deltaTime, Input &input)
{
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
    }
    if (input[Input::Action::TurnRight] == Input::State::JustPressed)
    {
        orientation += 15.0f;
        CHECK_ANGLE(orientation);
    }

    // Move
    const glm::vec2 direction = glm::rotate(glm::vec2(0.0f, -1.0f), glm::radians(orientation));

    switch (speedState)
    {
    case SpeedState::Forward:
        position += direction * deltaTime * SHIP_SPEED;
        break;
    case SpeedState::Stop:
        break;
    case SpeedState::Backward:
        position -= direction * deltaTime * SHIP_SPEED;
        break;
    }
}

void Ship::render() const
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(position.x, position.y, 0.0f);
    glRotatef(orientation + 180.0f, 0.0f, 0.0f, 1.0f);
    glScalef(100.0f, 200.0f, 1.0f);

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
}
