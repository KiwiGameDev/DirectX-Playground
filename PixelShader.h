#pragma once
class RenderSystem;
class DeviceContext;
class ID3D11PixelShader;

class PixelShader
{
	friend class RenderSystem;
	friend class DeviceContext;

public:
	PixelShader(const void* shader_byte_code, size_t size_byte_code, RenderSystem* system);
	
	~PixelShader();

private:
	ID3D11PixelShader* m_ps;

	RenderSystem* m_system;
};
