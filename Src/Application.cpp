#include "Application.h"

#include <format>

#include "Entity/Explosion.h"
#include "Entity/Missile.h"
#include "Entity/Ship.h"
#include "Event.h"
#include "Utils/Constants.h"
#include "Utils/Random.h"

Application::Application() : lastFpsUpdate(now()), camera(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT)
{
    Random::initialize();
    window = std::make_unique<Window>(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, WINDOW_TITLE,
                                      [this](uint32_t width, uint32_t height) { onResize(width, height); });
    input.initialize(*window);

    nextEntityId = 0;
    newEntity<entity::Ship>(SHIP_DEFAULT_POSITION, SHIP_DEFAULT_ORIENTATION, input);
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

bool Application::deleteEntity(int entityId)
{
    auto it = std::find_if(entities.begin(), entities.end(), [entityId](auto e) { return e->id == entityId; });

    if (it != entities.end())
    {
        entities.erase(it);
        return true;
    }
    return false;
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
            newEntity<entity::Missile>(event->start, event->target);
        }
        else if (auto *event = dynamic_cast<event::TargetReachedEvent *>(rawEvent.get()))
        {
            if (deleteEntity(event->entityId))
            {
                newEntity<entity::Explosion>(event->position);
                camera.shake();
            }
        }
        else if (auto *event = dynamic_cast<event::ExplosionDoneEvent *>(rawEvent.get()))
        {
            deleteEntity(event->entityId);
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
