#pragma once
#include "IUpdateable.h"
#include "Transformable.h"
#include "Prerequisites.h"
#include <string>

class GameObject : public IUpdateable, public Transformable
{
public:
	GameObject(const std::string& name, VertexBufferPtr vertex_buffer, IndexBufferPtr index_buffer, ConstantBufferPtr constant_buffer, VertexShaderPtr vertex_shader, PixelShaderPtr pixel_shader);

	void update() override;
	
	void draw();

	void setTexture(const TexturePtr& texture);

	std::string getName() const;

private:
	std::string name;

	TexturePtr m_texture = nullptr;
	
	VertexBufferPtr m_vb = nullptr;
	IndexBufferPtr m_ib = nullptr;
	ConstantBufferPtr m_cb = nullptr;
	VertexShaderPtr m_vs = nullptr;
	PixelShaderPtr m_ps = nullptr;
};
