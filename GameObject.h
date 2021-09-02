#pragma once
#include "Transformable.h"
#include "Component.h"
#include "Prerequisites.h"
#include <string>
#include <vector>
#include <unordered_map>

class GameObject : public Transformable
{
public:
	GameObject(const std::string& name, VertexBufferPtr vertex_buffer, IndexBufferPtr index_buffer, ConstantBufferPtr constant_buffer, VertexShaderPtr vertex_shader, PixelShaderPtr pixel_shader);

	virtual void awake();
	
	void update();
	void draw();
	
	void setTexture(const TexturePtr& texture);

	void attachComponent(Component* component);
	void detachComponent(Component* component);

	std::string getName() const;
	Component* getComponentByName(const std::string& name);
	Component* getComponentByType(Component::Type type);
	std::vector<Component*> getComponentsByType(Component::Type type);
	std::vector<Component*> getComponentsByTypeRecursive(Component::Type type);

private:
	std::string name;

	bool m_is_enabled = true;

	std::vector<Component*> m_component_list;
	std::unordered_map<std::string, Component*> m_component_map;

	TexturePtr m_texture = nullptr;
	
	VertexBufferPtr m_vb = nullptr;
	IndexBufferPtr m_ib = nullptr;
	ConstantBufferPtr m_cb = nullptr;
	VertexShaderPtr m_vs = nullptr;
	PixelShaderPtr m_ps = nullptr;
};
