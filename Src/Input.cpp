#include "Input.h"

#include <GLFW/glfw3.h>

void Input::initialize(const Window &window)
{
    window_handle = window.handle;
}

void Input::bind(Action action, int key)
{
    binds[action] = key;
    states[action] = State::HeldReleased;
}

void Input::update()
{
    for (const auto [action, key] : binds)
    {

        const auto glfw_state = glfwGetKey(window_handle, key);

        if (glfw_state == GLFW_PRESS)
        {
            switch (states[action])
            {

            case State::JustPressed:
            case State::HeldPressed:
                states[action] = State::HeldPressed;
                break;
            case State::JustReleased:
            case State::HeldReleased:
                states[action] = State::JustPressed;
                break;
            }
        }
        else // glfw_state == GLFW_RELEASE
        {
            switch (states[action])
            {

            case State::JustPressed:
            case State::HeldPressed:
                states[action] = State::JustReleased;
                break;
            case State::JustReleased:
            case State::HeldReleased:
                states[action] = State::HeldReleased;
                break;
            }
        }
    }
}

Input::State Input::operator[](Action action) const
{
    return states.at(action);
}

bool Input::isPressed(Action action) const
{
    return states.at(action) == State::JustPressed || states.at(action) == State::HeldPressed;
}
