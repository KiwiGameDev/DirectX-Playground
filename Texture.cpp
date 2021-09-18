#include "Texture.h"
#include "GraphicsEngine.h"
#include "RenderSystem.h"
#include <DirectXTex.h>

Texture::Texture(const std::string& file_path)
{
	DirectX::ScratchImage image_data;
	HRESULT res = LoadFromWICFile(std::wstring(file_path.begin(), file_path.end()).c_str(), DirectX::WIC_FLAGS_NONE, nullptr, image_data);

	if (!SUCCEEDED(res))
		throw std::exception("Texture failed to load successfully!");

	res = CreateTexture(
		GraphicsEngine::get().getRenderSystem().m_d3d_device,
		image_data.GetImages(),
		image_data.GetImageCount(),
		image_data.GetMetadata(),
		&m_texture);

	D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
	desc.Format = image_data.GetMetadata().format;
	desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MipLevels = image_data.GetMetadata().mipLevels;
	desc.Texture2D.MostDetailedMip = 0;

	GraphicsEngine::get().getRenderSystem().m_d3d_device->CreateShaderResourceView(m_texture, &desc, &m_shader_resource_view);

	size_t name_index_start = file_path.find_last_of('\\');

	if (name_index_start != std::string::npos)
	{
		m_name = file_path.substr(name_index_start + 1, file_path.size() - name_index_start - 1);
	}
	else
	{
		m_name = file_path;
	}
}

ID3D11ShaderResourceView* Texture::getShaderResourceView() const
{
	return m_shader_resource_view;
}

std::string Texture::getName() const
{
	return m_name;
}

Texture::~Texture()
{
	m_texture->Release();
	m_shader_resource_view->Release();
}
