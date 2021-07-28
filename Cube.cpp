#include "Cube.h"
#include "Random.h"
#include <utility>

Cube::Cube(const std::string& name, VertexBufferPtr vertex_buffer, IndexBufferPtr index_buffer, VertexShaderPtr vertex_shader, PixelShaderPtr pixel_shader)
	: GameObject(name, std::move(vertex_buffer), std::move(index_buffer), std::move(vertex_shader), std::move(pixel_shader))
{
	float x = Random::get().range(-7.0f, 7.0f);
	float y = Random::get().range(-7.0f, 7.0f);
	float z = Random::get().range(2.0f, 3.0f);
	rotation_update = Vector3(x, y, z);
}

void Cube::update(float deltaTime)
{
	rotation.x += rotation_update.x * deltaTime;
	rotation.y += rotation_update.y * deltaTime;
	rotation.z += rotation_update.z * deltaTime;
}
