#include "stdafx.h"
#include "indexBuffer.h"
#include "GLError.h"

namespace Diamond {
	IndexBuffer::IndexBuffer(uint32_t size, BufferUsage usage) : m_size(size) {
		GL(glCreateBuffers(1, &m_handle));
		GL(glNamedBufferData(m_handle, m_size, nullptr, (GLuint)usage));
	}

	IndexBuffer::IndexBuffer(void* data, uint32_t size, BufferUsage usage) : m_size(size) {
		GL(glCreateBuffers(1, &m_handle));
		m_data = Buffer::copy(data, size);
		GL(glNamedBufferData(m_handle, m_data.getSize(), m_data.getData(), (GLuint)usage));
	}

	IndexBuffer::~IndexBuffer() {
		GL(glDeleteBuffers(1, &m_handle));
	}

	void IndexBuffer::setData(void* data, uint32_t size, uint32_t offset) {
		m_data = Buffer::copy(data, size);
		m_size = size;
		GL(glNamedBufferSubData(m_handle, offset, m_data.getSize(), m_data.getData()));
	}

	void IndexBuffer::bind() const {
		GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handle));
	}
}