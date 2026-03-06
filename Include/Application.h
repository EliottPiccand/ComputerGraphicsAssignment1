#pragma once

#include <memory>

#include "Camera.h"
#include "Clock.h"
#include "GL.h"
#include "Input.h"
#include "Window.h"
#include "World.h"

class Application
{
  private:
    Clock clock;
    std::unique_ptr<Window> window;
    Input input;

    Camera camera;

    World world;

    void update(float deltaTime);
    void render();

    void onResize(uint32_t width, uint32_t height);

  public:
    Application();
    void run();
};
