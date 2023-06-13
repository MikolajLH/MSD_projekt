#include "application.h"

App::App(int w, int h, std::string_view title, glm::vec3 cam_pos)
    : wnd(nullptr), camera(w, h, cam_pos) {

  auto err = glfwInit();
  assert(err);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  wnd = glfwCreateWindow(w, h, title.data(), NULL, NULL);
  assert(wnd);

  glfwMakeContextCurrent(wnd);

  glfwSwapInterval(1);

  err = glewInit();
  assert(err == GLEW_OK);
}

App::~App() { glfwTerminate(); }

void App::run() {
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  static float et = 0.f;
  while (not glfwWindowShouldClose(this->wnd)) {
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

void App::draw_cube(Shader &shader, glm::vec3 pos, glm::vec4 color,
                    GLfloat size, GLfloat rot, glm::vec3 norm) {
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

  static const bool dummy =
      (va.link_attrib(vb, 0, 4, GL_FLOAT, sizeof(glm::vec4), (const void *)0),
       false);

  shader.bind();

  shader.set_uniform_vec4f("u_Color", color);
  glm::mat4 model = glm::mat4(1.f);
  glm::mat4 mvp =
      get_camera_matrix() *
      glm::translate(glm::rotate(glm::scale(model, glm::vec3(size, size, size)),
                                 rot, norm),
                     pos);
  shader.set_uniform_mat4f("u_MVP", mvp);

  va.bind();
  ib.bind();

  glDrawElements(GL_TRIANGLES, ib.get_count(), GL_UNSIGNED_INT, nullptr);
}
