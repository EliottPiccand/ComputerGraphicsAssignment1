#pragma once

#include <chrono>
#include <cstdint>
#include <ratio>

using Duration = std::chrono::nanoseconds;

using Instant = std::chrono::time_point<std::chrono::steady_clock>;
Instant now();
