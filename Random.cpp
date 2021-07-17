#include "Random.h"

Random* Random::instance = nullptr;

Random::Random()
	: random_engine(0)
{
	
}

void Random::setSeed(size_t value)
{
	random_engine.seed(value);
}

int Random::range(int x, int y)
{
	std::uniform_int_distribution distribution(x, y);
	return distribution(random_engine);
}

float Random::range(float x, float y)
{
	std::uniform_real_distribution distribution(x, y);
	return distribution(random_engine);
}

void Random::perlinNoise2D(int width, int height, float* seed, int octaves, float bias, float* output)
{
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			float fNoise = 0.0f;
			float fScaleAcc = 0.0f;
			float fScale = 1.0f;

			for (int o = 0; o < octaves; o++)
			{
				int nPitch = width >> o;
				int nSampleX1 = (x / nPitch) * nPitch;
				int nSampleY1 = (y / nPitch) * nPitch;

				int nSampleX2 = (nSampleX1 + nPitch) % width;
				int nSampleY2 = (nSampleY1 + nPitch) % width;

				float fBlendX = (float)(x - nSampleX1) / (float)nPitch;
				float fBlendY = (float)(y - nSampleY1) / (float)nPitch;

				float fSampleT = (1.0f - fBlendX) * seed[nSampleY1 * width + nSampleX1] + fBlendX * seed[nSampleY1 * width + nSampleX2];
				float fSampleB = (1.0f - fBlendX) * seed[nSampleY2 * width + nSampleX1] + fBlendX * seed[nSampleY2 * width + nSampleX2];

				fScaleAcc += fScale;
				fNoise += (fBlendY * (fSampleB - fSampleT) + fSampleT) * fScale;
				fScale = fScale / bias;
			}
			
			output[y * width + x] = fNoise / fScaleAcc;
		}
	}
}

Random::~Random()
{
	
}
