#pragma once
#include "Component.h"
#include <reactphysics3d/collision/Collider.h>
#include <reactphysics3d/components/RigidBodyComponents.h>

class GameObject;

class Rigidbody : public Component
{
public:
	Rigidbody(reactphysics3d::BodyType body_type, GameObject* owner);
	
	void perform() override;
	
	void addCollider(reactphysics3d::CollisionShape* collision_shape);

	void prePhysicsUpdate();
	void postPhysicsUpdate();

	void setAdjusted(bool value);

	reactphysics3d::RigidBody* getRigidBody() const;
	reactphysics3d::BodyType getBodyType() const;

	~Rigidbody();

protected:
	void onStart() override;

private:
	reactphysics3d::RigidBody* m_rigidbody = nullptr;
	reactphysics3d::BodyType m_body_type = reactphysics3d::BodyType::DYNAMIC;
	bool m_was_adjusted = false;
};
