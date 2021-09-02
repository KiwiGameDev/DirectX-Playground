#pragma once
#include "Component.h"
#include "Vector3.h"
#include <reactphysics3d/body/RigidBody.h>

class BoxPhysicsComponent : public Component
{
public:
	BoxPhysicsComponent(const std::string& name, Vector3 half_extents, GameObject* owner);

	void awake() override;
	void perform() override;

	void prePerform();

	void setAdjusted(bool value);
	
	reactphysics3d::RigidBody* getRigidBody() const;

	~BoxPhysicsComponent();

private:
	reactphysics3d::RigidBody* m_rigidbody = nullptr;
	
	bool m_was_adjusted = false;
	Vector3 m_half_extents;
};
