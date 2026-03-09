#include "Application.h"

#include <format>

#include "Event.h"
#include "Missile.h"
#include "Ship.h"
#include "Utils/Constants.h"
#include "Utils/Random.h"

Application::Application() : lastFpsUpdate(now()), camera(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT)
{
    Random::initialize();
    window = std::make_unique<Window>(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, WINDOW_TITLE,
                                      [this](uint32_t width, uint32_t height) { onResize(width, height); });
    input.initialize(*window);

    nextEntityId = 0;
    entities.push_back(std::make_shared<Ship>(nextEntityId, SHIP_DEFAULT_POSITION, SHIP_DEFAULT_ORIENTATION, input));
    nextEntityId += 1;
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

    // Event handleing
    for (const auto &rawEvent : events.popAll())
    {
        if (auto *event = dynamic_cast<event::FireEvent *>(rawEvent.get()))
        {
            entities.push_back(std::make_shared<Missile>(nextEntityId, event->start, event->target));
            nextEntityId += 1;
        }
        else if (auto *event = dynamic_cast<event::TargetReachedEvent *>(rawEvent.get()))
        {
            auto it =
                std::find_if(entities.begin(), entities.end(), [&event](auto e) { return e->id == event->entityId; });

            if (it != entities.end())
            {
                entities.erase(it);
                camera.shake();
            }
        }
    }

    camera.update();

    // Update entities
    for (auto &entity : entities)
    {
        entity->update(deltaTime, input, camera, events);
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
