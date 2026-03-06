#pragma once

#include "Camera.h"
#include "EventHandler.h"
#include "Input.h"

class Entity
{
  public:
    int id;

    Entity(int id);

    virtual void update(float deltaTime, Input &input, Camera &camera, EventHandler &events) {};
    virtual void render() const {};
};
