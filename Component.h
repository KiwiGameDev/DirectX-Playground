#pragma once
class GameObject;

class Component
{
public:
	Component(GameObject* owner);

	void attachOwner(GameObject* owner);
	void detachOwner();

	void start();

	virtual void perform() = 0;
	
	GameObject* getOwner() const;

	virtual ~Component() = default;

protected:
	virtual void onStart() = 0;

private:
	GameObject* m_owner;

	bool m_has_started = false;
};
