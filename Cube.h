#pragma once
#include "GameObject.h"

class Cube : public GameObject
{
public:
	Cube(const std::string& name, VertexBufferPtr vertex_buffer, IndexBufferPtr index_buffer, VertexShaderPtr vertex_shader, PixelShaderPtr pixel_shader);

	void update() override;

private:
	Vector3 rotation_update;
};
