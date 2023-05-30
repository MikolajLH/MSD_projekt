#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <GLFW/glfw3.h>


class Camera
{
public:
	glm::vec3 Position;
	glm::vec3 Orientation = glm::vec3(0.f, 0.f, -1.f);
	glm::vec3 Up = glm::vec3(0.f, 1.f, 0.f);
	glm::mat4 cameraMatrix = glm::mat4(1.f);

	bool firstClick = true;

	int width;
	int height;

	float speed = 0.1f;
	float sensitivity = 100.f;

	Camera(int width, int height, glm::vec3 position);

	void update_matrix(float FOVdeg, float nearPlane, float farPlane);
	const glm::mat4& get_matrix()const;

	//void Matrix(float FOVdeg, float nearPlane, float farPlane, Shader& shader, const char* uniform);
	void handle_inputs(GLFWwindow* window);
};