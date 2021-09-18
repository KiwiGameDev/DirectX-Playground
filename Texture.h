#pragma once
#include "Resource.h"
#include <d3d11.h>
#include <string>

class Texture : public Resource
{
	friend class DeviceContext;

public:
	Texture(const std::string& file_path);
	
	ID3D11ShaderResourceView* getShaderResourceView() const;

	std::string getName() const;

	~Texture();

private:
	ID3D11Resource* m_texture = nullptr;
	ID3D11ShaderResourceView* m_shader_resource_view = nullptr;

	std::string m_name;
};
