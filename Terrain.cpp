#include "Terrain.h"
#include "vertex.h"
#include "GraphicsEngine.h"
#include "RenderSystem.h"
#include "DeviceContext.h"
#include "constant.h"
#include "ConstantBuffer.h"
#include "IndexBuffer.h"
#include "Random.h"
#include <random>

Terrain::Terrain() : Terrain(128u, 128u, 128u, 128u, 0u) { }

Terrain::Terrain(unsigned int terrain_width, unsigned int terrain_length, unsigned int heightmap_width, unsigned int heightmap_length, int seed)
	: terrain_width(terrain_width), terrain_length(terrain_length), heightmap_width(heightmap_width), heightmap_length(heightmap_length), seed(seed)
{
	unsigned int subdivisions_width = terrain_width - 1;
	unsigned int subdivisions_length = terrain_length - 1;
	unsigned int vertices_width = terrain_width;
	unsigned int vertices_length = terrain_length;

	// Generate vertices
	std::vector<vertex> terrainVertices;
	std::vector<unsigned int> terrainIndices;
	for (int i = 0; i <= subdivisions_length; i++)
	{
		for (int j = 0; j <= subdivisions_width; j++)
		{
			terrainVertices.push_back({ {(float)j / subdivisions_width, 0.0f, (float)i / subdivisions_length } });
		}
	}

	// Generate indices
	for (int i = 0; i < subdivisions_length; i++)
	{
		for (int j = 0; j < subdivisions_width; j++)
		{
			unsigned int botLeft = j + i * vertices_width;
			unsigned int topLeft = j + (i + 1) * vertices_width;
			unsigned int topRight = (j + 1) + (i + 1) * vertices_width;
			unsigned int botRight = (j + 1) + i * vertices_width;
			terrainIndices.push_back(botLeft);
			terrainIndices.push_back(topLeft);
			terrainIndices.push_back(topRight);
			terrainIndices.push_back(botLeft);
			terrainIndices.push_back(topRight);
			terrainIndices.push_back(botRight);
		}
	}

	// Index Buffer
	ib = GraphicsEngine::get().getRenderSystem()->createIndexBuffer(terrainIndices.data(), terrainIndices.size());

	// Vertex Shader and Vertex Buffer
	void* shader_byte_code = nullptr;
	size_t size_shader_byte_code = 0;
	GraphicsEngine::get().getRenderSystem()->compileVertexShader(L"TerrainVertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader_byte_code);
	vs = GraphicsEngine::get().getRenderSystem()->createVertexShader(shader_byte_code, size_shader_byte_code);
	vb = GraphicsEngine::get().getRenderSystem()->createVertexBuffer(terrainVertices.data(), sizeof(vertex), terrainVertices.size(), shader_byte_code, size_shader_byte_code);
	GraphicsEngine::get().getRenderSystem()->releaseCompiledShader();

	// Pixel Shader
	GraphicsEngine::get().getRenderSystem()->compilePixelShader(L"TerrainPixelShader.hlsl", "psmain", &shader_byte_code, &size_shader_byte_code);
	ps = GraphicsEngine::get().getRenderSystem()->createPixelShader(shader_byte_code, size_shader_byte_code);
	GraphicsEngine::get().getRenderSystem()->releaseCompiledShader();

	// Allocate space for Perlin noise array
	perlin_noise_seed = (float*)malloc(heightmap_width * heightmap_length * sizeof(float));
	perlin_noise = (float*)malloc(heightmap_width * heightmap_length * sizeof(float));
}

void Terrain::update()
{
	// Update heightmap
	Random::get().perlinNoise2D(heightmap_width, heightmap_length, perlin_noise_seed, octaves, bias, perlin_noise);
	heightmap = GraphicsEngine::get().getRenderSystem()->createHeightmapTexture(heightmap_width, heightmap_length, perlin_noise);
}

void Terrain::draw(const ConstantBufferPtr& cb, constant& cc)
{
	DeviceContextPtr deviceContext = GraphicsEngine::get().getRenderSystem()->getImmediateDeviceContext();

	cc.m_world = Matrix4x4::identity();
	cc.m_world *= Matrix4x4::scale({ (float)terrain_width, 1.0f, (float)terrain_length });
	cb->update(deviceContext, &cc);
	
	deviceContext->setConstantBuffer(vs, cb);
	deviceContext->setConstantBuffer(ps, cb);
	deviceContext->setVertexShader(vs);
	deviceContext->setPixelShader(ps);
	deviceContext->setHeightmapVertexShader(heightmap);
	deviceContext->setVertexBuffer(vb);
	deviceContext->setIndexBuffer(ib);
	deviceContext->drawIndexedTriangleList(ib->getSizeIndices(), 0, 0);
}

void Terrain::GeneratePerlinNoiseSeed()
{
	std::default_random_engine randomEngine(seed);
	std::uniform_real_distribution distribution(0.0f, 1.0f);
	for (int i = 0; i < heightmap_width * heightmap_length; i++)
	{
		perlin_noise_seed[i] = distribution(randomEngine);
	}
}

Terrain::~Terrain()
{
	delete[] perlin_noise_seed;
	delete[] perlin_noise;
}
