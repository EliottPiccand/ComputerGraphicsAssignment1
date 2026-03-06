#include "Input.h"

#include <cassert>

constexpr const size_t MASK_OFFSET = sizeof(unsigned int) * 8 / 2;
constexpr const unsigned int MASK = (1 << MASK_OFFSET) - 1;

void Input::initialize(const Window &window)
{
    window_handle = window.handle;
}

void Input::bindKey(Action action, unsigned int key)
{
    key += 1;
    assert((key & MASK) == key);
    binds[action] = key;
    states[action] = State::HeldReleased;
}

void Input::bindMouseButton(Action action, unsigned int mouseButton)
{
    mouseButton += 1;
    assert((mouseButton & MASK) == mouseButton);
    binds[action] = mouseButton << MASK_OFFSET;
    states[action] = State::HeldReleased;
}

void Input::update()
{
    for (const auto &[action, key] : binds)
    {
        const auto glfw_state = ((key & (MASK << MASK_OFFSET)) == 0)
                                    ? glfwGetKey(window_handle, key - 1)
                                    : glfwGetMouseButton(window_handle, (key >> MASK_OFFSET) - 1);

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
