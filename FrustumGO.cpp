#include "FrustumGO.h"
#include "ConstantBufferData.h"
#include "ConstantBuffer.h"
#include "IndexBuffer.h"
#include "DeviceContext.h"
#include "CameraManager.h"
#include "GraphicsEngine.h"
#include "RenderSystem.h"
#include "Time.h"

FrustumGO::FrustumGO(const std::string& name, VertexBufferPtr vertex_buffer, IndexBufferPtr index_buffer, ConstantBufferPtr constant_buffer, VertexShaderPtr vertex_shader, PixelShaderPtr pixel_shader, Camera* camera)
	: GameObject(name, std::move(vertex_buffer), std::move(index_buffer), std::move(constant_buffer), std::move(vertex_shader), std::move(pixel_shader)), m_camera(camera)
{
	
}

void FrustumGO::update()
{
	
}

void FrustumGO::draw()
{
	DeviceContextPtr device_context = GraphicsEngine::get().getRenderSystem()->getImmediateDeviceContext();

	// TODO: Jacob idk what I'm doing pls fix
	ConstantBufferData cbd;
	cbd.m_world = m_camera->getViewMatrix();
	cbd.m_world *= m_camera->getProjectionMatrix();
	cbd.m_world.inverse();
	cbd.m_world *= getTransform();
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
	device_context->drawIndexedTriangleList(m_ib->getSizeIndices(), 0, 0);
}
