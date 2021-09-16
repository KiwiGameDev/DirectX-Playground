#pragma once
#include "Component.h"
#include "Prerequisites.h"
#include <string>
#include <unordered_map>

class GameObject
{
public:
	GameObject(const std::string& name);

	void start();
	virtual void update();
	void draw();
	
	std::string getName() const;
	bool getIsEnabled() const;
	
	template<typename Component, typename... Args>
	Component& addComponent(Args&&... args)
	{
		const char* name = typeid(Component).raw_name();
		Component* component = new Component(std::forward<Args>(args)...);
		m_component_map.insert({ name, component });
		component->attachOwner(this);
		return *static_cast<Component*>(m_component_map[name]);
	}

	template<typename Component>
	void removeComponent()
	{
		m_component_map.erase(typeid(Component).raw_name());
	}

	template<typename Component>
	Component& getComponent()
	{
		return *static_cast<Component*>(m_component_map[typeid(Component).raw_name()]);
	}

	template<typename Component>
	bool hasComponent()
	{
		static_assert(std::is_base_of<Component, Component>::value);
		return m_component_map.find(typeid(Component).raw_name()) != m_component_map.end();
	}

	virtual ~GameObject();

private:
	std::string name;

	bool m_is_enabled = true;
	
	std::unordered_map<const char*, Component*> m_component_map;
};
