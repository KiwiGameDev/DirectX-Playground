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

GameObject::GameObject(const std::string& name)
	: name(name)
{
	addComponent<Transform>(this);
}

GameObject::GameObject(const std::string& name, VertexBufferPtr vertex_buffer, IndexBufferPtr index_buffer, ConstantBufferPtr constant_buffer, VertexShaderPtr vertex_shader, PixelShaderPtr pixel_shader)
	: name(name), m_vb(std::move(vertex_buffer)), m_ib(std::move(index_buffer)), m_cb(std::move(constant_buffer)), m_vs(std::move(vertex_shader)), m_ps(std::move(pixel_shader))
{
	addComponent<Transform>(this);
}

void GameObject::start()
{
	if (!m_is_enabled)
		return;
	
	for (auto name_component_pair : m_component_map)
	{
		name_component_pair.second->start();
	}
}

void GameObject::update()
{
	if (!m_is_enabled)
		return;

	for (auto name_component_pair : m_component_map)
	{
		name_component_pair.second->perform();
	}
}

void GameObject::draw()
{
	if (!m_is_enabled)
		return;
	if (m_vb == nullptr || m_ib == nullptr || m_vs == nullptr || m_ib == nullptr)
		return;
	
	DeviceContextPtr device_context = GraphicsEngine::get().getRenderSystem().getImmediateDeviceContext();
	
	ConstantBufferData cbd;
	cbd.m_world = getComponent<Transform>().getTransformMatrix();
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

void GameObject::setTexture(const TexturePtr& texture)
{
	m_texture = texture;
}

std::string GameObject::getName() const
{
	return name;
}

bool GameObject::getIsEnabled() const
{
	return m_is_enabled;
}

GameObject::~GameObject()
{
	// TODO: Delete all components
}
