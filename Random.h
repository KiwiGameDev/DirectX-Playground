#pragma once
#include "Singleton.h"
#include <random>

class Random : public Singleton<Random>
{
	friend class Singleton<Random>;

public:
	float range(float min, float max);
	
private:
	Random();

	std::default_random_engine engine;
	float delta_time = 0.0f;
};