#pragma once
#include "Resource.h"

class RenderSystem;
class DeviceContext;
class ID3D11VertexShader;

class VertexShader : public Resource
{
	friend class RenderSystem;
	friend class DeviceContext;
	friend class VertexInputLayout;
	
public:
	VertexShader(const wchar_t* full_path, RenderSystem& render_system);

	~VertexShader();

private:
	ID3D11VertexShader* m_vs = nullptr;
	void* m_shader_byte_code = nullptr;
	size_t m_shader_byte_code_size = 0;
};
