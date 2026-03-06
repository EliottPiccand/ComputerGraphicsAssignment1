#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <string>

#include "GL.h"

class Input;

class Window
{
  public:
    using PFN_ResizeCallback = std::function<void(uint32_t, uint32_t)>;

  private:
    struct CallbackData
    {
        PFN_ResizeCallback resizeCallback;
    };

    friend Input;

    GLFWwindow *handle;
    std::unique_ptr<CallbackData> user_data;

  public:
    Window(uint16_t width, uint16_t height, const char *title, PFN_ResizeCallback resizeCallback);
    ~Window();

    [[nodiscard]] bool shouldClose() const;
    void endFrame() const;

    void setTitle(std::string title) const;
};
