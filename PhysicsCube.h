#pragma once
#include "GameObject.h"
#include <reactphysics3d/body/RigidBody.h>

class BoxPhysicsComponent;

class PhysicsCube : public GameObject
{
public:
	PhysicsCube(const std::string& name, VertexBufferPtr vertex_buffer, IndexBufferPtr index_buffer, ConstantBufferPtr constant_buffer, VertexShaderPtr vertex_shader, PixelShaderPtr pixel_shader);

	void awake() override;

	void setRigidBodyType(reactphysics3d::BodyType body_type);

private:
	BoxPhysicsComponent* m_physics_component;
};
