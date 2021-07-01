#pragma once
class RenderSystem;
class DeviceContext;
class ID3D11VertexShader;

class VertexShader
{
	friend class RenderSystem;
	friend class DeviceContext;
	
public:
	VertexShader(const void* shader_byte_code, size_t size_byte_code, RenderSystem* system);

	~VertexShader();

private:
	ID3D11VertexShader* m_vs;
	RenderSystem* m_system;
};
