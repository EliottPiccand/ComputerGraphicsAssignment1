#pragma once

#include <string>
#include <unordered_map>

#include "GL.h"
#include "Window.h"

class Input
{
  public:
    enum class Action
    {
        SpeedUp,
        SpeedDown,
        TurnLeft,
        TurnRight,
        Fire,
        CancelFire,
    };

    enum class State
    {
        JustPressed,
        HeldPressed,
        JustReleased,
        HeldReleased,
    };

  private:
    GLFWwindow *window_handle;
    std::unordered_map<Action, int> binds;
    std::unordered_map<Action, State> states;

  public:
    void initialize(const Window &window);

    void bindKey(Action action, int key);
    void bindMouseButton(Action action, int mouseButton);
    void update();

    [[nodiscard]] State operator[](Action action) const;
    [[nodiscard]] bool isPressed(Action action) const;
    [[nodiscard]] glm::vec2 getMousePos() const;
};
