#pragma once
#include "Camera.h"
#include "GameObject.h"

class FrustumGO : public GameObject
{
public:
	FrustumGO(const std::string& name, VertexBufferPtr vertex_buffer, IndexBufferPtr index_buffer, ConstantBufferPtr constant_buffer, VertexShaderPtr vertex_shader, PixelShaderPtr pixel_shader, Camera* camera);

	void update() override;
	void draw() override;

private:
	Camera* m_camera;
};

