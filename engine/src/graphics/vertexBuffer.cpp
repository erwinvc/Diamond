#include "stdafx.h"
#include "vertexBuffer.h"

namespace Diamond {
	VertexBuffer::VertexBuffer(const void* data, uint32_t vertexCount, BufferLayout layout, BufferUsage usage)
		: m_layout(layout) {
		glCreateBuffers(1, &m_handle);
		SetData(data, vertexCount, 0, usage);
	}

	VertexBuffer::VertexBuffer(uint32_t vertexCount, BufferLayout layout, BufferUsage usage)
		: m_layout(layout) {
		glCreateBuffers(1, &m_handle);
		SetData(nullptr, vertexCount, 0, usage);
	}

	VertexBuffer::~VertexBuffer() {
		glDeleteBuffers(1, &m_handle);
	}

	void VertexBuffer::SetData(const void* data, uint32_t vertexCount, uint32_t offset, BufferUsage usage) {
		m_vertexCount = vertexCount;
		m_usage = usage;

		uint32_t size = vertexCount * m_layout.getTotalComponentCountSize();
		if (data != nullptr) {
			m_data = Buffer::copy(data, size);
		}
		glNamedBufferData(m_handle, size, m_data.getData(), usage);
	}
}