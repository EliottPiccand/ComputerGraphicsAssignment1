#pragma once

#include "Input.h"

class Entity
{
  public:
    virtual void update(float deltaTime, Input &input) {};
    virtual void render() const {};
};
