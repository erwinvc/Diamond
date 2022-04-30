#include "stdafx.h"
#include "graphics/vertexArray.h"
#include "graphics/buffers/vertexBuffer.h"
#include "GLError.h"

namespace Diamond {
	VertexArray::VertexArray() {
		GL(glGenVertexArrays(1, &m_handle));
	}
	VertexArray::~VertexArray() {
		GL(glDeleteVertexArrays(1, &m_handle));
	}

	void VertexArray::addBuffer(VertexBuffer* buffer) {
		for (auto existingBuffer : m_buffers) {
			if(buffer->getHandle() != existingBuffer->getHandle()) LOG_ERROR("VAO already contains buffer");
		}
		m_buffers.push_back(buffer);
	}

	void VertexArray::setVertexAttributeLocations() {
		uint32_t attributeIndex = 0;
		for (int i = 0; i < m_buffers.size(); i++) {
			m_buffers[i]->setVertexAttributeLocations(attributeIndex);
			attributeIndex += (uint32_t)m_buffers[i]->getLayout().getElements().size();
		}
	}

	void VertexArray::bind() {
		GL(glBindVertexArray(m_handle));
	}
}