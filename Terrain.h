#pragma once
#include "Prerequisites.h"
#include "constant.h"

class Terrain
{
public:
	Terrain();
	Terrain(unsigned int terrain_width, unsigned int terrain_length, unsigned int heightmap_width, unsigned int heightmap_length, int seed = 0);

	void update();
	void draw(const ConstantBufferPtr& cb, constant& cc);

	void GeneratePerlinNoiseSeed();
	
	~Terrain();

	int seed;
	int octaves = 6;
	float bias = 1.0f;

private:

	VertexBufferPtr vb;
	IndexBufferPtr ib;
	VertexShaderPtr vs;
	PixelShaderPtr ps;
	
	unsigned int terrain_width;
	unsigned int terrain_length;

	HeightmapPtr heightmap;
	unsigned int heightmap_width;
	unsigned int heightmap_length;
	

	float* perlin_noise_seed;
	float* perlin_noise;
};
