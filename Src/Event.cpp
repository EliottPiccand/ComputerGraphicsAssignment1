#include "Event.h"

using namespace event;

FireEvent::FireEvent(const glm::vec2 &start, const glm::vec2 &target) : start(start), target(target)
{
}

TargetReachedEvent::TargetReachedEvent(int entityId, const glm::vec2 &position) : entityId(entityId), position(position)
{
}

ExplosionDoneEvent::ExplosionDoneEvent(int entityId) : entityId(entityId)
{
}
