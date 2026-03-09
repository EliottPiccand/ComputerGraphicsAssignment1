#pragma once

#include <random>

class Random
{
  private:
    static inline std::random_device randomDevice;
    static inline std::mt19937 generator;

  public:
    static void initialize();
    static float random(float min, float max);
};
