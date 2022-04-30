#pragma once
#include "assets/asset.h"
#include "graphics/buffers/vertexBufferLayout.h"
#include "buffer.h"

namespace Diamond {
	class VertexArray;
	class IndexBuffer;
	class Material;

	struct Submesh {
		uint32_t m_baseVertex;
		uint32_t m_baseIndex;
		uint32_t m_materialIndex;
		uint32_t m_indexCount;
		uint32_t m_vertexCount;
	};

	struct StandardVertex {
		glm::vec3 m_position;
		glm::vec3 m_normal;
		glm::vec2 m_uv;
		glm::vec3 m_tangent;
		glm::vec3 m_bitangent;

		StandardVertex(glm::vec3 position, glm::vec3 normal, glm::vec2 uv) 
			: m_position(position), m_normal(normal), m_uv(uv), m_tangent(glm::vec3(0)), m_bitangent(glm::vec3(0)) {}
		
		StandardVertex(glm::vec3 position, glm::vec3 normal, glm::vec2 uv, glm::vec3 tangent, glm::vec3 bitangent) 
			: m_position(position), m_normal(normal), m_uv(uv), m_tangent(tangent), m_bitangent(bitangent) {}
	};

	class Mesh : public Asset {
	private:
		BufferLayout m_layout = {
			{VertexAttributeType::Float3, "vsPosition", 0},
			{VertexAttributeType::Float3, "vsNormal", 0},
			{VertexAttributeType::Float2, "vsUv", 0},
			{VertexAttributeType::Float3, "vsTangent", 0},
			{VertexAttributeType::Float3, "vsBitangent", 0}
		};

		VertexArray* m_vao;
		IndexBuffer* m_ibo;

		Buffer m_vertices;
		uint32_t m_vertexCount;

		Material* m_material;

		std::vector<Submesh> m_subMeshes;

	public:
		//glm::vec3 m_size;
		//glm::vec3 m_center;

		Mesh(const std::string& name, const std::string& fileName);
		Mesh(const std::string& name, void* vertices, uint32_t vertexCount, uint32_t vertexSize, void* indices, uint32_t indexCount);
		virtual ~Mesh() {}

		void bind();


		void setLayout(BufferLayout layout) { m_layout = layout; }
		//void setIndices(void* indices, uint32_t count);
		//void setVertices(void* data, uint32_t size);

		void addSubmesh(uint32_t baseVertex, uint32_t baseIndex, uint32_t materialIndex, uint32_t indexCount, uint32_t vertexCount) {
			Submesh submesh;
			submesh.m_baseVertex = baseVertex;
			submesh.m_baseIndex = baseIndex;
			submesh.m_materialIndex = materialIndex;
			submesh.m_indexCount = indexCount;
			submesh.m_vertexCount = vertexCount;
			m_subMeshes.push_back(submesh);
		}

		//Mesh Copy();

		inline VertexArray* getVAO() { return m_vao; }
		inline IndexBuffer* getIBO() { return m_ibo; }
		inline Material* GetMaterial() { return m_material; }
		inline void setMaterial(Material* mat) { m_material = mat; }
	};

}