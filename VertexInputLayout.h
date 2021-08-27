#pragma once
#include "VertexFormat.h"
#include "Prerequisites.h"
#include <d3d11.h>

class RenderSystem;

class VertexInputLayout
{
	friend class DeviceContext;

public:
	VertexInputLayout(VertexFormat vertex_format, VertexShaderPtr vertex_shader, RenderSystem* render_system);

	~VertexInputLayout();

private:
	ID3D11InputLayout* m_layout;
};
