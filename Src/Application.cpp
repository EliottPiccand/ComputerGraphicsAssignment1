#include "Application.h"

#include <format>
#include <ranges>

#include "Entity/Explosion.h"
#include "Entity/Missile.h"
#include "Entity/Obstacle.h"
#include "Entity/Ship.h"
#include "Event.h"
#include "Input.h"
#include "Utils/Constants.h"
#include "Utils/Random.h"

Application::Application() : lastFpsUpdate(now()), camera(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT)
{
    Random::initialize();
    window = std::make_unique<Window>(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, WINDOW_TITLE,
                                      [this](uint32_t width, uint32_t height) { onResize(width, height); });
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    input.initialize(*window);
    input.bindKey(Input::Action::ToggleFullScreen, GLFW_KEY_F11);

    nextEntityId = 0;
    newEntity<entity::Ship>(SHIP_DEFAULT_POSITION, SHIP_DEFAULT_ORIENTATION, input);

    // Obstacles
    const int obstacleCount = Random::randint(1, 3);
    for (auto &&_ : std::views::iota(0, obstacleCount))
    {
        bool spawned = false;
        for (auto &&_ : std::views::iota(0uz, MAX_OBSTACLE_SPAWN_ATTEMPTS))
        {
            const glm::vec2 obstaclePosition{
                Random::random(OBSTACLE_SPAWN_MARGIN, WORLD_WIDTH - OBSTACLE_SPAWN_MARGIN),
                Random::random(OBSTACLE_SPAWN_MARGIN, WORLD_HEIGHT - OBSTACLE_SPAWN_MARGIN),
            };

            if (glm::length(obstaclePosition - SHIP_DEFAULT_POSITION) < OBSTACLE_CENTER_EXCLUSION_RADIUS)
            {
                continue;
            }

            auto obstacle = std::make_shared<entity::Obstacle>(nextEntityId, obstaclePosition);
            if (!world.canPlaceObstacle(obstacle))
            {
                continue;
            }

            entities.push_back(obstacle);
            nextEntityId += 1;
            world.addObstacle(obstacle);
            spawned = true;
            break;
        }

        if (!spawned)
        {
            break;
        }
    }
}

void Application::run()
{
    while (!window->shouldClose())
    {
        const float deltaTime = clock.tick();
        if (deltaTime > 1.0f)
        {
            continue;
        }

        input.update();
        update(deltaTime);
        render();

        window->endFrame();
    }
}

bool Application::deleteEntity(int entityId)
{
    const auto it = std::find_if(entities.begin(), entities.end(), [entityId](auto e) { return e->id == entityId; });

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

    if (input[Input::Action::ToggleFullScreen] == Input::State::JustReleased)
    {
        window->toggleFullscreen();
    }

    camera.update();

    world.update(deltaTime);

    // Update entities
    for (auto &entity : entities)
    {
        entity->update(deltaTime, input, camera, events, world);
    }
}

void Application::render() const
{
    glClearColor(BACKGROUND_COLOR, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    camera.render();
    world.render();

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
