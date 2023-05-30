#pragma once
#include <GL/glew.h>

class IndexBuffer
{
public:
	IndexBuffer(const GLuint* data, GLuint count);
	~IndexBuffer();

	void bind() const;
	void unbind() const;
	
	GLuint get_count()const;

private:
	GLuint m_ID;
	GLuint m_Count;
};
