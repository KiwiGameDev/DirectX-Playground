#include "Random.h"

Random* Singleton<Random>::instance = nullptr;

float Random::range(float min, float max)
{
	std::uniform_real_distribution distribution(min, max);
	return distribution(engine);
}

Random::Random()
{
	
}
