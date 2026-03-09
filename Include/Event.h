#pragma once

#include "GL.h"

namespace event
{

struct Event
{
    virtual ~Event() = default;
};

struct FireEvent : public Event
{
    glm::vec2 start;
    glm::vec2 target;

    FireEvent(glm::vec2 &start, glm::vec2 &target);
};

struct TargetReachedEvent : public Event
{
    int entityId;
    glm::vec2 position;

    TargetReachedEvent(int entityId, glm::vec2 position);
};

struct ExplosionDoneEvent : public Event
{
    int entityId;

    ExplosionDoneEvent(int entityId);
};

} // namespace event
