#pragma once
#include "vertexBufferLayout.h"
#include "buffer.h"
#include "GLError.h"

namespace Diamond {
	class VertexBuffer {
	private:
		uint32_t m_handle;
		uint32_t m_vertexCount;
		BufferLayout m_layout;
		BufferUsage m_usage;
		Buffer m_data;

	public:
		VertexBuffer() = default;
		VertexBuffer(const void* data, uint32_t vertexCount, BufferLayout layout, BufferUsage usage = STATIC_DRAW);
		VertexBuffer(uint32_t vertexCount, BufferLayout layout, BufferUsage usage);
		~VertexBuffer();

		inline const BufferLayout& getLayout() { return m_layout; }
		inline void setLayout(BufferLayout& layout) { m_layout = layout; }
		void setVertexAttributeLocations(uint32_t attributeIndex);

		void setData(const void* data, uint32_t size, uint32_t offset = 0, BufferUsage usage = BufferUsage::STATIC_DRAW);

		inline uint32_t getHandle() { return m_handle; }
		inline uint32_t getVertexCount() { return m_vertexCount; }

		inline void bind() {
			GL(glBindBuffer(GL_ARRAY_BUFFER, m_handle));
		}

		template<typename T>
		T* map() {
			GL(T * toRet = (T*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY | GL_MAP_UNSYNCHRONIZED_BIT));
			return toRet;
		}

		void unmap() {
			GL(glUnmapBuffer(GL_ARRAY_BUFFER));
		}
	};
}