#pragma once
#include <string>

class GameObject;

class Component
{
public:
	enum class Type
	{
		Invalid = -1,
		Script,
		Physics,
		Renderer,
		Input,
	};
	
	Component(const std::string& name, GameObject* owner, Type type);

	void attachOwner(GameObject* owner);
	void detachOwner();

	virtual void awake() = 0;
	virtual void perform() = 0;

	std::string getName() const;
	GameObject* getOwner() const;
	Type getType() const;

private:
	std::string m_name;
	GameObject* m_owner;
	Type m_type;
};
