#pragma once
#include <GL/glew.h>

class VertexBuffer
{
	public:
		VertexBuffer(const void* data, GLuint size);
		~VertexBuffer();

		void bind()const;
		void unbind()const;

	private:
		GLuint m_ID;
};
