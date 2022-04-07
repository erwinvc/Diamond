#include "stdafx.h"
#include "indexBuffer.h"

namespace Diamond {

	IndexBuffer::IndexBuffer(uint32_t size, BufferUsage usage) : m_size(size) {
		glCreateBuffers(1, &m_handle);
		glNamedBufferData(m_handle, m_size, nullptr, (GLuint)usage);
	}

	IndexBuffer::IndexBuffer(void* data, uint32_t size, BufferUsage usage) : m_size(size) {
		m_data = Buffer::copy(data, size);
		glNamedBufferData(m_handle, m_size, m_data.getData(), (GLuint)usage);
	}


	IndexBuffer::~IndexBuffer() {
		glDeleteBuffers(1, &m_handle);
	}

	void IndexBuffer::SetData(void* data, uint32_t size, uint32_t offset, BufferUsage usage) {
		m_data = Buffer::copy(data, size);
		m_size = size;
		glNamedBufferSubData(m_handle, offset, m_size, m_data.getData());
	}

	void IndexBuffer::Bind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handle);
	}
}