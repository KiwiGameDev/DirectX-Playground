#pragma once
#include "Vector3.h"
#include "Vector2.h"

struct VertexMesh
{
	VertexMesh() = default;
	VertexMesh(const Vector3& position, const Vector2& texcoord) : m_position(position), m_texcoord(texcoord) { }

private:
	Vector3 m_position;
	Vector2 m_texcoord;
};
