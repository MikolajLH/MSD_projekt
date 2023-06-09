#include "Camera.h"

Camera::Camera(int width, int height, glm::vec3 position)
    : width{width}, height{height}, Position{position} {}

void Camera::update_matrix(float FOVdeg, float nearPlane, float farPlane) {
  glm::mat4 view = glm::mat4(1.f);
  glm::mat4 projection = glm::mat4(1.f);

  view = glm::lookAt(Position, Position + Orientation, Up);
  projection = glm::perspective(
      glm::radians(FOVdeg), float(width) / float(height), nearPlane, farPlane);

  cameraMatrix = projection * view;
}

const glm::mat4 &Camera::get_matrix() const { return cameraMatrix; }

void Camera::handle_inputs(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    Position += speed * Orientation;
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    Position += speed * -Orientation;
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    Position += speed * -glm::normalize(glm::cross(Orientation, Up));
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    Position += speed * glm::normalize(glm::cross(Orientation, Up));
  }
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    Position += speed * Up;
  }
  if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
    Position += speed * -Up;
  }
  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
    speed = 0.4f;
  } else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) {
    speed = 0.1f;
  }

  if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    if (firstClick) {
      glfwSetCursorPos(window, (width / 2.0), (height / 2.0));
      firstClick = false;
    }

    double mouseX;
    double mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    float rotx = sensitivity * (float)(mouseY - (height / 2.f)) / float(height);
    float roty = sensitivity * (float)(mouseX - (width / 2.f)) / float(height);

    glm::vec3 newOrientation =
        glm::rotate(Orientation, glm::radians(-rotx),
                    glm::normalize(glm::cross(Orientation, Up)));

    if (not((glm::angle(newOrientation, Up) <= glm::radians(5.f)) or
            (glm::angle(newOrientation, -Up) <= glm::radians(5.f)))) {
      Orientation = newOrientation;
    }

    Orientation = glm::rotate(Orientation, glm::radians(-roty), Up);

    glfwSetCursorPos(window, (width / 2.0), (height / 2.0));

  } else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) ==
             GLFW_RELEASE) {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    firstClick = true;
  }
}