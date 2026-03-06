#include "Input.h"

#include <cassert>

void Input::initialize(const Window &window)
{
    window_handle = window.handle;
}

void Input::bindKey(Action action, int key)
{
    key += 1;
    assert((key & 0xFF'FF'FF'FF) == key);
    binds[action] = key;
    states[action] = State::HeldReleased;
}

void Input::bindMouseButton(Action action, int mouseButton)
{
    mouseButton += 1;
    assert((mouseButton & 0xFF'FF'FF'FF) == mouseButton);
    binds[action] = mouseButton << 16;
    states[action] = State::HeldReleased;
}

void Input::update()
{
    for (const auto [action, key] : binds)
    {
        const auto glfw_state = ((key & ((0xFF'FF'FF'FF) << 16)) == 0)
                                    ? glfwGetKey(window_handle, key - 1)
                                    : glfwGetMouseButton(window_handle, (key >> 16) - 1);

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

glm::vec2 Input::getMousePos() const
{
    double xpos, ypos;
    glfwGetCursorPos(window_handle, &xpos, &ypos);
    return {static_cast<float>(xpos), static_cast<float>(ypos)};
}
