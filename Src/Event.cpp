#include "Event.h"

using namespace event;

FireEvent::FireEvent(glm::vec2 &start, glm::vec2 &target) : start(start), target(target)
{
}

TargetReachedEvent::TargetReachedEvent(int entityId, glm::vec2 position) : entityId(entityId), position(position)
{
}

ExplosionDoneEvent::ExplosionDoneEvent(int entityId) : entityId(entityId)
{
}
