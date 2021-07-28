#include "GameObject.h"
#include "ConstantBuffer.h"
#include "ConstantBufferData.h"
#include "IndexBuffer.h"
#include "GraphicsEngine.h"
#include "RenderSystem.h"
#include "DeviceContext.h"

GameObject::GameObject(const std::string& name, VertexBufferPtr vertex_buffer, IndexBufferPtr index_buffer, VertexShaderPtr vertex_shader, PixelShaderPtr pixel_shader)
	: name(name), vb(vertex_buffer), ib(index_buffer), vs(vertex_shader), ps(pixel_shader)
{
	position = Vector3(0.0f, 0.0f, 0.0f);
	rotation = Vector3(0.0f, 0.0f, 0.0f);
	scale = Vector3(1.0f, 1.0f, 1.0f);
}

void GameObject::draw(const ConstantBufferPtr& cb, ConstantBufferData& cbd)
{
	DeviceContextPtr device_context = GraphicsEngine::get().getRenderSystem()->getImmediateDeviceContext();
	
	cbd.m_world = Matrix4x4::identity();
	cbd.m_world *= Matrix4x4::scale(scale);
	cbd.m_world *= Matrix4x4::rotationX(rotation.x);
	cbd.m_world *= Matrix4x4::rotationY(rotation.y);
	cbd.m_world *= Matrix4x4::rotationZ(rotation.z);
	cbd.m_world *= Matrix4x4::translation(position);
	cb->update(device_context, &cbd);

	device_context->setVertexShader(vs);
	device_context->setPixelShader(ps);
	device_context->setConstantBuffer(vs, cb);
	device_context->setConstantBuffer(ps, cb);
	device_context->setVertexBuffer(vb);
	device_context->setIndexBuffer(ib);
	device_context->drawIndexedTriangleList(ib->getSizeIndices(), 0, 0);
}

void GameObject::setPosition(float x, float y, float z)
{
	position = Vector3(x, y, z);
}

void GameObject::setPosition(const Vector3& newPosition)
{
	position = newPosition;
}

Vector3 GameObject::getLocalPosition() const
{
	return position;
}

void GameObject::setScale(float x, float y, float z)
{
	scale = Vector3(x, y, z);
}

void GameObject::setScale(const Vector3& newScale)
{
	scale = newScale;
}

Vector3 GameObject::getLocalScale() const
{
	return scale;
}

void GameObject::setRotation(float x, float y, float z)
{
	rotation = Vector3(x, y, z);
}

void GameObject::setRotation(const Vector3& newRotation)
{
	rotation = newRotation;
}

Vector3 GameObject::getLocalRotation() const
{
	return rotation;
}

GameObject::~GameObject()
{

}
