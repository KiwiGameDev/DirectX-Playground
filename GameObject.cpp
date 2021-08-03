#include "GameObject.h"

#include <utility>
#include "ConstantBuffer.h"
#include "ConstantBufferData.h"
#include "IndexBuffer.h"
#include "GraphicsEngine.h"
#include "RenderSystem.h"
#include "DeviceContext.h"

GameObject::GameObject(const std::string& name, VertexBufferPtr vertex_buffer, IndexBufferPtr index_buffer, VertexShaderPtr vertex_shader, PixelShaderPtr pixel_shader)
	: name(name), vb(std::move(vertex_buffer)), ib(std::move(index_buffer)), vs(std::move(vertex_shader)), ps(std::move(pixel_shader))
{
	
}

void GameObject::update()
{
	
}

void GameObject::draw(const ConstantBufferPtr& cb, ConstantBufferData& cbd)
{
	DeviceContextPtr device_context = GraphicsEngine::get().getRenderSystem()->getImmediateDeviceContext();
	
	cbd.m_world = Matrix4x4::identity();
	cbd.m_world *= Matrix4x4::scale(Scale);
	cbd.m_world *= Matrix4x4::rotationX(Rotation.x);
	cbd.m_world *= Matrix4x4::rotationY(Rotation.y);
	cbd.m_world *= Matrix4x4::rotationZ(Rotation.z);
	cbd.m_world *= Matrix4x4::translation(Position);
	cb->update(device_context, &cbd);
	
	device_context->setVertexShader(vs);
	device_context->setPixelShader(ps);
	device_context->setConstantBuffer(vs, cb);
	device_context->setConstantBuffer(ps, cb);
	device_context->setVertexBuffer(vb);
	device_context->setIndexBuffer(ib);
	device_context->drawIndexedTriangleList(ib->getSizeIndices(), 0, 0);
}
