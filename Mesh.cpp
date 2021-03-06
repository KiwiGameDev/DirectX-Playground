#include "Mesh.h"
#include "VertexMesh.h"
#include "GraphicsEngine.h"
#include "RenderSystem.h"
#include "VertexShaderManager.h"
#include "IndexBuffer.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include <locale>

Mesh::Mesh(const std::string& file_path)
	: Resource(file_path)
{
	tinyobj::attrib_t attribs;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn;
	std::string err;
	
	bool res = LoadObj(&attribs, &shapes, &materials, &warn, &err, file_path.c_str());

	if (!err.empty())
	{
		throw std::exception("Mesh failed to load successfully!");
	}

	if (!res)
	{
		throw std::exception("Mesh failed to load successfully!");
	}

	std::vector<VertexMesh> list_vertices;
	std::vector<unsigned int> list_indices;

	for (const tinyobj::shape_t& shape : shapes)
	{
		size_t index_offset = 0;

		list_vertices.reserve(shape.mesh.indices.size());
		list_indices.reserve(shape.mesh.indices.size());
		
		for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++)
		{
			unsigned char num_face_verts = shape.mesh.num_face_vertices[f];

			for (unsigned char v = 0; v < num_face_verts; v++)
			{
				tinyobj::index_t index = shape.mesh.indices[index_offset + v];
				tinyobj::real_t vx = attribs.vertices[index.vertex_index * 3 + 0];
				tinyobj::real_t vy = attribs.vertices[index.vertex_index * 3 + 1];
				tinyobj::real_t vz = attribs.vertices[index.vertex_index * 3 + 2];
				
				tinyobj::real_t tx = attribs.texcoords[index.texcoord_index * 2 + 0];
				tinyobj::real_t ty = attribs.texcoords[index.texcoord_index * 2 + 1];

				list_vertices.push_back(VertexMesh({ vx, vy, vz }, { tx, ty }));
				list_indices.push_back(index_offset + v);
			}

			index_offset += num_face_verts;
		}
	}
	
	VertexShaderPtr vs = GraphicsEngine::get().getVertexShaderManager().getVertexShaderFromFile("TexturedVertexShader.hlsl");
	m_vertex_buffer = GraphicsEngine::get().getRenderSystem().createVertexBuffer(&list_vertices[0], sizeof(VertexMesh), list_vertices.size(), vs, VertexFormat::POSITION_UV);
	m_index_buffer = GraphicsEngine::get().getRenderSystem().createIndexBuffer(&list_indices[0], (UINT)list_indices.size());
}

Mesh::Mesh(const std::string& name, VertexBufferPtr vertex_buffer, IndexBufferPtr index_buffer)
	: Resource(name), m_vertex_buffer(vertex_buffer), m_index_buffer(index_buffer)
{
	
}

size_t Mesh::getIndicesCount() const
{
	return m_index_buffer->getIndicesCount();
}

const VertexBufferPtr& Mesh::getVertexBuffer()
{
	return m_vertex_buffer;
}

const IndexBufferPtr& Mesh::getIndexBuffer()
{
	return m_index_buffer;
}

Mesh::~Mesh()
{
	
}
