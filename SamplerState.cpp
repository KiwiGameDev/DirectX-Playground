#include "SamplerState.h"
#include "RenderSystem.h"

SamplerState::SamplerState(D3D11_SAMPLER_DESC sampler_desc, RenderSystem* render_system)
{
	m_sampler_desc = sampler_desc;

	if (FAILED(render_system->m_d3d_device->CreateSamplerState(&m_sampler_desc, &m_sampler_state)))
		throw std::exception("Failed to create Sampler State!");
}

SamplerState::~SamplerState()
{
	m_sampler_state->Release();
}
