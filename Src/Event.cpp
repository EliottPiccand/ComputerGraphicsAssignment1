#include "Event.h"

using namespace event;

FireEvent::FireEvent(glm::vec2 &start, glm::vec2 &target) : start(start), target(target)
{
}

TargetReachedEvent::TargetReachedEvent(int entityId) : entityId(entityId)
{
}
