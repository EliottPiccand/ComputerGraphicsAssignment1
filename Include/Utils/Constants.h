#pragma once

#include <array>
#include <chrono>
#include <cstdint>
#include <ratio>

#include "GL.h"
#include "Utils/Time.h"

#define rgba(r, g, b, a) r##.0f / 255.0f, g##.0f / 255.0f, b##.0f / 255.0f

constexpr const float EPSILON = 1e-5f;

// **************
// *   Window   *
// **************

constexpr const char *WINDOW_TITLE = "Computer Graphics Assignment #1";
constexpr const uint32_t DEFAULT_WINDOW_WIDTH = 1280; // m
constexpr const uint32_t DEFAULT_WINDOW_HEIGHT = 720; // m
constexpr const Duration FPS_UPDATE_INTERVAL = Duration(std::chrono::seconds(2));
#define BACKGROUND_COLOR rgba(189, 220, 230, 1)


// *************
// *   World   *
// *************

constexpr const float WORLD_WIDTH = 1000.0f; // m
constexpr const float WORLD_HEIGHT = 1000.0f; // m
constexpr const float WORLD_DISPLAY_MARGIN = 100.0f; // m

constexpr const glm::vec3 WATER_COLOR = {rgba(84, 176, 204, 1)};
constexpr const float WORLD_SUBDIVISION_SIZE = 8.0f; // m
constexpr float WATER_WAVES_SPEED = 100.0f; // m/s
constexpr float WATER_WAVES_DAMPING = 1.2f;

static_assert(static_cast<int>(WORLD_WIDTH)  % static_cast<int>(WORLD_SUBDIVISION_SIZE) == 0, "WORLD_WIDTH must be an integer multiple of WORLD_SUBDIVISION_SIZE");
static_assert(static_cast<int>(WORLD_HEIGHT) % static_cast<int>(WORLD_SUBDIVISION_SIZE) == 0, "WORLD_HEIGHT must be an integer multiple of WORLD_SUBDIVISION_SIZE");

// ************
// *   Ship   *
// ************

constexpr const glm::vec2 SHIP_DEFAULT_POSITION = { WORLD_WIDTH / 2.0f, WORLD_HEIGHT / 2.0f };
constexpr const float SHIP_DEFAULT_ORIENTATION = 45.0f; // °
constexpr const GLfloat SHIP_OUTLINE_WIDTH = 5.0f;
#define SHIP_COLOR rgba(164, 164, 164, 1)
#define SHIP_OUTLINE_COLOR rgba(92, 92, 92, 1)
constexpr const std::array SHIP_VERTICES = std::array{
    glm::vec2(-0.45f, -0.5f), // Bottom-left
    glm::vec2( 0.45f, -0.5f), // Bottom-right
    glm::vec2( 0.5f ,  0.2f), // Top-right
    glm::vec2( 0.0f ,  0.6f), // Top
    glm::vec2(-0.5f ,  0.2f), // Top-left
};
constexpr const glm::vec2 SHIP_SCALE = { 100.0f, 200.0f };
constexpr const float SHIP_SPEED = 100.0f; // m/s

#define SHIP_TURRET_COLOR rgba(70, 129, 80, 1)
constexpr const std::array SHIP_TURRET_VERTICES = std::array{
    glm::vec2(-0.25f, -0.1f), // Bottom-left
    glm::vec2( 0.25f, -0.1f), // Bottom-right
    glm::vec2( 0.0f ,  0.2f), // Top
};

#define AIM_RAY_COLOR rgba(255, 0, 0, 1)
constexpr const GLfloat AIM_RAY_WIDTH = 3.0f;

// ***********************
// *   Ship Foam Trail   *
// ***********************

constexpr const size_t MAX_TRAIL_PARTICLES = 100;
#define FOAM_COLOR rgba(255, 255, 255, 1)
constexpr const float TRAIL_MAX_SIZE = 100.0f; // m
constexpr const float TRAIL_MIN_SIZE = 20.0f; // m
constexpr const float MIN_TRAIL_STEP = 10.0f; // m
constexpr const float TRAIL_PARTICLE_INTENTISY_DECAY = 0.2f; // alpha[0;1] / s

// ****************
// *   Missiles   *
// ****************

constexpr const float MISSILE_SPEED = 450.0f; // m/s
constexpr const float MISSILE_TARGET_ERROR_MARGIN = 10.0f; // m
constexpr const float MISSILE_SIZE = 10.0f; // m
#define MISSILE_COLOR rgba(255, 255, 0, 1)

#define MISSILE_RAY_COLOR rgba(255, 0, 255, 1)
constexpr const GLfloat MISSILE_RAY_WIDTH = 2.0f;

// ********************
// *   Camera Shake   *
// ********************

constexpr const float SHAKING_INTENSITY = 10.0f; // m
constexpr const float SHAKING_DECAY_FACTOR = 0.9f;
constexpr const float SHAKING_SPREAD_ANGLE = 60.0f; // °
constexpr const Duration SHAKING_UPDATE_INTERVAL = Duration(std::chrono::milliseconds(20));

// ******************
// *   Explosions   *
// ******************

constexpr const float EXPLOSION_MAX_RADIUS = 100.0f; // m
constexpr const float EXPLOSION_EXPANSION_RATE = 300.0f; // m/s
constexpr const glm::vec3 EXPLOSION_COLOR_START = {rgba(242, 248, 100, 1)};

constexpr const std::array EXPLOSION_COLOR_ENDS = {
    glm::vec3{rgba(182, 45, 12, 1)},
    glm::vec3{rgba(208, 98, 34, 1)},
    glm::vec3{rgba(215, 164, 55, 1)},
};

constexpr const std::array EXPLOSION_VERTICES = []{
    constexpr const glm::vec2 CANVAS_SIZE{100.0f, 100.0f};

    constexpr const glm::vec2 V1  {41.0f, 2.0f};
    constexpr const glm::vec2 V2  {37.0f, 23.0f};
    constexpr const glm::vec2 V3  {52.0f, 22.0f};
    constexpr const glm::vec2 V4  {34.0f, 64.0f};
    constexpr const glm::vec2 V5  {73.0f, 55.0f};
    constexpr const glm::vec2 V6  {71.0f, 31.0f};
    constexpr const glm::vec2 V7  {72.0f, 14.0f};
    constexpr const glm::vec2 V8  {92.0f, 37.0f};
    constexpr const glm::vec2 V9  {59.0f, 73.0f};
    constexpr const glm::vec2 V10 {79.0f, 66.0f};
    constexpr const glm::vec2 V11 {52.0f, 84.0f};
    constexpr const glm::vec2 V12 {26.0f, 74.0f};
    constexpr const glm::vec2 V13 {29.0f, 60.0f};
    constexpr const glm::vec2 V14 {20.0f, 47.0f};
    constexpr const glm::vec2 V15 {15.0f, 63.0f};
    constexpr const glm::vec2 V16 {23.0f, 20.0f};
    constexpr const glm::vec2 V17 {12.0f, 38.0f};

    auto vertices = std::array{
        V1,  V2,  V3,
        V2,  V3,  V4,
        V3,  V4,  V5,
        V3,  V5,  V6,
        V3,  V6,  V7,
        V5,  V6,  V8,
        V4,  V5,  V9,
        V5,  V9,  V10,
        V9,  V11, V12,
        V4,  V9,  V12,
        V2,  V4,  V13,
        V2,  V13, V14,
        V13, V14, V15,
        V2,  V14, V16,
        V14, V16, V17,
    };

    for (auto& vertex : vertices) {
        vertex -= CANVAS_SIZE / 2.0f;
        vertex /= CANVAS_SIZE / 2.0f;
    }

    return vertices;
}();

// *****************
// *   Obstacles   *
// *****************

#define OBSTACLE_COLOR rgba(139, 69, 19, 1)
constexpr float SHIP_OBSTACLE_CLEARANCE = 3.0f;
constexpr size_t OBSTACLE_VERTEX_COUNT = 4;
