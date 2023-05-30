#include "Cube.h"

const std::vector<glm::vec4> Cube::vertices = {
	  glm::vec4{-0.5f, -0.5f, -0.5f, 1.f}, // A 0
	  glm::vec4{ 0.5f, -0.5f, -0.5f, 1.f}, // B 1
	  glm::vec4{ 0.5f,  0.5f, -0.5f, 1.f}, // C 2
	  glm::vec4{-0.5f,  0.5f, -0.5f, 1.f}, // D 3
	  glm::vec4{-0.5f, -0.5f,  0.5f, 1.f}, // E 4
	  glm::vec4{ 0.5f, -0.5f,  0.5f, 1.f}, // F 5
	  glm::vec4{ 0.5f,  0.5f,  0.5f, 1.f}, // G 6
	  glm::vec4{-0.5f,  0.5f,  0.5f, 1.f}, // H 7
	  
	  glm::vec4{-0.5f,  0.5f, -0.5f, 1.f}, // D 8
	  glm::vec4{-0.5f, -0.5f, -0.5f, 1.f}, // A 9
	  glm::vec4{-0.5f, -0.5f,  0.5f, 1.f}, // E 10
	  glm::vec4{-0.5f,  0.5f,  0.5f, 1.f}, // H 11
	  glm::vec4{ 0.5f, -0.5f, -0.5f, 1.f}, // B 12
	  glm::vec4{ 0.5f,  0.5f, -0.5f, 1.f}, // C 13
	  glm::vec4{ 0.5f,  0.5f,  0.5f, 1.f}, // G 14
	  glm::vec4{ 0.5f, -0.5f,  0.5f, 1.f}, // F 15
	  
	  glm::vec4{-0.5f, -0.5f, -0.5f, 1.f}, // A 16
	  glm::vec4{ 0.5f, -0.5f, -0.5f, 1.f}, // B 17
	  glm::vec4{ 0.5f, -0.5f,  0.5f, 1.f}, // F 18
	  glm::vec4{-0.5f, -0.5f,  0.5f, 1.f}, // E 19
	  glm::vec4{ 0.5f,  0.5f, -0.5f, 1.f}, // C 20
	  glm::vec4{-0.5f,  0.5f, -0.5f, 1.f}, // D 21
	  glm::vec4{-0.5f,  0.5f,  0.5f, 1.f}, // H 22
	  glm::vec4{ 0.5f,  0.5f,  0.5f, 1.f}  // G 23
};


const std::vector<GLuint> Cube::indices = {
	// front and back
	0, 3, 2,
	2, 1, 0,
	4, 5, 6,
	6, 7 ,4,
	// left and right
	11, 8, 9,
	9, 10, 11,
	12, 13, 14,
	14, 15, 12,
	// bottom and top
	16, 17, 18,
	18, 19, 16,
	20, 21, 22,
	22, 23, 20
};


/*
const std::vector<GLuint> Cube::indices = {
	3, 5, 2,
	3, 0, 2,
	5, 7, 2,
	2, 4, 7,
	3, 0, 1,
	3, 6, 1,
	3, 5, 7,
	3, 6, 7,
	6, 1, 7,
	1, 4, 7,
	0, 2, 4,
	1, 4, 2
};
*/

/*
const std::vector<glm::vec4> Cube::vertices = {
		glm::vec4( 0.25f,  0.25f,  0.25f, 1.f),
		glm::vec4( 0.25f,  0.25f, -0.25f, 1.f),
		glm::vec4( 0.25f, -0.25f,  0.25f, 1.f),
		glm::vec4(-0.25f,  0.25f,  0.25f, 1.f),
		glm::vec4( 0.25f, -0.25f, -0.25f, 1.f),
		glm::vec4(-0.25f, -0.25f,  0.25f, 1.f),
		glm::vec4(-0.25f,  0.25f, -0.25f, 1.f),
		glm::vec4(-0.25f, -0.25f, -0.25f, 1.f)
};
*/


Cube::Cube()
	:vb(Cube::vertices.data(), Cube::vertices.size() * sizeof(glm::vec4)), ib(Cube::indices.data(), Cube::indices.size())
{
	va.link_attrib(vb, 0, 4, GL_FLOAT, sizeof(glm::vec4), (const void*)0);
	va.unbind();
	vb.unbind();
	ib.unbind();
}


void Cube::draw(const Shader& shader, glm::vec3 pos, glm::vec4 color, GLfloat rot, const Camera& camera, GLfloat size = 1.f)
{
	shader.bind();

	shader.set_uniform_vec4f("u_Color", color);
	glm::mat4 model = glm::mat4(1.f);
	glm::mat4 mvp = camera.cameraMatrix * glm::translate(glm::rotate(glm::scale(model, glm::vec3(size,size,size)), rot, camera.Up), pos);
	shader.set_uniform_mat4f("u_MVP", mvp);

	va.bind();
	ib.bind();

	glDrawElements(GL_TRIANGLES, ib.get_count(), GL_UNSIGNED_INT, nullptr);
}