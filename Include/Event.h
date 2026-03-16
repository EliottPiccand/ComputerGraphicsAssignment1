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
    const glm::vec2 start;
    const glm::vec2 target;

    FireEvent(const glm::vec2 &start, const glm::vec2 &target);
};

struct TargetReachedEvent : public Event
{
    const int entityId;
    const glm::vec2 position;

    TargetReachedEvent(int entityId, const glm::vec2 &position);
};

struct ExplosionDoneEvent : public Event
{
    const int entityId;

    ExplosionDoneEvent(int entityId);
};

} // namespace event
