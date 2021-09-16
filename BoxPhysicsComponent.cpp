#include "BoxPhysicsComponent.h"
#include "ComponentSystem.h"
#include "GameObject.h"
#include "Transform.h"

BoxPhysicsComponent::BoxPhysicsComponent(Vector3 half_extents, reactphysics3d::BodyType body_type, GameObject* owner)
	: m_half_extents(half_extents), m_body_type(body_type), Component(owner)
{
	ComponentSystem::get().getPhysicsSystem().addComponent(this);
}

void BoxPhysicsComponent::onStart()
{
	// Create a rigid body in the world
	Vector3 nativePos = getOwner()->getComponent<Transform>().getPosition();
	reactphysics3d::Vector3 pos(nativePos.x, nativePos.y, nativePos.z);
	reactphysics3d::Quaternion rot(getOwner()->getComponent<Transform>().getOrientation());
	reactphysics3d::PhysicsCommon* physics_common = ComponentSystem::get().getPhysicsSystem().getPhysicsCommon();
	reactphysics3d::PhysicsWorld* physics_world = ComponentSystem::get().getPhysicsSystem().getPhysicsWorld();
	reactphysics3d::BoxShape* boxShape = physics_common->createBoxShape({ m_half_extents.x, m_half_extents.y, m_half_extents.z });
	m_rigidbody = physics_world->createRigidBody(reactphysics3d::Transform(pos, rot));
	m_collider = m_rigidbody->addCollider(boxShape, reactphysics3d::Transform::identity());
	m_rigidbody->setMass(3.0f);
	m_rigidbody->setType(m_body_type);
	m_rigidbody->setIsAllowedToSleep(false);
}

void BoxPhysicsComponent::perform()
{
	
}

void BoxPhysicsComponent::prePhysicsUpdate()
{
	if (!m_was_adjusted)
		return;
	
	m_was_adjusted = false;

	// Copy transform to React Physics 3D
	const Transform& transform = getOwner()->getComponent<Transform>();
	Vector3 nativePos = transform.getPosition();
	reactphysics3d::Vector3 pos(nativePos.x, nativePos.y, nativePos.z);
	reactphysics3d::Quaternion rot = transform.getOrientation();
	m_rigidbody->setTransform(reactphysics3d::Transform(pos, rot));
	m_rigidbody->setLinearVelocity({ 0.0f, 0.0f, 0.0f });
	m_rigidbody->setAngularVelocity({ 0.0f, 0.0f, 0.0f });
}

void BoxPhysicsComponent::postPhysicsUpdate()
{
	// Copy transform from React Physics 3D to GameObject
	const reactphysics3d::Transform& transform = m_rigidbody->getTransform();
	reactphysics3d::Vector3 reactPos = transform.getPosition();
	reactphysics3d::Quaternion q = transform.getOrientation();
	getOwner()->getComponent<Transform>().setPosition(reactPos.x, reactPos.y, reactPos.z);
	getOwner()->getComponent<Transform>().setOrientation(q);
}

void BoxPhysicsComponent::setAdjusted(bool value)
{
	m_was_adjusted = value;
}

reactphysics3d::RigidBody* BoxPhysicsComponent::getRigidBody() const
{
	return m_rigidbody;
}

BoxPhysicsComponent::~BoxPhysicsComponent()
{
	delete m_collider;
	ComponentSystem::get().getPhysicsSystem().removeComponent(this);
}
