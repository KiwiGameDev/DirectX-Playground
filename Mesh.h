#pragma once
#include "Resource.h"
#include "VertexBuffer.h"
#include "Prerequisites.h"
#include <string>

class Mesh : public Resource
{
	friend class DeviceContext;

public:
	Mesh(const wchar_t* full_path);
	Mesh(const std::string& name, VertexBufferPtr vertex_buffer, IndexBufferPtr index_buffer);

	std::string getName() const;
	size_t getIndicesCount() const;
	const VertexBufferPtr& getVertexBuffer();
	const IndexBufferPtr& getIndexBuffer();
	
	~Mesh();

private:
	std::string m_name;
	
	VertexBufferPtr m_vertex_buffer;
	IndexBufferPtr m_index_buffer;
};
