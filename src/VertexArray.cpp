#include "VertexArray.h"



VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_ID);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_ID);
}

void VertexArray::link_attrib(const VertexBuffer& vb, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, const void* offset)const
{
	this->bind();
	vb.bind();
	glEnableVertexAttribArray(layout);
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
}

void VertexArray::bind()const
{
	glBindVertexArray(m_ID);
}

void VertexArray::unbind()const
{
	glBindVertexArray(0);
}