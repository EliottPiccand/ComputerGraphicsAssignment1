#include "EventHandler.h"

#include <utility>

std::vector<std::unique_ptr<event::Event>> EventHandler::popAll()
{
    return std::move(events);
}
