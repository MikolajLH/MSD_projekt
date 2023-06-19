#include "application.h"
#include "VertexArray.h"
#include <GL/gl.h>
#include <cstdio>
#include <glm/fwd.hpp>
#include <iostream>

namespace {
int width, height;

void framebuffer_size_callback(GLFWwindow *window, int _width, int _height) {
  glViewport(0, 0, _width, _height);
  width = _width;
  height = _height;
}
void debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity,
                    GLsizei length, const GLchar *message,
                    const void *userParam) {
  printf("GL debug: %s\n", message);
  fflush(stdout);
}

} // namespace

App::App(int w, int h, size_t n, std::string_view title, glm::vec3 cam_pos)
    : wnd(nullptr), camera(w, h, cam_pos), colors_vec{n * n * n},
      indexes_vec(n * n * n) {

  auto err = glfwInit();
  assert(err);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  width = w;
  height = h;

  wnd = glfwCreateWindow(w, h, title.data(), NULL, NULL);
  assert(wnd);

  glfwMakeContextCurrent(wnd);

  glfwSetWindowSizeCallback(wnd, framebuffer_size_callback);

  glfwSwapInterval(1);

  err = glewInit();
  if (err != GLEW_OK) {
    std::cerr << "Error: glewInit: " << glewGetErrorString(err) << std::endl;
    // return EXIT_FAILURE;
    return;
  }

  glDebugMessageCallback(debug_callback, nullptr);
}

App::~App() { glfwTerminate(); }

void App::run() {
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  for (auto &v : colors_vec) {
    v.r = 1.0;
    v.g = 0.0;
    v.b = 0.0;
    v.a = 1.0;
  }
  for (int i = 0; i < indexes_vec.size(); i++) {
    indexes_vec[i] = i;
  }

  static float et = 0.f;
  while (not glfwWindowShouldClose(this->wnd)) {
    camera.width = width;
    camera.height = height;

    auto beg = std::chrono::steady_clock::now();
    glfwPollEvents();
    camera.handle_inputs(this->wnd);
    camera.update_matrix(this->FOVdeg, this->nearPlane, this->farPlane);

    this->update(et);

    glfwSwapBuffers(this->wnd);
    et = std::chrono::duration<float>(std::chrono::steady_clock::now() - beg)
             .count();
  }
}
const glm::mat4 &App::get_camera_matrix() { return this->camera.get_matrix(); }

void App::clear(const glm::vec4 color) {
  glClearColor(color.r, color.g, color.b, color.a);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void App::draw_cubes(Shader &shader, int N, GLfloat size, GLfloat rot,
                     glm::vec3 norm) {
  static const std::vector<glm::vec4> vertices = {
      glm::vec4{-0.5f, -0.5f, -0.5f, 1.f}, // A 0
      glm::vec4{0.5f, -0.5f, -0.5f, 1.f},  // B 1
      glm::vec4{0.5f, 0.5f, -0.5f, 1.f},   // C 2
      glm::vec4{-0.5f, 0.5f, -0.5f, 1.f},  // D 3
      glm::vec4{-0.5f, -0.5f, 0.5f, 1.f},  // E 4
      glm::vec4{0.5f, -0.5f, 0.5f, 1.f},   // F 5
      glm::vec4{0.5f, 0.5f, 0.5f, 1.f},    // G 6
      glm::vec4{-0.5f, 0.5f, 0.5f, 1.f},   // H 7

      glm::vec4{-0.5f, 0.5f, -0.5f, 1.f},  // D 8
      glm::vec4{-0.5f, -0.5f, -0.5f, 1.f}, // A 9
      glm::vec4{-0.5f, -0.5f, 0.5f, 1.f},  // E 10
      glm::vec4{-0.5f, 0.5f, 0.5f, 1.f},   // H 11
      glm::vec4{0.5f, -0.5f, -0.5f, 1.f},  // B 12
      glm::vec4{0.5f, 0.5f, -0.5f, 1.f},   // C 13
      glm::vec4{0.5f, 0.5f, 0.5f, 1.f},    // G 14
      glm::vec4{0.5f, -0.5f, 0.5f, 1.f},   // F 15

      glm::vec4{-0.5f, -0.5f, -0.5f, 1.f}, // A 16
      glm::vec4{0.5f, -0.5f, -0.5f, 1.f},  // B 17
      glm::vec4{0.5f, -0.5f, 0.5f, 1.f},   // F 18
      glm::vec4{-0.5f, -0.5f, 0.5f, 1.f},  // E 19
      glm::vec4{0.5f, 0.5f, -0.5f, 1.f},   // C 20
      glm::vec4{-0.5f, 0.5f, -0.5f, 1.f},  // D 21
      glm::vec4{-0.5f, 0.5f, 0.5f, 1.f},   // H 22
      glm::vec4{0.5f, 0.5f, 0.5f, 1.f}     // G 23
  };

  static const std::vector<GLuint> indices = {
      // front and back
      0, 3, 2, 2, 1, 0, 4, 5, 6, 6, 7, 4,
      // left and right
      11, 8, 9, 9, 10, 11, 12, 13, 14, 14, 15, 12,
      // bottom and top
      16, 17, 18, 18, 19, 16, 20, 21, 22, 22, 23, 20};
  static bool init = false;

  static VertexBuffer vb(vertices.data(), vertices.size() * sizeof(glm::vec4));
  static IndexBuffer ib(indices.data(), indices.size());

  static VertexArray va;

  va.link_attrib(vb, 0, 4, GL_FLOAT, sizeof(glm::vec4), (const void *)0);
  shader.bind();

  VertexBuffer colors(colors_vec.data(), colors_vec.size() * sizeof(glm::vec4));
  colors.bind();

  va.link_attrib(colors, 1, 4, GL_FLOAT, sizeof(glm::vec4), (const void *)0);
  glVertexAttribDivisor(1, 1);

  // VertexBuffer indexes(indexes_vec.data(), indexes_vec.size() *
  // sizeof(GLint)); indexes.bind(); va.link_attrib(indexes, 2, 1, GL_INT,
  // sizeof(GLint), (const void *)0); glVertexAttribDivisor(2, 1);

  glm::mat4 model = glm::mat4(1.f);
  glm::mat4 mvp = get_camera_matrix() * model;
  shader.set_uniform_mat4f("u_MVP", mvp);
  GLint dimensions[] = {N, N, N};
  shader.set_uniform_3i("dimensions", dimensions);

  va.bind();
  ib.bind();

  glDrawElementsInstanced(GL_TRIANGLES, ib.get_count(), GL_UNSIGNED_INT,
                          nullptr, N * N * N);

  va.unbind();
  ib.unbind();
  shader.unbind();
  // indexes.unbind();
}
