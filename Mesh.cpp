#include "Mesh.h"
#include "VertexMesh.h"
#include "GraphicsEngine.h"
#include "RenderSystem.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include <locale>
#include <codecvt>

Mesh::Mesh(const wchar_t* full_path)
	: Resource(full_path)
{
	tinyobj::attrib_t attribs;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn;
	std::string err;

	int size_needed = WideCharToMultiByte(CP_UTF8, 0, &full_path[0], wcslen(full_path), NULL, 0, NULL, NULL);
	std::string input_file(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, &full_path[0], wcslen(full_path), &input_file[0], size_needed, NULL, NULL);

	bool res = LoadObj(&attribs, &shapes, &materials, &warn, &err, input_file.c_str());

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

	void* shader_byte_code = nullptr;
	size_t size_shader_byte_code = 0;
	GraphicsEngine::get().getVertexMeshLayoutShaderByteCodeAndSize(&shader_byte_code, &size_shader_byte_code);
	m_vertex_buffer = GraphicsEngine::get().getRenderSystem()->createVertexBuffer(
		&list_vertices[0],
		sizeof(VertexMesh),
		list_vertices.size(),
		shader_byte_code,
		size_shader_byte_code);
	m_index_buffer = GraphicsEngine::get().getRenderSystem()->createIndexBuffer(&list_indices[0], (UINT)list_indices.size());
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