#pragma once
#include "src/graphics/buffers/vertexBuffer.h"
#include "ref.h"

namespace Diamond {

	class VertexArray : public Ref {
	private:
		GLuint m_handle;
		std::vector<VertexBuffer*> m_buffers;

	public:
		VertexArray();
		~VertexArray();

		void addBuffer(VertexBuffer* buffer);
		void setVertexAttributeLocations();

		void bind();

		inline uint32_t getBufferCount() { return (uint32_t)m_buffers.size(); }
		inline std::vector<VertexBuffer*>& getBuffers() { return m_buffers; }
		inline VertexBuffer* getBuffer(int index) { return m_buffers.at(index); }
	};
}