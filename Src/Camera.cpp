#include "Camera.h"

#include "Utils/Constants.h"

Camera::Camera(uint32_t width, uint32_t height)
{
    onScreenResized(width, height);
}

void Camera::onScreenResized(uint32_t width, uint32_t height)
{
    constexpr float VIRTUAL_WORLD_WIDTH = WORLD_WIDTH + 2.0f * WORLD_DISPLAY_MARGIN;
    constexpr float VIRTUAL_WORLD_HEIGHT = WORLD_HEIGHT + 2.0f * WORLD_DISPLAY_MARGIN;

    constexpr float WORLD_ASPECT_RATIO = VIRTUAL_WORLD_WIDTH / VIRTUAL_WORLD_HEIGHT;

    screenWidth = static_cast<float>(width);
    screenHeight = static_cast<float>(height);
    const float aspectRatio = screenWidth / screenHeight;

    if (aspectRatio > WORLD_ASPECT_RATIO)
    {
        top = -WORLD_DISPLAY_MARGIN;
        bottom = VIRTUAL_WORLD_HEIGHT - WORLD_DISPLAY_MARGIN;

        const float world_display_width = VIRTUAL_WORLD_WIDTH * screenHeight / VIRTUAL_WORLD_HEIGHT;
        const float gap_display_width = (screenWidth - world_display_width) / 2.0f;
        const float gap_world_width = gap_display_width * VIRTUAL_WORLD_HEIGHT / screenHeight;

        left = -gap_world_width - WORLD_DISPLAY_MARGIN;
        right = VIRTUAL_WORLD_WIDTH + gap_world_width - WORLD_DISPLAY_MARGIN;
    }
    else
    {
        left = -WORLD_DISPLAY_MARGIN;
        right = VIRTUAL_WORLD_WIDTH - WORLD_DISPLAY_MARGIN;

        const float world_display_height = VIRTUAL_WORLD_HEIGHT * screenWidth / VIRTUAL_WORLD_WIDTH;
        const float gap_display_height = (screenHeight - world_display_height) / 2.0f;
        const float gap_world_height = gap_display_height * VIRTUAL_WORLD_WIDTH / screenWidth;

        top = -gap_world_height - WORLD_DISPLAY_MARGIN;
        bottom = VIRTUAL_WORLD_HEIGHT + gap_world_height - WORLD_DISPLAY_MARGIN;
    }
}

void Camera::render() const
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(left, right, bottom, top, -1, 1);
}

glm::vec2 Camera::toWorldPosition(const glm::vec2 &position) const
{
    const glm::vec2 normalizedPosition =
        glm::vec2(position.x, screenHeight - position.y) / glm::vec2(screenWidth, screenHeight) * 2.0f - 1.0f;
    const glm::vec4 worldPosition = glm::inverse(glm::ortho(left, right, bottom, top)) *
                                    glm::vec4(normalizedPosition.x, normalizedPosition.y, 0.0, 1.0);
    return {worldPosition.x, worldPosition.y};
}
