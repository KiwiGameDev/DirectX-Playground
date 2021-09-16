#pragma once
#include "Component.h"
#include "Prerequisites.h"

class MeshRenderer : public Component
{
public:
	MeshRenderer(GameObject* owner, VertexBufferPtr vb, IndexBufferPtr ib, ConstantBufferPtr cb, VertexShaderPtr vs, PixelShaderPtr ps);
	
	void perform() override;

	void draw();

	void setTexture(const TexturePtr& texture);

protected:
	void onStart() override;

private:
	TexturePtr m_texture = nullptr;

	VertexBufferPtr m_vb = nullptr;
	IndexBufferPtr m_ib = nullptr;
	ConstantBufferPtr m_cb = nullptr;
	VertexShaderPtr m_vs = nullptr;
	PixelShaderPtr m_ps = nullptr;
};
