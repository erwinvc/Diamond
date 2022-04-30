#include "stdafx.h"
#include "vertexBufferLayout.h"
#include "src/GLError.h"

namespace Diamond {
	uint32_t BufferLayout::getTotalComponentCountSize() {
		uint32_t count = 0;
		for (BufferElement& element : m_elements) count += element.getComponentCount() * sizeof(vertexAttributeTypeToOpenGLBaseType(element.m_type));
		return count;
	}

	uint32_t BufferElement::getComponentCount() const {
		switch (m_type) {
			case VertexAttributeType::Bool: return 1;
			case VertexAttributeType::Float: return 1;
			case VertexAttributeType::Float2: return 2;
			case VertexAttributeType::Float3: return 3;
			case VertexAttributeType::Float4: return 4;
			case VertexAttributeType::UInt: return 1;
			case VertexAttributeType::Int: return 1;
			case VertexAttributeType::Int2: return 2;
			case VertexAttributeType::UInt2: return 2;
			case VertexAttributeType::Int3: return 3;
			case VertexAttributeType::UInt3: return 3;
			case VertexAttributeType::Int4: return 4;
			case VertexAttributeType::UInt4: return 4;
			case VertexAttributeType::Mat3: return 9;
			case VertexAttributeType::Mat4: return 16;
		}
		LOG_ERROR("[~cBuffers~x] Unknow VertexBufferDataType!");
		return 0;
	}

	bool BufferElement::isVertexAttribI() const {
		switch (m_type) {
			case VertexAttributeType::Bool:
			case VertexAttributeType::Float:
			case VertexAttributeType::Float2:
			case VertexAttributeType::Float3:
			case VertexAttributeType::Float4: return false;
			case VertexAttributeType::UInt:
			case VertexAttributeType::Int:
			case VertexAttributeType::Int2:
			case VertexAttributeType::Int3:
			case VertexAttributeType::Int4:
			case VertexAttributeType::UInt2:
			case VertexAttributeType::UInt3:
			case VertexAttributeType::UInt4: return true;
		}
		LOG_ERROR("[~cBuffers~x] Unknow VertexBufferDataType!");
		return 0;
	}

	void BufferLayout::setVertexAttributeLocations(uint32_t attributeIndex) {
		uint32_t index = attributeIndex;
		for (BufferElement& element : m_elements) {
			GL(glEnableVertexAttribArray(index));
			if (element.isVertexAttribI()) {
				GL(glVertexAttribIPointer(index, element.getComponentCount(), vertexAttributeTypeToOpenGLBaseType(element.m_type), m_stride, (const void*)(uint64_t)element.m_offset));
			} else {
				GL(glVertexAttribPointer(index, element.getComponentCount(), vertexAttributeTypeToOpenGLBaseType(element.m_type), element.m_normalized, m_stride, (const void*)(uint64_t)element.m_offset));
			}
			if (element.m_divisor) {
				GL(glVertexAttribDivisor(index, 1));
			}
			index++;
		}
	}
}