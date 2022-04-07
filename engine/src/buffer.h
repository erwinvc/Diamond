#pragma once
#include "stdincl.h"

namespace Diamond {
	enum BufferUsage {
		STATIC_DRAW = 0x88E4,	//GL_STATIC_DRAW
		DYNAMIC_DRAW = 0x88E8,	//GL_DYNAMIC_DRAW
		STREAM_DRAW = 0x88E0	//GL_STREAM_DRAW 
	};

	class Buffer {
	private:
		void* m_data;
		uint32_t m_size;

	public:

		Buffer()
			: m_data(nullptr), m_size(0) {
		}

		Buffer(void* _data, uint32_t _size)
			: m_data(_data), m_size(_size) {

		}

		static Buffer copy(const void* data, uint32_t size) {
			Buffer buffer;
			buffer.allocate(size);
			memcpy(buffer.m_data, data, size);
			return buffer;
		}

		void allocate(uint32_t size) {
			delete[] m_data;
			m_data = nullptr;
		
			if (size == 0) return;

			m_data = new byte[size];
			m_size = size;
		}

		void cleanup() {
			delete[] m_data;
			m_data = nullptr;
			m_size = 0;
		}

		operator bool() const {
			return m_data;
		}

		inline uint32_t getSize() const { return m_size; }
		inline void* getData() const { return m_data; }
	};
}