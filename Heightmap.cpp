#include "Heightmap.h"
#include "GraphicsEngine.h"
#include "RenderSystem.h"
#include <d3d11.h>

Heightmap::Heightmap(unsigned width, unsigned height, float* buffer)
{
	D3D11_TEXTURE2D_DESC tdesc;
	D3D11_SUBRESOURCE_DATA data;

	data.pSysMem = (void*)buffer;
	data.SysMemPitch = width * sizeof(float);
	data.SysMemSlicePitch = width * height * sizeof(float);

	tdesc.Width = width;
	tdesc.Height = height;
	tdesc.MipLevels = 1;
	tdesc.ArraySize = 1;
	tdesc.SampleDesc.Count = 1;
	tdesc.SampleDesc.Quality = 0;
	tdesc.Usage = D3D11_USAGE_DEFAULT;
	tdesc.Format = DXGI_FORMAT_R32_FLOAT;
	tdesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	tdesc.CPUAccessFlags = 0;
	tdesc.MiscFlags = 0;
	
	if (FAILED(GraphicsEngine::get().getRenderSystem()->m_d3d_device->CreateTexture2D(&tdesc, &data, &m_texture)))
		throw std::exception("Heightmap failed to initialize!");

	D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc = {};
	srv_desc.Format = DXGI_FORMAT_R32_FLOAT;
	srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srv_desc.Texture2D.MipLevels = 1;
	srv_desc.Texture2D.MostDetailedMip = 0;
	
	if (FAILED(GraphicsEngine::get().getRenderSystem()->m_d3d_device->CreateShaderResourceView(m_texture, &srv_desc, &m_shader_resource_view)))
		throw std::exception("Heightmap failed to initialize!");
}

Heightmap::~Heightmap()
{
	m_texture->Release();
	m_shader_resource_view->Release();
}
