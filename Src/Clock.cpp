#include "Clock.h"

Clock::Clock(float targetFps) : inverseTargetFps(1.0f / targetFps), lastFrame(std::chrono::steady_clock::now())
{
}

float Clock::tick()
{
    while (true)
    {
        const std::chrono::duration<float> elapsed = std::chrono::steady_clock::now() - lastFrame;

        if (elapsed.count() > inverseTargetFps)
        {
            frameCount += 1;
            frameTimeSum += elapsed.count();

            if (frameCount >= FPS_SAMPLES)
            {
                fps = static_cast<float>(frameCount) / frameTimeSum;
                frameCount = 0;
                frameTimeSum = 0.0f;
            }

            lastFrame = std::chrono::steady_clock::now();
            return elapsed.count();
        }
    }
}

const std::optional<float> Clock::getFpsUpdate() const
{
    if (frameCount == 0)
    {
        return {fps};
    }
    return std::nullopt;
}
