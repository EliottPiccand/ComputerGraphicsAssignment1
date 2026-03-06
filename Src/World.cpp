#include "World.h"

#include "GL.h"
#include "Utils/Constants.h"

void World::render()
{
    glColor3f(WATER_COLOR);
    glRectf(0.0f, 0.0f, WORLD_WIDTH, WORLD_HEIGHT);
}
