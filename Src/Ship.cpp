#include "Ship.h"

#include "GL.h"
#include "Utils/Constants.h"

Ship::Ship(glm::vec2 position, float orientation, Input &input) : position(position), orientation(orientation)
{
}

void Ship::update(float deltaTime, Input &input)
{
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

    // Turet
    glColor3f(SHIP_TURET_COLOR);
    glBegin(GL_POLYGON);
    for (const auto &vertex : SHIP_TURET_VERTICES)
    {
        glVertex2f(vertex.x, vertex.y);
    }
    glEnd();

    glPopMatrix();
}
