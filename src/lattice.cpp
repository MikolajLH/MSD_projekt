#include "lattice.h"
#include "Grid3d.h"

Lattice::Cell::Cell()
    : state{Lattice::Cell::State::empty}, type{Lattice::Cell::Type::air} {}

Lattice::Lattice(size_t N, const Cell &val)
    : N{N}, foreground(N * N * N, val), background(N * N * N, val) {}

auto Lattice::operator()(size_t s, size_t r, size_t c) -> Cell & {
  assert(s < N and r < N and c < N);

  return foreground[N * N * s + N * r + c];
}

auto Lattice::get_or(size_t s, size_t r, size_t c, const Cell &val)
    -> const Cell & {
  return (s < N and r < N and c < N) ? foreground[N * N * s + N * r + c] : val;
}

void Lattice::update(float dt) {

  // update heat of each cell
  background = foreground;
  for (size_t s = 0u; s < N; ++s)
    for (size_t r = 0u; r < N; ++r)
      for (size_t c = 0u; c < N; ++c) {

        auto current_cell = (*this)(s, r, c);

        const auto &cN = get_or(s + 1, r, c, current_cell);
        const auto &cS = get_or(s - 1, r, c, current_cell);

        const auto &cU = get_or(s, r + 1, c, current_cell);
        const auto &cD = get_or(s, r - 1, c, current_cell);

        const auto &cE = get_or(s, r, c + 1, current_cell);
        const auto &cW = get_or(s, r, c - 1, current_cell);

        float Tdiff = 0.f;
        for (const auto &neighboring_cell : {cN, cS, cU, cD, cE, cW}) {
          if (neighboring_cell.type == Cell::Type::air)
            continue;

          Tdiff += neighboring_cell.lambda *
                   (neighboring_cell.temperature - current_cell.temperature);
        }

        Cell &updated_cell = background[N * N * s + N * r + c];

        updated_cell.temperature = updated_cell.temperature + dt * Tdiff;
      }
  foreground = background;

  for (size_t s = 0u; s < N; ++s)
    for (size_t r = 0u; r < N; ++r)
      for (size_t c = 0u; c < N; ++c) {
        auto current_cell = (*this)(s, r, c);
        Cell &updated_cell = background[N * N * s + N * r + c];

        const auto &cN = get_or(s + 1, r, c, current_cell);
        const auto &cS = get_or(s - 1, r, c, current_cell);

        const auto &cU = get_or(s, r + 1, c, current_cell);
        const auto &cD = get_or(s, r - 1, c, current_cell);

        const auto &cE = get_or(s, r, c + 1, current_cell);
        const auto &cW = get_or(s, r, c - 1, current_cell);

        if (current_cell.type == Cell::Type::air) {
          if (cD.state == Cell::State::on_fire) {
            // Create new smoke
            updated_cell.smoke_contents += std::min(
                0.5f / cD.burning_time,
                current_cell.smoke_capacity - current_cell.smoke_contents);
            // std::cout << "Smoke is being emitted, contents : "
            //           << updated_cell.smoke_contents << std::endl;
          }
          const Cell cells[] = {cU, cN, cS, cE, cW, cD};
          const float dn[] = {1.f, 0.5f, 0.5f, 0.5f, 0.5f, 0.25f};
          const float dn_when_full[] = {0.f, 0.5f, 0.5f, 0.5f, 0.5f, 1.0f};
          if (current_cell.smoke_capacity - current_cell.smoke_contents <=
              current_cell.smoke_capacity * 0.1f) {
            // Close to being full
            for (size_t i = 1; i < 6; i++) {
              const Cell &other = cells[i];
              updated_cell.smoke_contents -=
                  1. / 6. *
                  std::min(current_cell.smoke_contents,
                           updated_cell.smoke_capacity - other.smoke_contents);
              updated_cell.smoke_contents +=
                  1. / 6. *
                  std::min(other.smoke_contents,
                           other.smoke_capacity - current_cell.smoke_contents);
            }
          } else {
            for (size_t i = 0; i < 6; i++) {
              const Cell &other = cells[i];
              updated_cell.smoke_contents -=
                  1. / 6. *
                  std::min(current_cell.smoke_contents,
                           updated_cell.smoke_capacity - other.smoke_contents);
              updated_cell.smoke_contents +=
                  1. / 6. *
                  std::min(other.smoke_contents,
                           other.smoke_capacity - current_cell.smoke_contents);
            }
          }
        }

        if (current_cell.state == Cell::State::non_flammable)
          continue;

        if (current_cell.state == Cell::State::on_fire) {
          if (current_cell.timer > current_cell.burning_time) {
            updated_cell.timer = 0.f;
            updated_cell.state = Cell::State::empty;
            updated_cell.type = Cell::Type::air;
            continue;
          } else {
            updated_cell.timer += dt;
          }
        }

        if (current_cell.state == Cell::State::empty) {
          if (current_cell.temperature >=
              current_cell.autoignition_temperature) {
            updated_cell.state = Cell::State::on_fire;
            updated_cell.timer = 0.f;
            continue;
          }

          for (const auto &neighboring_cell : {cN, cS, cU, cD, cE, cW}) {
            if (neighboring_cell.state == Cell::State::on_fire) {
              if (current_cell.temperature >
                      current_cell.ignition_temperature and
                  current_cell.timer > current_cell.ignition_time) {
                updated_cell.state = Cell::State::on_fire;
                updated_cell.timer = 0.f;
                break;
              } else {
                updated_cell.timer += dt;
              }
            }
          }
        }
      }

  foreground = background;
}