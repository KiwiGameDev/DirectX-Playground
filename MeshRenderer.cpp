#include "MeshRenderer.h"
#include "GraphicsEngine.h"
#include "CameraManager.h"
#include "Time.h"
#include "DeviceContext.h"
#include "ConstantBufferData.h"
#include "ConstantBuffer.h"
#include "IndexBuffer.h"
#include "Transform.h"

MeshRenderer::MeshRenderer(GameObject* owner, VertexBufferPtr vb, IndexBufferPtr ib, ConstantBufferPtr cb, VertexShaderPtr vs, PixelShaderPtr ps)
	: Component(owner), m_vb(vb), m_ib(ib), m_cb(cb), m_vs(vs), m_ps(ps)
{
	
}

void MeshRenderer::onStart()
{
	
}

void MeshRenderer::perform()
{
	
}

void MeshRenderer::draw()
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
	device_context->setVertexBuffer(m_vb);
	device_context->setIndexBuffer(m_ib);
	if (m_texture != nullptr)
	{
		device_context->setTextureVertexShader(m_texture);
		device_context->setTexturePixelShader(m_texture);
	}
	device_context->drawIndexedTriangleList(m_ib->getSizeIndices(), 0, 0);
}

void MeshRenderer::setTexture(const TexturePtr& texture)
{
	m_texture = texture;
}
