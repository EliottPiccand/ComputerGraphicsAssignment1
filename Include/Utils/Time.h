#pragma once

#include <chrono>
#include <cstdint>
#include <ratio>

using Duration = std::chrono::duration<uint64_t, std::milli>;
