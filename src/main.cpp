#include "application.h"
#include "lattice3d.h"
#include "utility.h"
#include <cstdint>
#include <iostream>


glm::vec4 cell_heat_color(Lattice3d::Cell cell)
{
    static constexpr float room_t = 294.f; //21
    static constexpr float max_t = 2000.f;
    glm::vec4 color(0, 0, 0, 0);
  
    float t_diff = room_t - cell.temperature;
    if (t_diff > 0.f)
    {
        color.r = 1.f - (cell.temperature / max_t);
        color.g = 1.f - (cell.temperature / max_t);
        color.b = 1.f;
        color.a = cell.material.solid ? 1.f : 0.2f;
    }
    else
    {
        color.r = 1.f;
        color.g = 1.f - (cell.temperature / max_t);
        color.b = 1.f - (cell.temperature / max_t);
        color.a = cell.material.solid ? 1.f : 0.2f;
    }

    if (abs(t_diff) < 1.f)color.a = 0.f;

    return color;
}

glm::vec4 cell_material_color(Lattice3d::Cell cell)
{
    if (cell.state == Lattice3d::Cell::State::on_fire)
        return clr::orange + glm::vec4(0.f, random<float>() / 10.f, random<float>() / 10.f, 1.f);

    return cell.material.color;
}


class Demo : public App {
public:
  Demo(int w, int h, size_t n, std::string_view title)
      : App(w, h, n, title, glm::vec3(10, 0, 20)),
        cube_shader("res/shaders/cube.shader"), lattice{n} {



      for (size_t s = 0u; s < lattice.N; ++s)
          for (size_t c = 0u; c < lattice.N; ++c) {
              lattice(s, 1, c).material = Lattice3d::Material::Type::Wood;
              lattice(s, 1, c).material.color = clr::brown;

              lattice(s, 2, c).material = Lattice3d::Material::Type::Carpet;
              lattice(s, 2, c).material.color = clr::light_green;
          }
      for (size_t r = 0u; r < lattice.N; ++r)
          for (size_t c = 0u; c < lattice.N; ++c) {
              lattice(0, r, c).material = Lattice3d::Material::Type::Concrete;
              lattice(0, r, c).material.color = clr::gray;
              lattice(0, r, c).state = Lattice3d::Cell::State::non_flammable;
          }
      for (size_t r = 0u; r < lattice.N; ++r)
          for (size_t s = 0u; s < lattice.N; ++s) {
              lattice(s, r, 0).material = Lattice3d::Material::Type::Concrete;
              lattice(s, r, 0).material.color = clr::gray + clr::mono(0.1f);
              lattice(s, r, 0).state = Lattice3d::Cell::State::non_flammable;
          }


      for (size_t s = 0u; s < lattice.N; ++s)
          for (size_t c = 0u; c < lattice.N; ++c) {
              lattice(s, 0, c).material = Lattice3d::Material::Type::Concrete;
              lattice(s, 0, c).material.color = clr::gray + clr::mono(0.1f);
              lattice(s, 0, c).state = Lattice3d::Cell::State::non_flammable;

              lattice(s, lattice.N - 1, c).material = Lattice3d::Material::Type::Concrete;
              lattice(s, lattice.N - 1, c).material.color = clr::gray + clr::mono(-0.15f);
              lattice(s, lattice.N - 1, c).state = Lattice3d::Cell::State::non_flammable;
          }


      //couch
      size_t s0 = 2, c0 = 1, r0 = 3;
      size_t width = 6;
      size_t depth = 6;
     
      for (size_t s = s0; s < s0 + width; ++s) {
          lattice(s, r0 + 2, c0).material = Lattice3d::Material::Type::Fabrics;
          lattice(s, r0 + 2, c0).material.color = clr::violet + clr::mono(-0.10f);

          lattice(s, r0 + 3, c0).material = Lattice3d::Material::Type::Fabrics;
          lattice(s, r0 + 3, c0).material.color = clr::violet + clr::mono(-0.10f);

          lattice(s, r0 + 4, c0).material = Lattice3d::Material::Type::Fabrics;
          lattice(s, r0 + 4, c0).material.color = clr::violet + clr::mono(-0.10f);

          for (size_t c = c0; c < c0 + 3; ++c) {
              lattice(s, r0, c).material = Lattice3d::Material::Type::Fabrics;
              lattice(s, r0, c).material.color = clr::violet;

              lattice(s, r0 + 1, c).material = Lattice3d::Material::Type::Fabrics;
              lattice(s, r0 + 1, c).material.color = clr::violet;

              if (s == s0 or s == s0 + width - 1)
              {
                  lattice(s, r0 + 2, c).material = Lattice3d::Material::Type::Fabrics;
                  lattice(s, r0 + 2, c).material.color = clr::violet + clr::mono(-0.05f);
              }
          }
      }

      //table
      s0 = 2;
      c0 = 5;
      r0 = 3;
      width = 6;
      depth = 6;

      lattice(s0, r0, c0).material = Lattice3d::Material::Type::Wood;
      lattice(s0, r0, c0).material.color = clr::brown;

      lattice(s0 + width - 1, r0, c0).material = Lattice3d::Material::Type::Wood;
      lattice(s0 + width - 1, r0, c0).material.color = clr::brown;

      lattice(s0, r0, c0 + depth - 1).material = Lattice3d::Material::Type::Wood;
      lattice(s0, r0, c0 + depth - 1).material.color = clr::brown;

      lattice(s0 + width - 1, r0, c0 + depth - 1).material = Lattice3d::Material::Type::Wood;
      lattice(s0 + width - 1, r0, c0 + depth - 1).material.color = clr::brown;

      for (size_t s = s0; s < s0 + width; ++s)
          for (size_t c = c0; c < c0 + depth; ++c) {
              lattice(s, r0 + 1, c).material = Lattice3d::Material::Type::Wood;
              lattice(s, r0 + 1, c).material.color = clr::brown;
          }


      //sep wall

      for (size_t r = 0u; r < lattice.N; ++r)
          for (size_t c = 0u; c < lattice.N; ++c) {
              lattice(12, r, c).material = Lattice3d::Material::Type::Concrete;
              lattice(12, r, c).material.color = clr::gray + clr::mono(-0.14f);
              lattice(12, r, c).state = Lattice3d::Cell::State::non_flammable;
          }
  }

  void update(float dt) override {
    clear(clr::blue);


    if (glfwGetKey(wnd, GLFW_KEY_Z) == GLFW_PRESS)
        lattice(3, 4, 1).temperature += 40.f;

    bool heat_map_view = (glfwGetKey(wnd, GLFW_KEY_Q) == GLFW_PRESS);

    lattice.update(dt);

    // draw lattice
    for (size_t s = 0u; s < lattice.N; ++s)
      for (size_t r = 0u; r < lattice.N; ++r)
          for (size_t c = 0u; c < lattice.N; ++c) {

              glm::vec4 color = heat_map_view ? cell_heat_color(lattice(s, r, c)) : cell_material_color(lattice(s, r, c));
              
              if(not lattice(s, r, c).material.solid)
                color = color + clr::alpha(lattice(s, r, c).smoke_contents / lattice(s, r, c).smoke_capacity * 0.9f);


              auto N = lattice.N;
              size_t index = N * N * s + N * r + c;
              colors_vec[index] = color;
          }
    draw_cubes(cube_shader, static_cast<int>(lattice.N));

    std::cout << lattice(3, 4, 1).temperature << "\n";
  }

  Shader cube_shader;
  Lattice3d lattice;
};

int main() {
  Demo demo(800, 800, 20, "Fire and Smoke");
  demo.run();

  return 0;
}