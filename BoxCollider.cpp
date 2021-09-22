#include "BoxCollider.h"
#include "ComponentSystem.h"
#include "GameObject.h"
#include "Rigidbody.h"

BoxCollider::BoxCollider(const Vector3& half_extents, GameObject* owner)
	: Component(owner), m_half_extents(half_extents)
{
	
}

void BoxCollider::onStart()
{
	reactphysics3d::PhysicsCommon* physics_common = ComponentSystem::get().getPhysicsSystem().getPhysicsCommon();
	reactphysics3d::BoxShape* collider = physics_common->createBoxShape({ m_half_extents.x, m_half_extents.y, m_half_extents.z });
	getOwner()->getComponent<Rigidbody>().addCollider(collider);
}

void BoxCollider::perform()
{
	
}
