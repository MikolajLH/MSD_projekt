#include "application.h"
#include "lattice.h"
#include "utility.h"
#include <cstdint>
#include <iostream>

class Demo : public App {
public:
  Demo(int w, int h, size_t n, std::string_view title)
      : App(w, h, n, title, glm::vec3(10, 0, 20)),
        cube_shader("res/shaders/cube.shader"), lattice{n} {

    lattice(5, 1, 5).type = Lattice::Cell::Type::wood;
    lattice(4, 1, 5).type = Lattice::Cell::Type::wood;
    lattice(5, 1, 4).type = Lattice::Cell::Type::wood;
    lattice(4, 1, 4).type = Lattice::Cell::Type::wood;

    lattice(3, 1, 4).type = Lattice::Cell::Type::wood;
    lattice(2, 1, 4).type = Lattice::Cell::Type::wood;
    lattice(1, 1, 4).type = Lattice::Cell::Type::wood;

    lattice(3, 1, 5).type = Lattice::Cell::Type::wood;
    lattice(2, 1, 5).type = Lattice::Cell::Type::wood;
    lattice(1, 1, 5).type = Lattice::Cell::Type::wood;

    for (size_t s = 0u; s < lattice.N; ++s)
      for (size_t r = 0u; r < lattice.N; ++r)
        for (size_t c = 0u; c < lattice.N; ++c) {
          using enum Lattice::Cell::Type;

          if (lattice(s, r, c).type == wood) {
            lattice(s, r, c).state = Lattice::Cell::State::empty;
            continue;
          }

          lattice(s, r, c).type = r == 0 ? concrete : air;

          lattice(s, r, c).lambda = r == 0 ? 0.5f : 1.f;

          lattice(s, r, c).temperature = 291.f;

          lattice(s, r, c).state = Lattice::Cell::State::non_flammable;
        }
  }

  void update(float dt) override {

    if (glfwGetKey(wnd, GLFW_KEY_Z) == GLFW_PRESS)
      lattice(5, 1, 5).temperature = 453.f;

    clear(clr::blue);

    lattice.update(dt);
    bool q_pressed = glfwGetKey(wnd, GLFW_KEY_Q) == GLFW_PRESS;

    // draw lattice
    for (size_t s = 0u; s < lattice.N; ++s)
      for (size_t r = 0u; r < lattice.N; ++r)
        for (size_t c = 0u; c < lattice.N; ++c) {
          using enum Lattice::Cell::Type;
          glm::vec4 color = clr::black;

          if (lattice(s, r, c).type == air) {
            const auto &cell = lattice(s, r, c);
            // if (cell.smoke_contents == 0.f)
            //   continue;
            color = glm::vec4(glm::vec3(0.f),
                              cell.smoke_contents / cell.smoke_capacity * 0.9f);
          } else if (q_pressed) {
            const float celciusT = lattice(s, r, c).temperature - 273.f;
            if (celciusT < 0.f) {
              color = glm::vec4(1.f + celciusT / 20.f, 1.f + celciusT / 20.f,
                                1.f, 1.f);
            } else {
              color = glm::vec4(1.f, 1.f - celciusT / 100.f,
                                1.f - celciusT / 100.f, 1.f);
            }
          } else {
            if (lattice(s, r, c).type == concrete)
              color = clr::gray;
            else if (lattice(s, r, c).type == wood)
              color = clr::brown;
            if (lattice(s, r, c).state == Lattice::Cell::State::on_fire)
              color = clr::orange;
          }
          auto N = lattice.N;
          size_t index = N * N * s + N * r + c;
          colors_vec[index] = color;
        }
    // std::cout << lattice(4, 1, 5).temperature << "\n";
    draw_cubes(cube_shader, static_cast<int>(lattice.N));
  }

  Shader cube_shader;
  Lattice lattice;
};

int main() {
  Demo demo(800, 800, 17, "Fire and Smoke");
  demo.run();

  return 0;
}