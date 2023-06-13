#include "lattice.h"




Lattice::Lattice(size_t N, const Cell& val)
	: N{N}, foreground(N * N * N, val), background(N * N * N, val)
{}

auto Lattice::operator()(size_t s, size_t r, size_t c) -> Cell&
{
	assert(s < N and r < N and c < N);

	return foreground[N * N * s + N * r + c];
}

auto Lattice::get_or(size_t s, size_t r, size_t c, const Cell& val) -> const Cell&
{
	return (s < N and r < N and c < N) ? foreground[N * N * s + N * r + c] : val;
}



void Lattice::update(float dt)
{

	//update heat of each cell
	background = foreground;
	for (size_t s = 0u; s < N; ++s)
		for (size_t r = 0u; r < N; ++r)
			for (size_t c = 0u; c < N; ++c)
			{

				auto current_cell = (*this)(s, r, c);

				auto cN = get_or(s + 1, r, c, current_cell);
				auto cS = get_or(s - 1, r, c, current_cell);

				auto cU = get_or(s, r + 1, c, current_cell);
				auto cD = get_or(s, r - 1, c, current_cell);

				auto cE = get_or(s, r, c + 1, current_cell);
				auto cW = get_or(s, r, c - 1, current_cell);


				float Tdiff = 0.f;
				for (const auto& neighboring_cell : { cN, cS, cU, cD, cE, cW })
				{
					if (neighboring_cell.type == Cell::Type::air)continue;

					Tdiff += neighboring_cell.lambda * (neighboring_cell.temperature - current_cell.temperature);
				}
					

				Cell& updated_cell = background[N * N * s + N * r + c];

				updated_cell.temperature = updated_cell.temperature + dt * Tdiff;
			}
	foreground = background;

	for (size_t s = 0u; s < N; ++s)
		for (size_t r = 0u; r < N; ++r)
			for (size_t c = 0u; c < N; ++c)
			{
				auto current_cell = (*this)(s, r, c);
				Cell& updated_cell = background[N * N * s + N * r + c];

				if (current_cell.state == Cell::State::non_flammable)continue;

				if (current_cell.state == Cell::State::on_fire)
				{
					if (current_cell.timer > current_cell.burning_time)
					{
						updated_cell.timer = 0.f;
						updated_cell.state = Cell::State::empty;
						updated_cell.type = Cell::Type::air;
						continue;
					}
					else
					{
						updated_cell.timer += dt;
					}
				}

				auto cN = get_or(s + 1, r, c, current_cell);
				auto cS = get_or(s - 1, r, c, current_cell);

				auto cU = get_or(s, r + 1, c, current_cell);
				auto cD = get_or(s, r - 1, c, current_cell);

				auto cE = get_or(s, r, c + 1, current_cell);
				auto cW = get_or(s, r, c - 1, current_cell);

				if (current_cell.state == Cell::State::empty)
				{
					if (current_cell.temperature >= current_cell.autoignition_temperature)
					{
						updated_cell.state = Cell::State::on_fire;
						updated_cell.timer = 0.f;
						continue;
					}

					for (const auto& neighboring_cell : { cN, cS, cU, cD, cE, cW })
					{
						if (neighboring_cell.state == Cell::State::on_fire)
						{
							if (current_cell.temperature > current_cell.ignition_temperature and current_cell.timer > current_cell.ignition_time)
							{
								updated_cell.state = Cell::State::on_fire;
								updated_cell.timer = 0.f;
								break;
							}
							else
							{
								updated_cell.timer += dt;
							}
						}
					}
				}
			}

	foreground = background;

}