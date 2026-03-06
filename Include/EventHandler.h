#pragma once

#include <concepts>
#include <memory>
#include <vector>

#include "Event.h"

class EventHandler
{
  private:
    std::vector<std::unique_ptr<event::Event>> events;

  public:
    std::vector<std::unique_ptr<event::Event>> popAll();

    template <std::derived_from<event::Event> EventType, typename... Args> void post(Args... args)
    {
        events.push_back(std::make_unique<EventType>(args...));
    }
};
