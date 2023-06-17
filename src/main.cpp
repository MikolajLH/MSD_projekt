#include "application.h"
#include "lattice.h"
#include "utility.h"
#include <iostream>

class Demo : public App {
public:
  Demo(int w, int h, std::string_view title)
      : App(w, h, title, glm::vec3(10, 0, 20)),
        cube_shader("res/shaders/cube.shader") {

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

    // draw lattice
    for (size_t s = 0u; s < lattice.N; ++s)
      for (size_t r = 0u; r < lattice.N; ++r)
        for (size_t c = 0u; c < lattice.N; ++c) {
          using enum Lattice::Cell::Type;
          glm::vec4 color = clr::black;
          glm::vec3 pos = glm::vec3(float(c), float(r), -float(s));

          if (lattice(s, r, c).type == air)
            continue;
          if (glfwGetKey(wnd, GLFW_KEY_Q) == GLFW_PRESS) {
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
          draw_cube(cube_shader, pos, color);
        }
    std::cout << lattice(4, 1, 5).temperature << "\n";
  }

  Shader cube_shader;
  Lattice lattice = Lattice(14);
};

int main() {
  Demo demo(800, 800, "Fire and Smoke");
  demo.run();

  return 0;
}