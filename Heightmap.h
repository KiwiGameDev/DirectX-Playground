#pragma once
class ID3D11Texture2D;
class ID3D11ShaderResourceView;

class Heightmap
{
	friend class DeviceContext;

public:
	Heightmap(unsigned int width, unsigned int height, float* buffer);
	
	~Heightmap();

private:
	ID3D11Texture2D* m_texture = nullptr;
	ID3D11ShaderResourceView* m_shader_resource_view = nullptr;
};
