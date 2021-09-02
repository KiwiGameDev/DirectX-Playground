#include "BoxPhysicsComponent.h"
#include "ComponentSystem.h"
#include "GameObject.h"

BoxPhysicsComponent::BoxPhysicsComponent(const std::string& name, Vector3 half_extents, GameObject* owner)
	: Component(name, owner, Type::Physics), m_half_extents(half_extents)
{
	
}

void BoxPhysicsComponent::awake()
{
	ComponentSystem::get().getPhysicsSystem().registerComponent(this);
	reactphysics3d::PhysicsCommon* physics_common = ComponentSystem::get().getPhysicsSystem().getPhysicsCommon();
	reactphysics3d::PhysicsWorld* physics_world = ComponentSystem::get().getPhysicsSystem().getPhysicsWorld();

	// Create a rigid body in the world
	reactphysics3d::Transform transform;
	Vector3 pos = getOwner()->getPosition();
	transform.setPosition({ pos.x, pos.y, pos.z });
	transform.setOrientation(getOwner()->getOrientation());

	reactphysics3d::BoxShape* boxShape = physics_common->createBoxShape({ m_half_extents.x, m_half_extents.y, m_half_extents.z });
	m_rigidbody = physics_world->createRigidBody(transform);
	m_rigidbody->addCollider(boxShape, reactphysics3d::Transform::identity());
	m_rigidbody->updateMassPropertiesFromColliders();
	m_rigidbody->setMass(3.0f);
	m_rigidbody->setType(reactphysics3d::BodyType::DYNAMIC);
	m_rigidbody->setIsAllowedToSleep(false);
}

void BoxPhysicsComponent::perform()
{
	const reactphysics3d::Transform& transform = m_rigidbody->getTransform();
	reactphysics3d::Vector3 reactPos = transform.getPosition();
	reactphysics3d::Quaternion q = transform.getOrientation();

	// Copy transform from React Physics 3D to GameObject
	getOwner()->setPosition(reactPos.x, reactPos.y, reactPos.z);
	getOwner()->setOrientation(q);
}

void BoxPhysicsComponent::prePerform()
{
	if (!m_was_adjusted)
		return;
	
	m_was_adjusted = false;

	// Copy transform to React Physics 3D
	Vector3 pos = getOwner()->getPosition();
	reactphysics3d::Quaternion orientation = getOwner()->getOrientation();
	reactphysics3d::Vector3 reactPos(pos.x, pos.y, pos.z);
	m_rigidbody->setTransform(reactphysics3d::Transform(reactPos, orientation));
	m_rigidbody->setLinearVelocity({ 0.0f, 0.0f, 0.0f });
	m_rigidbody->setAngularVelocity({ 0.0f, 0.0f, 0.0f });
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
	ComponentSystem::get().getPhysicsSystem().unregisterComponent(this);
}
