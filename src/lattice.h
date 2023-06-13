#pragma once
#include <vector>
#include <cassert>


class Lattice
{
	public:
		class Cell
		{
			public:
				enum class State { non_flammable, on_fire, empty };
				State state = State::empty;

				enum class Type { air, wood, concrete };
				Type type = Type::air;

				float temperature = 273.f;
				float lambda = 1.f;

				float ignition_temperature = 273.f + 40.f;
				float autoignition_temperature = 273.f + 100.f;
				float ignition_time = 1.f;
				float burning_time = 5.f;

				float timer = 0.f;
		};

		explicit Lattice(size_t N, const Cell& val = Cell());

		Cell& operator()(size_t s, size_t r, size_t c);
		const Cell& get_or(size_t s, size_t r, size_t c, const Cell& val = Cell());

		void update(float dt);

		const size_t N;
	private:
		std::vector<Cell>foreground;
		std::vector<Cell>background;
};
