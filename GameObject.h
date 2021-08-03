#pragma once
#include "IUpdateable.h"
#include "ITransformable.h"
#include "ConstantBufferData.h"
#include "Prerequisites.h"
#include <string>

class GameObject : public IUpdateable, public ITransformable
{
public:
	GameObject(const std::string& name, VertexBufferPtr vertex_buffer, IndexBufferPtr index_buffer, VertexShaderPtr vertex_shader, PixelShaderPtr pixel_shader);;

	void update() override;
	
	void draw(const ConstantBufferPtr& cb, ConstantBufferData& cbd);

private:
	std::string name;
	
	VertexBufferPtr vb;
	IndexBufferPtr ib;
	VertexShaderPtr vs;
	PixelShaderPtr ps;
};
