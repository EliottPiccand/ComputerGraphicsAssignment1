#pragma once

#include <chrono>
#include <cstddef>
#include <optional>

class Clock
{
  private:
    float inverseTargetFps;
    std::chrono::time_point<std::chrono::steady_clock> lastFrame;

    float fps = 0.0f;

    static constexpr size_t FPS_SAMPLES = 240;
    float frameTimeSum = 0.0f;
    size_t frameCount = 0;

  public:
    Clock(float targetFps);

    float tick();
    [[nodiscard]] const std::optional<float> getFpsUpdate() const;
};
