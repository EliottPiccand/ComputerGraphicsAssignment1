#include "Camera.h"

#include "GL.h"
#include "Utils/Constants.h"

Camera::Camera(uint32_t width, uint32_t height)
{
    onScreenResized(width, height);
}

void Camera::onScreenResized(uint32_t width, uint32_t height)
{
    constexpr float VIRTUAL_WORLD_WIDTH  = WORLD_WIDTH  + 2.0f * WORLD_DISPLAY_MARGIN;
    constexpr float VIRTUAL_WORLD_HEIGHT = WORLD_HEIGHT + 2.0f * WORLD_DISPLAY_MARGIN;

    constexpr float WORLD_ASPECT_RATIO = VIRTUAL_WORLD_WIDTH / VIRTUAL_WORLD_HEIGHT;

    float screen_width = static_cast<float>(width);
    float screen_height = static_cast<float>(height);
    float aspectRatio = screen_width / screen_height;

    if (aspectRatio > WORLD_ASPECT_RATIO) {
        top = -WORLD_DISPLAY_MARGIN;
        bottom = VIRTUAL_WORLD_HEIGHT - WORLD_DISPLAY_MARGIN;
    
        float world_display_width = VIRTUAL_WORLD_WIDTH * screen_height / VIRTUAL_WORLD_HEIGHT;
        float gap_display_width = (screen_width - world_display_width) / 2.0f;
        float gap_world_width = gap_display_width * VIRTUAL_WORLD_HEIGHT / screen_height;
    
        left = -gap_world_width - WORLD_DISPLAY_MARGIN;
        right = VIRTUAL_WORLD_WIDTH + gap_world_width - WORLD_DISPLAY_MARGIN;
    } else {
        left = -WORLD_DISPLAY_MARGIN;
        right = VIRTUAL_WORLD_WIDTH - WORLD_DISPLAY_MARGIN;

        float world_display_height = VIRTUAL_WORLD_HEIGHT * screen_width / VIRTUAL_WORLD_WIDTH;
        float gap_display_height = (screen_height - world_display_height) / 2.0f;
        float gap_world_height = gap_display_height * VIRTUAL_WORLD_WIDTH / screen_width;

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
