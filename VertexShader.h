#pragma once
#include "Resource.h"
#include <string>

class RenderSystem;
class DeviceContext;
class ID3D11VertexShader;

class VertexShader : public Resource
{
	friend class RenderSystem;
	friend class DeviceContext;
	friend class VertexInputLayout;
	
public:
	VertexShader(const std::string& file_path, RenderSystem& render_system);

	~VertexShader();

private:
	ID3D11VertexShader* m_vs = nullptr;
	void* m_shader_byte_code = nullptr;
	size_t m_shader_byte_code_size = 0;
};
