#pragma once
#include "Component.h"
#include "Prerequisites.h"
#include <string>

class MeshRenderer : public Component
{
public:
	MeshRenderer(GameObject* owner, const MeshPtr& mesh, const ConstantBufferPtr& cb, const VertexShaderPtr& vs, const PixelShaderPtr& ps);
	
	void perform() override;

	void draw() const;

	void setTexture(const TexturePtr& texture);
	
	std::string getMeshName() const;
	std::string getTextureName() const;

protected:
	void onStart() override;

private:
	TexturePtr m_texture = nullptr;

	MeshPtr m_mesh = nullptr;
	ConstantBufferPtr m_cb = nullptr;
	VertexShaderPtr m_vs = nullptr;
	PixelShaderPtr m_ps = nullptr;
};
