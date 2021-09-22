#include "CapsuleCollider.h"
#include "ComponentSystem.h"
#include "GameObject.h"
#include "Rigidbody.h"

CapsuleCollider::CapsuleCollider(float radius, float height, GameObject* owner)
	: Component(owner), m_radius(radius), m_height(height)
{
	
}

void CapsuleCollider::onStart()
{
	reactphysics3d::PhysicsCommon* physics_common = ComponentSystem::get().getPhysicsSystem().getPhysicsCommon();
	reactphysics3d::CapsuleShape* collider = physics_common->createCapsuleShape(m_radius, m_height);
	getOwner()->getComponent<Rigidbody>().addCollider(collider);
}

void CapsuleCollider::perform()
{
	
}
