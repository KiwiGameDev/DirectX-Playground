#pragma once
#include "Singleton.h"
#include <random>

class Random : public Singleton<Random>
{
	friend class Singleton<Random>;

public:
	void setSeed(size_t value);

	int range(int x, int y);
	float range(float x, float y);
	void perlinNoise2D(int width, int height, float seed[], int octaves, float bias, float output[]);

private:
	Random();
	~Random();

	std::default_random_engine random_engine;
};
