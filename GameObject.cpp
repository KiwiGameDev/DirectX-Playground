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
#include <iostream>

GameObject::GameObject(const std::string& name, VertexBufferPtr vertex_buffer, IndexBufferPtr index_buffer, ConstantBufferPtr constant_buffer, VertexShaderPtr vertex_shader, PixelShaderPtr pixel_shader)
	: name(name), m_vb(std::move(vertex_buffer)), m_ib(std::move(index_buffer)), m_cb(std::move(constant_buffer)), m_vs(std::move(vertex_shader)), m_ps(std::move(pixel_shader))
{
	
}

void GameObject::awake()
{
	
}

void GameObject::update()
{
	if (!m_is_enabled)
		return;

	for (Component* component : m_component_list)
	{
		if (component->getType() == Component::Type::Script)
		{
			component->perform();
		}
	}
}

void GameObject::draw()
{
	DeviceContextPtr device_context = GraphicsEngine::get().getRenderSystem().getImmediateDeviceContext();
	
	ConstantBufferData cbd;
	cbd.m_world = getTransform();
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

void GameObject::attachComponent(Component* component)
{
	if (m_component_map.find(component->getName()) == m_component_map.end())
	{
		m_component_list.push_back(component);
		m_component_map.insert({ component->getName(), component });
		component->attachOwner(this);
		component->awake();
	}
	else
	{
		std::cerr << component->getName() << " is already attached!\n";
	}
}

void GameObject::detachComponent(Component* component)
{
	if (m_component_map.find(component->getName()) != m_component_map.end())
	{
		m_component_list.erase(std::ranges::find(m_component_list, component));
		m_component_map.erase(component->getName());
	}
	else
	{
		std::cerr << component->getName() << " is already detached!\n";
	}
}

Component* GameObject::getComponentByName(const std::string& name)
{
	return m_component_map.at(name);
}

Component* GameObject::getComponentByType(Component::Type type)
{
	auto itr = std::ranges::find_if(m_component_list, [type](const Component* component)
	{
		return component->getType() == type;
	});

	if (itr != m_component_list.end())
		return *itr;

	return nullptr;
}

std::vector<Component*> GameObject::getComponentsByType(Component::Type type)
{
	std::vector<Component*> components_match;

	for (Component* component : m_component_list)
	{
		if (component->getType() == type)
		{
			components_match.push_back(component);
		}
	}

	return components_match;
}

std::vector<Component*> GameObject::getComponentsByTypeRecursive(Component::Type type)
{
	std::vector<Component*> components_match;

	// TODO: Get components of type including children

	return components_match;
}

std::string GameObject::getName() const
{
	return name;
}
