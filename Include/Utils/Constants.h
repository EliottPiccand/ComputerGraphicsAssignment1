#pragma once

#include <cstdint>

#define COLOR(r, g, b) r##.0f / 255.0f, g##.0f / 255.0f, b##.0f / 255.0f

constexpr const uint32_t DEFAULT_WINDOW_WIDTH = 1280;
constexpr const uint32_t DEFAULT_WINDOW_HEIGHT = 720;
#define BACKGROUND_COLOR COLOR(189, 220, 230)

constexpr const float WORLD_WIDTH = 1200.0f;
constexpr const float WORLD_HEIGHT = 1000.0f;
constexpr const float WORLD_DISPLAY_MARGIN = 100.0f;
#define WATER_COLOR COLOR(92, 174, 200)
