#pragma once
class GraphicsEngine;
class DeviceContext;
class ID3D11VertexShader;

class VertexShader
{
	friend class GraphicsEngine;
	friend class DeviceContext;
	
public:
	void release();

private:
	bool init(const void* shader_byte_code, size_t size_byte_code);

	ID3D11VertexShader* m_vs;
};
