#pragma once
#include <GL/glew.h>
#include "VertexBuffer.h"


class VertexArray
{
	public:
		VertexArray();
		~VertexArray();

		void link_attrib(const VertexBuffer& vb, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, const void* offset)const;

		void bind()const;
		void unbind()const;

	private:
		GLuint m_ID;
};