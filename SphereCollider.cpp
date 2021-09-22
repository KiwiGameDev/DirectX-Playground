#include "SphereCollider.h"
#include "ComponentSystem.h"
#include "GameObject.h"
#include "Rigidbody.h"
#include <reactphysics3d/engine/PhysicsCommon.h>

SphereCollider::SphereCollider(float radius, GameObject* owner)
	: Component(owner), m_radius(radius)
{
	
}

void SphereCollider::onStart()
{
	reactphysics3d::PhysicsCommon* physics_common = ComponentSystem::get().getPhysicsSystem().getPhysicsCommon();
	reactphysics3d::SphereShape* collider = physics_common->createSphereShape(m_radius);
	getOwner()->getComponent<Rigidbody>().addCollider(collider);
}

void SphereCollider::perform()
{
	
}
