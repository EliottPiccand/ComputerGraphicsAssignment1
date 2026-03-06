#pragma once

#include <memory>
#include <vector>

#include "Camera.h"
#include "Clock.h"
#include "Entity.h"
#include "GL.h"
#include "Input.h"
#include "Utils/Time.h"
#include "Window.h"
#include "World.h"

class Application
{
  private:
    Clock clock;
    Instant lastFpsUpdate;

    std::unique_ptr<Window> window;
    Input input;

    Camera camera;

    std::vector<std::shared_ptr<Entity>> entities;

    void update(float deltaTime);
    void render() const;

    void onResize(uint32_t width, uint32_t height);

  public:
    Application();
    void run();
};
