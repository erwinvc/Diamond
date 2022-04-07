#pragma once
#include "vertexBufferLayout.h"
#include "buffer.h"

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
		void SetData(const void* data, uint32_t size, uint32_t offset = 0, BufferUsage usage = BufferUsage::STATIC_DRAW);

		inline uint32_t getHandle() { return m_handle; }
		inline uint32_t getVertexCount() { return m_vertexCount; }

		inline void bind() {
			glBindBuffer(GL_ARRAY_BUFFER, m_handle);
		}

		template<typename T>
		T* map() {
			bind();
			return (T*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY | GL_MAP_UNSYNCHRONIZED_BIT);
		}

		void unmap() {
			bind();
			glUnmapBuffer(GL_ARRAY_BUFFER);
		}
	};
}