#include "MeshRenderer.h"
#include "GraphicsEngine.h"
#include "CameraManager.h"
#include "Time.h"
#include "DeviceContext.h"
#include "ConstantBufferData.h"
#include "ConstantBuffer.h"
#include "Mesh.h"
#include "Texture.h"
#include "Transform.h"

MeshRenderer::MeshRenderer(GameObject* owner, const MeshPtr& mesh, const ConstantBufferPtr& cb, const VertexShaderPtr& vs, const PixelShaderPtr& ps)
	: Component(owner), m_mesh(mesh), m_cb(cb), m_vs(vs), m_ps(ps)
{
	
}

void MeshRenderer::onStart()
{
	
}

void MeshRenderer::perform()
{
	
}

void MeshRenderer::draw() const
{
	DeviceContextPtr device_context = GraphicsEngine::get().getRenderSystem().getImmediateDeviceContext();

	ConstantBufferData cbd;
	cbd.m_world = getOwner()->getComponent<Transform>().getTransformMatrix();
	cbd.m_view = CameraManager::get().getEditorCamera()->getViewMatrix();
	cbd.m_proj = CameraManager::get().getEditorCamera()->getProjectionMatrix();
	cbd.m_time = Time::get().timeSinceApplicationStart();
	m_cb->update(device_context, &cbd);

	device_context->setVertexShader(m_vs);
	device_context->setPixelShader(m_ps);
	device_context->setConstantBuffer(m_vs, m_cb);
	device_context->setConstantBuffer(m_ps, m_cb);
	device_context->setMesh(m_mesh);
	if (m_texture != nullptr)
	{
		device_context->setTextureVertexShader(m_texture);
		device_context->setTexturePixelShader(m_texture);
	}
	device_context->drawIndexedTriangleList(m_mesh->getIndicesCount(), 0, 0);
}

void MeshRenderer::setTexture(const TexturePtr& texture)
{
	m_texture = texture;
}

std::string MeshRenderer::getMeshName() const
{
	return m_mesh->getName();
}

std::string MeshRenderer::getTextureName() const
{
	if (m_texture == nullptr)
	{
		return "UNTEXTURED";
	}
	
	return m_texture->getName();
}
