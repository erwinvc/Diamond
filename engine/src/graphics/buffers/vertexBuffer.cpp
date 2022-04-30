#include "stdafx.h"
#include "vertexBuffer.h"

namespace Diamond {
	VertexBuffer::VertexBuffer(const void* data, uint32_t vertexCount, BufferLayout layout, BufferUsage usage)
		: m_layout(layout) {
		GL(glCreateBuffers(1, &m_handle));
		setData(data, vertexCount, 0, usage);
	}

	VertexBuffer::VertexBuffer(uint32_t vertexCount, BufferLayout layout, BufferUsage usage)
		: m_layout(layout) {
		GL(glCreateBuffers(1, &m_handle));
		setData(nullptr, vertexCount, 0, usage);
	}

	VertexBuffer::~VertexBuffer() {
		GL(glDeleteBuffers(1, &m_handle));
	}

	void VertexBuffer::setVertexAttributeLocations(uint32_t attributeIndex) {
		bind();
		m_layout.setVertexAttributeLocations(attributeIndex);
	}

	void VertexBuffer::setData(const void* data, uint32_t vertexCount, uint32_t offset, BufferUsage usage) {
		bind();
		m_vertexCount = vertexCount;
		m_usage = usage;

		uint32_t size = vertexCount * m_layout.getTotalComponentCountSize();
		if (data != nullptr) {
			m_data = Buffer::copy(data, size);
		}
		GL(glNamedBufferData(m_handle, size, m_data.getData(), usage));
	}
}