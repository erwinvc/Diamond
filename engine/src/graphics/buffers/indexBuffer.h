#pragma once
#include "buffer.h"

namespace Diamond {
	class IndexBuffer {
	private:
		uint32_t m_handle = 0;
		uint32_t m_size;
		Buffer m_data;

	public:
		IndexBuffer(uint32_t size, BufferUsage usage = STATIC_DRAW);
		IndexBuffer(void* data, uint32_t size, BufferUsage usage = STATIC_DRAW);
		~IndexBuffer();

		void setData(void* data, uint32_t size, uint32_t offset);
		void bind() const;

		uint32_t getHandle() const { return m_handle; }
		uint32_t getCount() const { return m_size / sizeof(uint32_t); }
		uint32_t getSize() const { return m_size; }
	};
}