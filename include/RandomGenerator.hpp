#pragma once

#include <random>
#include <chrono>

class RandomGenerator
{
    public:
        RandomGenerator();

        template<typename T>
        std::vector<T> getRandomNumbers(int count, T min, T max);

    private:
        std::mt19937_64 m_Engine;
};