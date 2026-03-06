#include "Application.h"

#include <format>
#include <memory>

#include "GL.h"
#include "Input.h"
#include "Ship.h"
#include "Utils/Constants.h"
#include "Utils/Time.h"

Application::Application() : lastFpsUpdate(now()), camera(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT)
{
    window = std::make_unique<Window>(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, WINDOW_TITLE,
                                      [this](uint32_t width, uint32_t height) { onResize(width, height); });
    input.initialize(*window);

    entities.push_back(std::make_shared<Ship>(SHIP_DEFAULT_POSITION, SHIP_DEFAULT_ORIENTATION, input));
}

void Application::run()
{
    while (!window->shouldClose())
    {
        float deltaTime = clock.tick();

        input.update();
        update(deltaTime);
        render();

        window->endFrame();
    }
}

void Application::update(float deltaTime)
{
    // Window title update
    if (now() - lastFpsUpdate > FPS_UPDATE_INTERVAL)
    {
        lastFpsUpdate = now();

        const float fps = clock.getFps();
        const std::string title = std::format("{} | {} FPS", WINDOW_TITLE, std::roundf(fps));
        window->setTitle(title);
    }

    // Update ship
    for (auto &entity : entities)
    {
        entity->update(deltaTime, input);
    }
}

void Application::render() const
{
    glClearColor(BACKGROUND_COLOR, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    camera.render();
    World::render();

    for (const auto &entity : entities)
    {
        entity->render();
    }
}

void Application::onResize(uint32_t width, uint32_t height)
{
    glViewport(0, 0, width, height);
    camera.onScreenResized(width, height);
}
