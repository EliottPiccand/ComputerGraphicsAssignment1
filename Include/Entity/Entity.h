#pragma once

#include "Camera.h"
#include "EventHandler.h"
#include "Input.h"
#include "World.h"

namespace entity
{

class Entity
{
  public:
    const int id;

    Entity(int id);

    virtual void update(float deltaTime, Input &input, const Camera &camera, EventHandler &events, World &world) {};
    virtual void render() const {};
};

} // namespace entity
