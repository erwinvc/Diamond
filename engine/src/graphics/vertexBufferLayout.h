#pragma once
#include "stdincl.h"

namespace Diamond {
	class VertexArray;

	enum class VertexBufferDataType {
		None = 0,
		Bool,
		Float,
		Float2,
		Float3,
		Float4,
		UInt,
		Int,
		Int2,
		UInt2,
		Int3,
		UInt3,
		Int4,
		UInt4,
		Mat3,
		Mat4
	};

	//#TODO: Move these two functions to a better place
	static uint32_t vertexBufferDataTypeToSize(VertexBufferDataType type) {
		switch (type) {
			case VertexBufferDataType::Bool: return 1;
			case VertexBufferDataType::Float: return 4;
			case VertexBufferDataType::Float2: return 4 * 2;
			case VertexBufferDataType::Float3: return 4 * 3;
			case VertexBufferDataType::Float4: return 4 * 4;
			case VertexBufferDataType::UInt:
			case VertexBufferDataType::Int: return 4;
			case VertexBufferDataType::Int2:
			case VertexBufferDataType::UInt2: return 4 * 2;
			case VertexBufferDataType::Int3:
			case VertexBufferDataType::UInt3: return 4 * 3;
			case VertexBufferDataType::Int4:
			case VertexBufferDataType::UInt4: return 4 * 4;
			case VertexBufferDataType::Mat3:
			case VertexBufferDataType::Mat4: return 4 * 4 * 4;
		}
		LOG_ERROR("[~cBuffers~x] Unknow VertexBufferDataType!");
		return 0;
	}

	static GLenum vertexBufferDataTypeToOpenGLBaseType(VertexBufferDataType type) {
		switch (type) {
			case VertexBufferDataType::Bool: return GL_BOOL;
			case VertexBufferDataType::Float:
			case VertexBufferDataType::Float2:
			case VertexBufferDataType::Float3:
			case VertexBufferDataType::Float4:
			case VertexBufferDataType::Mat3:
			case VertexBufferDataType::Mat4: return GL_FLOAT;
			case VertexBufferDataType::UInt: return GL_UNSIGNED_INT;
			case VertexBufferDataType::Int:
			case VertexBufferDataType::Int2:
			case VertexBufferDataType::Int3:
			case VertexBufferDataType::Int4: return GL_INT;
			case VertexBufferDataType::UInt2:
			case VertexBufferDataType::UInt3:
			case VertexBufferDataType::UInt4: return GL_UNSIGNED_INT;
		}
		LOG_ERROR("[~cBuffers~x] Unknow VertexBufferDataType!");
		return 0;
	}

	struct BufferElement {
		std::string m_name;
		VertexBufferDataType m_type;
		uint32_t m_size;
		uint32_t m_offset;
		uint32_t m_bufferIndex;
		bool m_divisor;
		bool m_normalized;

		BufferElement() = default;
		BufferElement(VertexBufferDataType type, const std::string& name, uint32_t bufferIndex, bool divisor = false, bool normalized = false)
			: m_name(name), m_type(type), m_size(vertexBufferDataTypeToSize(type)), m_offset(0), m_bufferIndex(bufferIndex), m_divisor(divisor), m_normalized(normalized) {
		}

		uint32_t getComponentCount() const;
		bool isVertexAttribI() const;
	};

	class BufferLayout {
	private:
		uint32_t m_stride = 0;
		std::vector<BufferElement> m_elements;

		void calculateOffsetsAndStride() {
			uint32_t offset = 0;
			m_stride = 0;
			for (auto& element : m_elements) {
				element.m_offset = offset;
				offset += element.m_size;
				m_stride += element.m_size;
			}
		}

	public:
		BufferLayout(const std::initializer_list<BufferElement>& elements) {
			calculateOffsetsAndStride();
		}
		BufferLayout() {}

		uint32_t getTotalComponentCountSize();

		inline std::vector<BufferElement>::iterator begin() { return m_elements.begin(); }
		inline std::vector<BufferElement>::iterator end() { return m_elements.end(); }
		inline const std::vector<BufferElement>& getElements() const { return m_elements; }
		inline uint32_t getStride() const { return m_stride; }
	};
}