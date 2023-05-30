#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <string>

#include "Cube.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

#include "Grid3d.h"

int width = 900;
int height = 900;
constexpr float CUBE_SIZE = 1.0f;

int main(void) {

  /* Initialize the library */
  if (!glfwInit())
    return -1;

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<float> real_dis(0.f, 1.f);
  std::uniform_int_distribution<int> int_dis(0, 5);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  /* Create a windowed mode window and its OpenGL context */
  GLFWwindow *window =
      glfwCreateWindow(width, height, "Hello World", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);

  glfwSwapInterval(1);

  if (glewInit() != GLEW_OK) {
    return -1;
  }

  std::cout << glGetString(GL_VERSION) << std::endl;

  {
    Shader shader("res/shaders/cube.shader");
    Cube cube;
    Camera camera(width, height, glm::vec3(0.f, 0.f, 2.f));

    int N = 15;

    Grid3d<Cell> foreground(N, {});
    Grid3d<Cell> background(N, {});

    foreground(0, 0, 0).temperature = 100.f;

    for (int s = 0; s < N; ++s)
      for (int r = 0; r < N; ++r)
        for (int c = 0; c < N; ++c) {
          foreground(s, r, c).mass = 1.0f;
          foreground(s, r, c).capacity = 1.0f;
          foreground(s, r, c).alpha = 1.f;
          foreground(s, r, c).lambda = 0.5f;
          if (r == 0) {
            foreground(s, r, c).state = StateOfMatter::Solid;
          } else {
          }

          background(s, r, c) = foreground(s, r, c);
        }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    double prevTime = glfwGetTime();

    auto get_color = [](float t) -> glm::vec4 {
      t = std::clamp(t, 0.f, 1.f);
      return {t, 0.f, 0.f, t != 0.f ? t / 2.f + 0.2f : 0.f};
    };

    const float dt = 0.1f;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {

      glClearColor(0.47f, 0.30f, 1.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      double crntTime = glfwGetTime();
      if (crntTime - prevTime >= 1. / 10.) {
        prevTime = crntTime;

        for (int s = 0; s < N; ++s)
          for (int r = 0; r < N - 1; ++r)
            for (int c = 0; c < N; ++c) {
              // if (s != 0 or c != 0)continue;

              auto cell = background(s, r, c);
              auto upper = background.get_or(s, r + 1, c);

              const float prim = cell.alpha / (cell.mass * cell.capacity);

              auto NU = foreground.get_or(s, r + 1, c);
              NU.temperature +=
                  prim * (cell.temperature - upper.temperature) * dt;
              foreground.set_if(s, r + 1, c, NU);

              // foreground(s, r + 1, c).temperature += prim * (cell.temperature
              // - upper.temperature) * dt;
              foreground(s, r, c).temperature -=
                  prim * (cell.temperature - upper.temperature) * dt;
            }
        std::cout << foreground(0, 0, 0).temperature << "      "
                  << foreground(0, N - 1, 0).temperature << "\n";

        for (int s = 0; s < N; ++s)
          for (int r = 0; r < N; ++r)
            for (int c = 0; c < N; ++c) {
              background(s, r, c).temperature = foreground(s, r, c).temperature;
            }
      }

      camera.handle_inputs(window);
      camera.update_matrix(45.f, 0.1f, 100.f);

      for (int s = 0; s < N; ++s)
        for (int r = 0; r < N; ++r)
          for (int c = 0; c < N; ++c) {
            auto color = get_color(foreground(s, r, c).temperature);
            if (color.a > 0.f) {
              glm::vec3 position = glm::vec3(0.f, 0.f, 0.f);
              position.x = float(c);
              position.y = float(r);
              position.z = float(s);
              cube.draw(shader, position, color, 0.f, camera, 1.f);
            }
          }

      /*if (colors[fg(s, r, c)].a == 1.f)
      {
          glm::vec3 position = glm::vec3(0.f, 0.f, 0.f);
          position.x = float(c) * CUBE_SIZE;
          position.y = float(r) * CUBE_SIZE;
          position.z = float(s) * CUBE_SIZE;

          cube.draw(shader, position, colors[fg(s, r, c)], 0.f, camera, 1.f);
      }*/

      // cube.draw(shader, {}, { 1.f, 0.f, 0.f, 1.f }, 0.f, camera, 1.f);

      /* Swap front and back buffers */
      glfwSwapBuffers(window);
      /* Poll for and process events */
      glfwPollEvents();
    }
  }

  glfwTerminate();
  return 0;
}