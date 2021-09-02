#include "PhysicsCube.h"
#include "BoxPhysicsComponent.h"
#include <reactphysics3d/components/RigidBodyComponents.h>

PhysicsCube::PhysicsCube(const std::string& name, VertexBufferPtr vertex_buffer, IndexBufferPtr index_buffer, ConstantBufferPtr constant_buffer, VertexShaderPtr vertex_shader, PixelShaderPtr pixel_shader)
	: GameObject(name, std::move(vertex_buffer), std::move(index_buffer), std::move(constant_buffer), std::move(vertex_shader), std::move(pixel_shader))
{
	
}

void PhysicsCube::awake()
{
	Vector3 scale = getScale();
	m_physics_component = new BoxPhysicsComponent("BoxPhysicsComponent", scale * 0.5f, this);
	attachComponent(m_physics_component);
}

void PhysicsCube::setRigidBodyType(reactphysics3d::BodyType body_type)
{
	m_physics_component->getRigidBody()->setType(body_type);
}
