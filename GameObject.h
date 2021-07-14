#pragma once
#include "Matrix4x4.h"
#include "Prerequisites.h"

__declspec(align(16))
struct constant
{
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_proj;
	unsigned int m_time;
};

class GameObject
{
public:
	GameObject(VertexBufferPtr vb, IndexBufferPtr ib, ConstantBufferPtr cb, VertexShaderPtr vs, PixelShaderPtr ps);
	
	void render(constant cc);
	
	~GameObject() = default;

	Vector3 position;
	Vector3 rotation;
	Vector3 scale;

private:
	VertexBufferPtr vb;
	IndexBufferPtr ib;
	ConstantBufferPtr cb;
	VertexShaderPtr vs;
	PixelShaderPtr ps;
};
