#pragma once
#include <d3d11.h>

class RenderSystem;

class SamplerState
{
	friend class RenderSystem;
	friend class DeviceContext;

public:
	SamplerState(D3D11_SAMPLER_DESC sampler_desc, RenderSystem* render_system);

	~SamplerState();

private:
	ID3D11SamplerState* m_sampler_state;
	D3D11_SAMPLER_DESC m_sampler_desc;
};
