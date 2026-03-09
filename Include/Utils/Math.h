#pragma once

template <typename T> T lerp(const T &start, const T &end, float t)
{
    return start + t * (end - start);
}
