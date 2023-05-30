#pragma once
#include <vector>
#include <GL/glew.h>
#include "Camera.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"


class Cube
{
	public:
		Cube();
		void draw(const Shader& shader, glm::vec3 pos, glm::vec4 color, GLfloat rot, const Camera& camera, GLfloat size);
	private:
		VertexArray va;
		VertexBuffer vb;
		IndexBuffer ib;
		static const std::vector<glm::vec4> vertices;
		static const std::vector<GLuint> indices;
};