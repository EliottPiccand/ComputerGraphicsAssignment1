#include "Application.h"

#include <format>
#include <memory>

#include "GL.h"
#include "Input.h"
#include "Utils/Constants.h"

constexpr const char *WINDOW_TITLE = "Window Title";

Application::Application() : clock(120.0f), camera(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT)
{
    window = std::make_unique<Window>(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, WINDOW_TITLE,
                                      [this](uint32_t width, uint32_t height) { onResize(width, height); });
    input.initialize(*window);
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
    if (auto fps = clock.getFpsUpdate()) {
        std::string title = std::format("{} | {} FPS", WINDOW_TITLE, std::roundf(*fps));
        window->setTitle(title);
    }

}

void Application::render()
{
    glClearColor(BACKGROUND_COLOR, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    camera.render();
    world.render();

}

void Application::onResize(uint32_t width, uint32_t height)
{
    glViewport(0, 0, width, height);
    camera.onScreenResized(width, height);
}
