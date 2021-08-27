#pragma once
#include "Resource.h"

class RenderSystem;
class DeviceContext;
class ID3D11PixelShader;

class PixelShader : public Resource
{
	friend class RenderSystem;
	friend class DeviceContext;

public:
	PixelShader(const wchar_t* full_path, RenderSystem* system);
	
	~PixelShader();

private:
	RenderSystem* m_system = nullptr;

	ID3D11PixelShader* m_ps = nullptr;
	void* m_shader_byte_code = nullptr;
	size_t m_shader_byte_code_size = 0;
};
