#include "lattice3d.h"

Lattice3d::Lattice3d(size_t N, const Cell& val)
    : N{ N }, foreground(N* N* N, val), background(N* N* N, val)
{}

auto Lattice3d::operator()(size_t s, size_t r, size_t c) -> Cell& {
    assert(s < N and r < N and c < N);
    return foreground[N * N * s + N * r + c];
}

auto Lattice3d::get_or(size_t s, size_t r, size_t c, const Cell& val)-> const Cell& {
    return (s < N and r < N and c < N) ? foreground[N * N * s + N * r + c] : val;
}

auto Lattice3d::fg_at(size_t s, size_t r, size_t c) -> Cell&
{
    assert(s < N and r < N and c < N);
    return foreground[N * N * s + N * r + c];
}

auto Lattice3d::bg_at(size_t s, size_t r, size_t c) -> Cell&
{
    assert(s < N and r < N and c < N);
    return background[N * N * s + N * r + c];
}

Lattice3d::Material::Material(Type type)
{
    this->type = type;
    switch (type)
    {
    case Type::Air:;
        solid = false;
        combustibility = false;//
        density = 1.29f;
        specific_heat = 1005.f;
        lambda = 0.026f;
        ignition_temperature = kelvin(2000.f);
        generated_energy = 0.f;
        smoke_emission = 0.f;
        break;
    case Type::Concrete:
        solid = true;
        combustibility = false;
        density = 2200.f;
        specific_heat = 900.f;
        lambda = 0.005f;//
        break;
    case Type::Wood:
        solid = true;
        combustibility = true;
        density = 400.f;
        specific_heat = 2390.f;
        lambda = 0.3f;
        ignition_temperature = kelvin(250.f);
        autoignition_temperature = kelvin(2000.f);
        ignition_time = side / 3.f;
        burning_time = 20.f;
        generated_energy = 1500.f;
        smoke_emission = 0.3f;
        break;
    case Type::Carpet:
        solid = true;
        combustibility = true;
        density = 300.f;
        specific_heat = 5200.f;
        lambda = 0.174f;
        ignition_temperature = kelvin(120.f);
        autoignition_temperature = kelvin(750.f);
        ignition_time = side / 0.8f;
        generated_energy = 500.f;
        smoke_emission = 0.8f;
        burning_time = 15.f;
        break;
    case Type::Fabrics:
        solid = true;
        combustibility = true;
        density = 400.f;
        specific_heat = 2390.f;
        lambda = 0.3f;
        ignition_temperature = kelvin(350.f);
        autoignition_temperature = kelvin(1400.f);
        ignition_time = side / 3.f;
        generated_energy = 1500.f;
        smoke_emission = 1.f;
        burning_time = 20.f;
        break;
    default:
        break;
    }
    mass = density * volume;
}




void Lattice3d::update(float dt)
{
    //heat transfer
    background = foreground;
    for (size_t s = 0u; s < N; ++s)
        for (size_t r = 0u; r < N; ++r)
            for (size_t c = 0u; c < N; ++c)
            {
                const auto& current_cell = fg_at(s, r, c);
                auto& cell_to_update = bg_at(s, r, c);

                const auto& cN = get_or(s + 1, r, c, current_cell);
                const auto& cS = get_or(s - 1, r, c, current_cell);

                const auto& cU = get_or(s, r + 1, c, current_cell);
                const auto& cD = get_or(s, r - 1, c, current_cell);

                const auto& cE = get_or(s, r, c + 1, current_cell);
                const auto& cW = get_or(s, r, c - 1, current_cell);

                //conduction

                float Tdiff = 0.f;
                for (const auto& neighboring_cell : { cN, cS, cU, cD, cE, cW }) {

                    const float lambda = neighboring_cell.material.lambda;// / (neighboring_cell.material.mass * neighboring_cell.material.specific_heat);
                    Tdiff += lambda * (neighboring_cell.temperature - current_cell.temperature);

                }

                cell_to_update.temperature += dt * Tdiff;

                //convection
                if (not current_cell.material.solid)
                { 
                    cell_to_update.temperature += dt * current_cell.material.alpha * (current_cell.temperature - cU.temperature);
                    cell_to_update.temperature -= dt * current_cell.material.alpha * (current_cell.temperature - cD.temperature);
                }

                //radiation
                //naive
                /*
                float Qrad = 0.f;
                for (const auto& neighboring_cell : { cN, cS, cU, cD, cE, cW }) {
                    Qrad += neighboring_cell.material.beta * (powf(current_cell.temperature, 4.f) - powf(neighboring_cell.temperature, 4.f));
                }
                */
            }

    foreground = background;

    //processing of fire and smoke
    for (size_t s = 0u; s < N; ++s)
        for (size_t r = 0u; r < N; ++r)
            for (size_t c = 0u; c < N; ++c)
            {
                const auto& current_cell = fg_at(s, r, c);
                auto& cell_to_update = bg_at(s, r, c);

                const auto& cN = get_or(s + 1, r, c, current_cell);
                const auto& cS = get_or(s - 1, r, c, current_cell);

                const auto& cU = get_or(s, r + 1, c, current_cell);
                const auto& cD = get_or(s, r - 1, c, current_cell);

                const auto& cE = get_or(s, r, c + 1, current_cell);
                const auto& cW = get_or(s, r, c - 1, current_cell);


                //fire
                if (current_cell.state == Cell::State::on_fire) {
                    if (current_cell.timer > current_cell.material.burning_time) {
                        cell_to_update.timer = 0.f;
                        cell_to_update.state = Cell::State::empty;
                        cell_to_update.material = Material::Type::Air;
                        //cell_to_update.type = Cell::Type::air;
                        continue;
                    }
                    else {
                        cell_to_update.timer += dt;
                        if (current_cell.material.solid)
                        {
                            float Q = current_cell.material.generated_energy * dt;
                            //float aaaa = Q / (current_cell.material.mass * current_cell.material.specific_heat);
                            //std::cout << "cos  " << aaaa << "\n";
                            cell_to_update.temperature += Q / (current_cell.material.mass * current_cell.material.specific_heat);
                        }
                    }
                }

                if (current_cell.state == Cell::State::empty and current_cell.material.solid) {
                    if (current_cell.temperature >=
                        current_cell.material.autoignition_temperature) {
                        cell_to_update.state = Cell::State::on_fire;
                        cell_to_update.timer = 0.f;
                    }
                    else
                    for (const auto& neighboring_cell : { cN, cS, cU, cD, cE, cW }) {
                        if (neighboring_cell.state == Cell::State::on_fire) {
                            if (current_cell.temperature >
                                current_cell.material.ignition_temperature and
                                current_cell.timer > current_cell.material.ignition_time) {
                                cell_to_update.state = Cell::State::on_fire;
                                cell_to_update.timer = 0.f;
                                break;
                            }
                            else {
                                cell_to_update.timer += dt;
                            }
                        }
                    }
                }
                //

                // smoke 
                if (not current_cell.material.solid)
                {
                    if (cD.state == Cell::State::on_fire) {
                        // Create new smoke
                        cell_to_update.smoke_contents += std::min(
                            0.5f / cD.material.burning_time,
                            current_cell.smoke_capacity - current_cell.smoke_contents);
                        // std::cout << "Smoke is being emitted, contents : "
                        //           << updated_cell.smoke_contents << std::endl;
                    }
                    const Cell cells[] = { cU, cN, cS, cE, cW, cD };
                    const float dn[] = { 1.f, 0.5f, 0.5f, 0.5f, 0.5f, 0.25f };
                    const float dn_when_full[] = { 0.f, 0.5f, 0.5f, 0.5f, 0.5f, 1.0f };
                    if (current_cell.smoke_capacity - current_cell.smoke_contents <=
                        current_cell.smoke_capacity * 0.1f) {
                        // Close to being full
                        for (size_t i = 1; i < 6; i++) {
                            const Cell& other = cells[i];
                            cell_to_update.smoke_contents -=
                                1. / 6. *
                                std::min(current_cell.smoke_contents,
                                    cell_to_update.smoke_capacity - other.smoke_contents);
                            cell_to_update.smoke_contents +=
                                1. / 6. *
                                std::min(other.smoke_contents,
                                    other.smoke_capacity - current_cell.smoke_contents);
                        }
                    }
                    else {
                        for (size_t i = 0; i < 6; i++) {
                            const Cell& other = cells[i];
                            cell_to_update.smoke_contents -=
                                1. / 6. *
                                std::min(current_cell.smoke_contents,
                                    cell_to_update.smoke_capacity - other.smoke_contents);
                            cell_to_update.smoke_contents +=
                                1. / 6. *
                                std::min(other.smoke_contents,
                                    other.smoke_capacity - current_cell.smoke_contents);
                        }
                    }
                }
                //
            }

    foreground = background;
}