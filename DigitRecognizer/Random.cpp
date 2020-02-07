#include "Random.h"

Random::Random()
{
	s_RandomEngine.seed(std::random_device()());
}

float Random::Float()
{
	return static_cast<float>(s_Distribution(s_RandomEngine)) / static_cast<float>(std::numeric_limits<uint32_t>::max());
}

int32_t Random::Int(int32_t start, int32_t end)
{
	return (s_Distribution(s_RandomEngine) % (end - start + 1)) + start;
}

Random rnd;