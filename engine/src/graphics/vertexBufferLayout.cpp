#include "stdafx.h"
#include "vertexBufferLayout.h"

namespace Diamond {
		uint32_t BufferLayout::getTotalComponentCountSize() {
		uint32_t count = 0;
		for (BufferElement& element : m_elements) count += element.getComponentCount() * sizeof(vertexBufferDataTypeToOpenGLBaseType(element.m_type));
		return count;
	}

	uint32_t BufferElement::getComponentCount() const {
		switch (m_type) {
			case VertexBufferDataType::Bool: return 1;
			case VertexBufferDataType::Float: return 1;
			case VertexBufferDataType::Float2: return 2;
			case VertexBufferDataType::Float3: return 3;
			case VertexBufferDataType::Float4: return 4;
			case VertexBufferDataType::UInt: return 1;
			case VertexBufferDataType::Int: return 1;
			case VertexBufferDataType::Int2: return 2;
			case VertexBufferDataType::UInt2: return 2;
			case VertexBufferDataType::Int3: return 3;
			case VertexBufferDataType::UInt3: return 3;
			case VertexBufferDataType::Int4: return 4;
			case VertexBufferDataType::UInt4: return 4;
			case VertexBufferDataType::Mat3: return 9;
			case VertexBufferDataType::Mat4: return 16;
		}
		LOG_ERROR("[~cBuffers~x] Unknow VertexBufferDataType!");
		return 0;
	}

	bool BufferElement::isVertexAttribI() const {
		switch (m_type) {
			case VertexBufferDataType::Bool:
			case VertexBufferDataType::Float:
			case VertexBufferDataType::Float2:
			case VertexBufferDataType::Float3:
			case VertexBufferDataType::Float4: return false;
			case VertexBufferDataType::UInt:
			case VertexBufferDataType::Int:
			case VertexBufferDataType::Int2:
			case VertexBufferDataType::Int3:
			case VertexBufferDataType::Int4:
			case VertexBufferDataType::UInt2:
			case VertexBufferDataType::UInt3:
			case VertexBufferDataType::UInt4: return true;
		}
		LOG_ERROR("[~cBuffers~x] Unknow VertexBufferDataType!");
		return 0;
	}
}