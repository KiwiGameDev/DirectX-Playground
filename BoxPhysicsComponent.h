#pragma once
#include "Component.h"
#include "Vector3.h"
#include <reactphysics3d/body/RigidBody.h>
#include <reactphysics3d/components/RigidBodyComponents.h>

class BoxPhysicsComponent : public Component
{
public:
	BoxPhysicsComponent(Vector3 half_extents, reactphysics3d::BodyType body_type, GameObject* owner);

	void perform() override;

	void prePhysicsUpdate();
	void postPhysicsUpdate();
	
	void setAdjusted(bool value);
	
	reactphysics3d::RigidBody* getRigidBody() const;

	~BoxPhysicsComponent();

protected:
	void onStart() override;

private:
	reactphysics3d::RigidBody* m_rigidbody = nullptr;
	reactphysics3d::Collider* m_collider = nullptr;
	reactphysics3d::BodyType m_body_type = reactphysics3d::BodyType::DYNAMIC;
	
	bool m_was_adjusted = false;
	Vector3 m_half_extents;
};
