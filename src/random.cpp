#include <chrono>
#include <gtest/gtest.h>
#include <iostream>
#include <random>

int dice()
{
    static std::random_device rd;  // Will be used to obtain a seed for the random number engine
    static std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
    static std::uniform_int_distribution<> distrib(1, 6);

    return distrib(gen);
}

TEST(random, simple)
{
    for (int i = 0; i < 10; ++i)
        std::cout << dice() << " ";
    std::cout << "\n";
}

static const long long DAY_IN_SECONDS = 86400;      // 24 * 60 *60
static const long long WORK_DAY_IN_SECONDS = 21600; // 6 *60 *60

class ExpireCache
{
  public:
    ExpireCache() : mRefreshTimeDistribution(DAY_IN_SECONDS, DAY_IN_SECONDS + WORK_DAY_IN_SECONDS)
    {      
        m_generator.seed(std::random_device()());
    }

    long long refreshTime()
    {
        using namespace std::chrono;
        auto now = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count() +
                   mRefreshTimeDistribution(m_generator) * 1000;
        return now;
    }

    std::mt19937 m_generator;
    std::uniform_int_distribution<long long> mRefreshTimeDistribution;
};

TEST(random, refreshtime)
{
    ExpireCache cache;

    for (int i = 0; i < 10; ++i)
        std::cout << cache.refreshTime() << '\n';
}
