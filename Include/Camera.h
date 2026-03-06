#pragma once

#include <cstdint>

#include "GL.h"

class Camera
{
  private:
    float screenWidth;
    float screenHeight;

    float left;
    float right;
    float bottom;
    float top;

  public:
    Camera(uint32_t width, uint32_t height);

    void onScreenResized(uint32_t width, uint32_t height);
    void render() const;

    glm::vec2 toWorldPosition(const glm::vec2 &position) const;
};
