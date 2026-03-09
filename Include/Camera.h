#pragma once

#include <cstdint>

#include "GL.h"
#include "Utils/Time.h"

class Camera
{
  private:
    float screenWidth;
    float screenHeight;

    float baseLeft;
    float baseRight;
    float baseBottom;
    float baseTop;

    glm::vec2 shakingOffset;
    Instant shakingLastUpdate;

  public:
    Camera(uint32_t width, uint32_t height);

    void onScreenResized(uint32_t width, uint32_t height);
    void update();
    void render() const;

    void shake();

    glm::vec2 toWorldPosition(const glm::vec2 &position) const;
};
