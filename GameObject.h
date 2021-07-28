#pragma once
#include "Vector3.h"
#include "ConstantBufferData.h"
#include "Prerequisites.h"
#include <string>

class GameObject
{
public:
	GameObject(const std::string& name, VertexBufferPtr vertex_buffer, IndexBufferPtr index_buffer, VertexShaderPtr vertex_shader, PixelShaderPtr pixel_shader);;

	virtual void update(float deltaTime) = 0;
	void draw(const ConstantBufferPtr& cb, ConstantBufferData& cbd);

	void setPosition(float x, float y, float z);
	void setPosition(const Vector3& newPosition);
	Vector3 getLocalPosition() const;

	void setScale(float x, float y, float z);
	void setScale(const Vector3& newScale);
	Vector3 getLocalScale() const;

	void setRotation(float x, float y, float z);
	void setRotation(const Vector3& newRotation);
	Vector3 getLocalRotation() const;

	virtual ~GameObject();

protected:
	Vector3 position;
	Vector3 rotation;
	Vector3 scale;

private:
	std::string name;

	VertexBufferPtr vb;
	IndexBufferPtr ib;
	VertexShaderPtr vs;
	PixelShaderPtr ps;
};
