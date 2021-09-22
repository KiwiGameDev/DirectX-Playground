#include "Rigidbody.h"
#include "ComponentSystem.h"
#include "GameObject.h"
#include "Transform.h"

Rigidbody::Rigidbody(reactphysics3d::BodyType body_type, GameObject* owner)
	: Component(owner), m_body_type(body_type)
{
	ComponentSystem::get().getPhysicsSystem().addComponent(this);
}

void Rigidbody::onStart()
{
	// Create a rigid body in the world
	Vector3 nativePos = getOwner()->getComponent<Transform>().getPosition();
	reactphysics3d::Vector3 pos(nativePos.x, nativePos.y, nativePos.z);
	reactphysics3d::Quaternion rot(getOwner()->getComponent<Transform>().getOrientation());
	reactphysics3d::PhysicsWorld* physics_world = ComponentSystem::get().getPhysicsSystem().getPhysicsWorld();
	m_rigidbody = physics_world->createRigidBody(reactphysics3d::Transform(pos, rot));
	m_rigidbody->setMass(3.0f);
	m_rigidbody->setType(m_body_type);
	m_rigidbody->setIsAllowedToSleep(false);
}

void Rigidbody::perform()
{

}

void Rigidbody::addCollider(reactphysics3d::CollisionShape* collision_shape)
{
	m_rigidbody->addCollider(collision_shape, reactphysics3d::Transform::identity());
}

void Rigidbody::prePhysicsUpdate()
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

void Rigidbody::postPhysicsUpdate()
{
	// Copy transform from React Physics 3D to GameObject
	const reactphysics3d::Transform& transform = m_rigidbody->getTransform();
	reactphysics3d::Vector3 reactPos = transform.getPosition();
	reactphysics3d::Quaternion q = transform.getOrientation();
	getOwner()->getComponent<Transform>().setPosition(reactPos.x, reactPos.y, reactPos.z);
	getOwner()->getComponent<Transform>().setOrientation(q);
}

void Rigidbody::setAdjusted(bool value)
{
	m_was_adjusted = value;
}

reactphysics3d::RigidBody* Rigidbody::getRigidBody() const
{
	return m_rigidbody;
}

reactphysics3d::BodyType Rigidbody::getBodyType() const
{
	return m_body_type;
}

Rigidbody::~Rigidbody()
{
	ComponentSystem::get().getPhysicsSystem().removeComponent(this);
}
