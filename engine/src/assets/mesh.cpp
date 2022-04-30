#include "stdafx.h"
#include "src/assets/mesh.h"
#include "src/graphics/vertexArray.h"
#include "src/graphics/buffers/indexBuffer.h"

namespace Diamond {
	Mesh::Mesh(const std::string& name, const std::string& fileName) : Asset(name), m_vao(new VertexArray()), m_ibo(new IndexBuffer(0, DYNAMIC_DRAW)) {
		m_vao->addBuffer(new VertexBuffer(0, m_layout, DYNAMIC_DRAW));
		//m_material = GetMaterialManager()->GetBasicMaterial(GetShaderManager()->Get("Geometry"));
	}

	Mesh::Mesh(const std::string& name, void* vertices, uint32_t vertexCount, uint32_t vertexSize, void* indices, uint32_t indexCount) : Asset(name), m_vertexCount(vertexCount) {
		m_vertices = Buffer::copy(vertices, vertexSize * vertexCount);

		m_vao = new VertexArray();
		m_vao->addBuffer(new VertexBuffer(m_vertices.getData(), m_vertices.getSize(), m_layout, DYNAMIC_DRAW));
		m_vao->bind();
		m_vao->setVertexAttributeLocations();

		m_ibo = new IndexBuffer(indices, indexCount * sizeof(uint32_t), DYNAMIC_DRAW);
	}

	void Mesh::bind() {
		m_vao->bind();
		m_ibo->bind();
	}

	//void Mesh::setIndices(void* indices, uint32_t count) {
	//	m_ibo->bind();
	//	m_ibo->setData(indices, count * sizeof(uint32_t), 0);
	//}
	//
	//void Mesh::setVertices(void* data, uint32_t size) {
	//	m_vertices = Buffer::copy(data, size);
	//	m_vao->getBuffer(0)->bind();
	//	m_vao->getBuffer(0)->setData(m_vertices.getData(), m_vertices.getSize(), 0);
	//}
}