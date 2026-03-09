#pragma once

#include <concepts>
#include <memory>
#include <utility>
#include <vector>

#include "Camera.h"
#include "Clock.h"
#include "Entity/Entity.h"
#include "EventHandler.h"
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
    EventHandler events;

    Camera camera;

    int nextEntityId;
    std::vector<std::shared_ptr<entity::Entity>> entities;

    void update(float deltaTime);
    void render() const;

    void onResize(uint32_t width, uint32_t height);

    template <std::derived_from<entity::Entity> EntityType, typename... Args>
    std::shared_ptr<EntityType> newEntity(Args &&...args)
    {
        auto entity = std::make_shared<EntityType>(nextEntityId, std::forward<Args>(args)...);
        entities.push_back(entity);
        nextEntityId += 1;
        return entity;
    }

    bool deleteEntity(int entityId);

  public:
    Application();
    void run();
};
