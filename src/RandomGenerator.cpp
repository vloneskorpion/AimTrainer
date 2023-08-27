#include "RandomGenerator.hpp"

RandomGenerator::RandomGenerator() :
    m_Engine{std::chrono::system_clock::now().time_since_epoch().count()}
{
}

template<typename T>
std::vector<T> RandomGenerator::getRandomNumbers(int count, T min, T max)
{
    std::vector<T> randomNumbers{};
    randomNumbers.reserve(count);

    auto distribution = std::uniform_real_distribution<T>(min, max);

    std::generate_n(std::back_inserter(randomNumbers), count, [&](){ return distribution(m_Engine); });

    return randomNumbers;
}

// Explicit instantiations for the template function
template std::vector<float> RandomGenerator::getRandomNumbers<float>(int count, float min, float max);