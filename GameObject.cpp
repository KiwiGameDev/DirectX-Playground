#include "GameObject.h"
#include "GraphicsEngine.h"
#include "RenderSystem.h"
#include "DeviceContext.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "ConstantBufferData.h"
#include "CameraManager.h"
#include "Time.h"
#include <utility>


GameObject::GameObject(const std::string& name, VertexBufferPtr vertex_buffer, IndexBufferPtr index_buffer, ConstantBufferPtr constant_buffer, VertexShaderPtr vertex_shader, PixelShaderPtr pixel_shader)
	: name(name), m_vb(std::move(vertex_buffer)), m_ib(std::move(index_buffer)), m_cb(std::move(constant_buffer)), m_vs(std::move(vertex_shader)), m_ps(std::move(pixel_shader))
{
	
}

void GameObject::update()
{
	
}

void GameObject::draw(bool is_project)
{
	DeviceContextPtr device_context = GraphicsEngine::get().getRenderSystem()->getImmediateDeviceContext();
	
	ConstantBufferData cbd;
	cbd.m_world = getTransform();
	cbd.m_game_cam = CameraManager::get().getGameCamera()->getViewMatrix();
	cbd.m_game_cam *= CameraManager::get().getGameCamera()->getProjectionMatrix();
	cbd.m_editor_view = CameraManager::get().getEditorCamera()->getViewMatrix();
	cbd.m_editor_proj = CameraManager::get().getEditorCamera()->getProjectionMatrix();
	cbd.m_is_project = is_project;
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