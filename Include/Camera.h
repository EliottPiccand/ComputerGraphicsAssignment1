#pragma once

#include <cstdint>

class Camera
{
  private:
    float left;
    float right;
    float bottom;
    float top;

  public:
    Camera(uint32_t width, uint32_t height);
    void onScreenResized(uint32_t width, uint32_t height);
    void render() const;
};
