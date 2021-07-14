#include "GameObject.h"
#include "GraphicsEngine.h"
#include "RenderSystem.h"
#include "DeviceContext.h"
#include "ConstantBuffer.h"
#include "IndexBuffer.h"

GameObject::GameObject(VertexBufferPtr vb, IndexBufferPtr ib, ConstantBufferPtr cb, VertexShaderPtr vs, PixelShaderPtr ps)
	: vb(vb), ib(ib), cb(cb), vs(vs), ps(ps), scale(1.0f, 1.0f, 1.0f)
{
	
}

void GameObject::render(constant cc)
{
	DeviceContextPtr ctx = GraphicsEngine::get().getRenderSystem()->getImmediateDeviceContext();

	cc.m_world = Matrix4x4::identity();
	cc.m_world *= Matrix4x4::scale(scale);
	cc.m_world *= Matrix4x4::rotationX(rotation.x);
	cc.m_world *= Matrix4x4::rotationY(rotation.y);
	cc.m_world *= Matrix4x4::rotationZ(rotation.z);
	cc.m_world *= Matrix4x4::translation(position);
	cb->update(ctx, &cc);
	
	ctx->setConstantBuffer(vs, cb);
	ctx->setConstantBuffer(ps, cb);
	ctx->setVertexBuffer(vb);
	ctx->setIndexBuffer(ib);
	ctx->setVertexShader(vs);
	ctx->setPixelShader(ps);
	ctx->drawIndexedTriangleList(ib->getSizeIndices(), 0, 0);
}
