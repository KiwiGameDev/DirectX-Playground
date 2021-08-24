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

	std::string getName() const;

private:
	std::string name;
	
	VertexBufferPtr m_vb;
	IndexBufferPtr m_ib;
	ConstantBufferPtr m_cb;
	VertexShaderPtr m_vs;
	PixelShaderPtr m_ps;
};
