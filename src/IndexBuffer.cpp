#include "IndexBuffer.h"


IndexBuffer::IndexBuffer(const GLuint* data, GLuint count)
    : m_ID(0), m_Count(count)
{
    glGenBuffers(1, &m_ID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_STATIC_DRAW);
}
IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, &m_ID);
}

GLuint IndexBuffer::get_count()const
{
    return this->m_Count;
}

void IndexBuffer::bind()const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
}
void IndexBuffer::unbind()const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}