#pragma once
#include "stdincl.h"

namespace Diamond {
	class VertexArray;

	enum class VertexAttributeType {
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
	static uint32_t vertexAttributeTypeToSize(VertexAttributeType type) {
		switch (type) {
			case VertexAttributeType::Bool: return 1;
			case VertexAttributeType::Float: return 4;
			case VertexAttributeType::Float2: return 4 * 2;
			case VertexAttributeType::Float3: return 4 * 3;
			case VertexAttributeType::Float4: return 4 * 4;
			case VertexAttributeType::UInt:
			case VertexAttributeType::Int: return 4;
			case VertexAttributeType::Int2:
			case VertexAttributeType::UInt2: return 4 * 2;
			case VertexAttributeType::Int3:
			case VertexAttributeType::UInt3: return 4 * 3;
			case VertexAttributeType::Int4:
			case VertexAttributeType::UInt4: return 4 * 4;
			case VertexAttributeType::Mat3:
			case VertexAttributeType::Mat4: return 4 * 4 * 4;
		}
		LOG_ERROR("[~cBuffers~x] Unknow VertexBufferDataType!");
		return 0;
	}

	static GLenum vertexAttributeTypeToOpenGLBaseType(VertexAttributeType type) {
		switch (type) {
			case VertexAttributeType::Bool: return GL_BOOL;
			case VertexAttributeType::Float:
			case VertexAttributeType::Float2:
			case VertexAttributeType::Float3:
			case VertexAttributeType::Float4:
			case VertexAttributeType::Mat3:
			case VertexAttributeType::Mat4: return GL_FLOAT;
			case VertexAttributeType::UInt: return GL_UNSIGNED_INT;
			case VertexAttributeType::Int:
			case VertexAttributeType::Int2:
			case VertexAttributeType::Int3:
			case VertexAttributeType::Int4: return GL_INT;
			case VertexAttributeType::UInt2:
			case VertexAttributeType::UInt3:
			case VertexAttributeType::UInt4: return GL_UNSIGNED_INT;
		}
		LOG_ERROR("[~cBuffers~x] Unknow VertexBufferDataType!");
		return 0;
	}

	struct BufferElement {
		std::string m_name;
		VertexAttributeType m_type;
		uint32_t m_size;
		uint32_t m_offset;
		uint32_t m_bufferIndex;
		bool m_divisor;
		bool m_normalized;

		BufferElement() = default;
		BufferElement(VertexAttributeType type, const std::string& name, uint32_t bufferIndex, bool divisor = false, bool normalized = false)
			: m_name(name), m_type(type), m_size(vertexAttributeTypeToSize(type)), m_offset(0), m_bufferIndex(bufferIndex), m_divisor(divisor), m_normalized(normalized) {
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
		BufferLayout(const std::initializer_list<BufferElement>& elements) : m_elements(elements) {
			calculateOffsetsAndStride();
		}
		BufferLayout() {}

		uint32_t getTotalComponentCountSize();
		void setVertexAttributeLocations(uint32_t attributeIndex);

		inline std::vector<BufferElement>::iterator begin() { return m_elements.begin(); }
		inline std::vector<BufferElement>::iterator end() { return m_elements.end(); }
		inline const std::vector<BufferElement>& getElements() const { return m_elements; }
		inline uint32_t getStride() const { return m_stride; }
	};
}