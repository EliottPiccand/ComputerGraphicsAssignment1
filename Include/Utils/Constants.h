#pragma once

#include <array>
#include <chrono>
#include <cstdint>

#include "GL.h"
#include "Utils/Time.h"

#define COLOR(r, g, b) r##.0f / 255.0f, g##.0f / 255.0f, b##.0f / 255.0f

constexpr const char *WINDOW_TITLE = "Window Title";
constexpr const uint32_t DEFAULT_WINDOW_WIDTH = 1280;
constexpr const uint32_t DEFAULT_WINDOW_HEIGHT = 720;
constexpr const Duration FPS_UPDATE_INTERVAL = Duration(std::chrono::seconds(2));
#define BACKGROUND_COLOR COLOR(189, 220, 230)

constexpr const float WORLD_WIDTH = 1000.0f;
constexpr const float WORLD_HEIGHT = 1000.0f;
constexpr const float WORLD_DISPLAY_MARGIN = 100.0f;
#define WATER_COLOR COLOR(92, 174, 200)

constexpr const glm::vec2 SHIP_DEFAULT_POSITION = { WORLD_WIDTH / 2.0f, WORLD_HEIGHT / 2.0f };
constexpr const float SHIP_DEFAULT_ORIENTATION = 45.0f; // °
constexpr const GLfloat SHIP_OUTLINE_WIDTH = 5.0f;
#define SHIP_COLOR COLOR(164, 164, 164)
#define SHIP_OUTLINE_COLOR COLOR(92, 92, 92)
constexpr const std::array SHIP_VERTICES = std::array{
    glm::vec2(-0.45f, -0.5f), // Bottom-left
    glm::vec2( 0.45f, -0.5f), // Bottom-right
    glm::vec2( 0.5f ,  0.2f), // Top-right
    glm::vec2( 0.0f ,  0.6f), // Top
    glm::vec2(-0.5f ,  0.2f), // Top-left
};
constexpr const glm::vec2 SHIP_SCALE = { 100.0f, 200.0f };
constexpr const float SHIP_SPEED = 100.0f;

#define SHIP_TURRET_COLOR COLOR(70, 129, 80)
constexpr const std::array SHIP_TURRET_VERTICES = std::array{
    glm::vec2(-0.25f, -0.1f), // Bottom-left
    glm::vec2( 0.25f, -0.1f), // Bottom-right
    glm::vec2( 0.0f ,  0.2f), // Top
};

constexpr const float MISSILE_SPEED = 150.0f;
constexpr const float MISSILE_TARGET_ERROR_MARGIN = 10.0f;
