#pragma once
#include "Shader.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cassert>
#include <chrono>
#include <glm/glm.hpp>
#include <string_view>
#include <vector>

#include "Camera.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

namespace clr {
constexpr glm::vec4 red = {1.f, 0.f, 0.f, 1.f};
constexpr glm::vec4 green = {0.f, 1.f, 0.f, 1.f};
constexpr glm::vec4 blue = {0.f, 0.f, 1.f, 1.f};
constexpr glm::vec4 black = {0.f, 0.f, 0.f, 1.f};
constexpr glm::vec4 white = {1.f, 1.f, 1.f, 1.f};
constexpr glm::vec4 brown = {0.20f, 0.10f, 0.07f, 1.f};
constexpr glm::vec4 gray = {0.5f, 0.5f, 0.5f, 1.f};
constexpr glm::vec4 orange = {1.f, 0.55f, 0.f, 1.f};
constexpr glm::vec4 light_green = { 0.635f, 1.f, 0.663f, 1.f };
constexpr glm::vec4 violet = { 0.925f, 0.38f, 1.f, 1.f };

constexpr glm::vec4 alpha(float a) { return glm::vec4(0, 0, 0, a); }
constexpr glm::vec4 mono(float t) { return glm::vec4(t, t, t, 1.f); }
} // namespace clr

class App {
public:
  App(int w, int h, size_t n, std::string_view title,
      glm::vec3 cam_pos = glm::vec3(0.f, 0.f, 0.f));
  virtual ~App();
  void run();

  void clear(const glm::vec4 color);
  void draw_cubes(Shader &shader, int N, GLfloat size = 1.f, GLfloat rot = 0.f,
                  glm::vec3 norm = glm::vec3(0.f, 1.f, 0.f));

  const glm::mat4 &get_camera_matrix();

  virtual void update(float dt) = 0;

protected:
  GLFWwindow *wnd;
  Camera camera;

  float FOVdeg = 45.f;
  float nearPlane = 0.1f;
  float farPlane = 100.f;
  std::vector<glm::vec4> colors_vec;
  std::vector<GLint> indexes_vec;
  std::vector<float> distances;
};
