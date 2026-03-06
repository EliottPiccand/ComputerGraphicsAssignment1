#include "Window.h"

#include <format>
#include <stdexcept>

void glfwErrorCallback(int code, const char *description)
{
    const std::string message = std::format("GLFW error ({}) : {}", code, description);
    throw std::runtime_error(message);
}

Window::Window(uint16_t width, uint16_t height, const char *title, PFN_ResizeCallback resizeCallback)
{
    glfwSetErrorCallback(glfwErrorCallback);
    glfwInit();

    handle = glfwCreateWindow(width, height, title, nullptr, nullptr);
    glfwMakeContextCurrent(handle);

    user_data = std::make_unique<CallbackData>(resizeCallback);
    glfwSetWindowUserPointer(handle, user_data.get());

    glfwSetFramebufferSizeCallback(handle, [](GLFWwindow *window, int width, int height) {
        const CallbackData *user_data = static_cast<CallbackData *>(glfwGetWindowUserPointer(window));
        user_data->resizeCallback(width, height);
    });
}

Window::~Window()
{
    glfwTerminate();
}

bool Window::shouldClose() const
{
    return glfwWindowShouldClose(handle) == GLFW_TRUE;
}

void Window::endFrame() const
{
    glfwSwapBuffers(handle);
    glfwPollEvents();
}

void Window::setTitle(std::string title) const
{
    glfwSetWindowTitle(handle, title.c_str());
}
