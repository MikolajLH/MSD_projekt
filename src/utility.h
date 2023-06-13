#pragma once
#include <random>
#include <concepts>


template<std::floating_point Float>
auto random(Float min = Float(0), Float max = Float(1))noexcept
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<Float> dist(min, max);
    return dist(gen);
}


template<std::integral Int>
auto random(Int min, Int max) noexcept
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<Int> dist(min, max);
    return dist(gen);
}
