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

    TargetReachedEvent(int entityId);
};

} // namespace event
