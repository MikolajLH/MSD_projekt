#pragma once
#include <vector>
#include <cassert>
#include <iostream>
#include <glm/glm.hpp>


constexpr float kelvin(float t) { return t + 273.f; }
constexpr float celcius(float t) { return t - 273.f; }

class Lattice3d
{
	public:

		//static constexpr float boltzmann = 5.670367e-8;

		static constexpr float side = 0.01f;//in meters
		static constexpr float area = side * side;
		static constexpr float volume = side * side * side;

		struct Material
		{
			public:

				enum class Type { Air, Concrete, Wood, Carpet, Fabrics } type = Type::Air;
			
				Material(Type type);

				float mass = 1.f;

				float density = 1.f;
				float specific_heat = 1.f;

				float lambda = 1.f; // conduction factor
				float alpha = 1.f; // convection factor ignored if 
		
				float generated_energy = 0.f;

				float ignition_temperature = 10e8f;
				float autoignition_temperature = 10e8f;

				float ignition_time = 2.f;
				float burning_time = 5.f;

				float smoke_emission = 1.f;

				bool solid = false;
				bool combustibility = false;

				glm::vec4 color = glm::vec4(0, 0, 0, 0);
		};

		struct Cell
		{
			Material material = Material(Material::Type::Air);
			enum class State { non_flammable, on_fire, empty } state = State::empty;
			float temperature = kelvin(21.f);

			float smoke_capacity = 1.0f;
			float smoke_contents = 0.0f;

			float timer = 0.f;
		};

		explicit Lattice3d(size_t N, const Cell& val = Cell{});

		Cell& operator()(size_t s, size_t r, size_t c);
		const Cell& get_or(size_t s, size_t r, size_t c, const Cell& val = Cell());

		void update(float dt);

		const size_t N;
	private:
		Cell& fg_at(size_t s, size_t r, size_t c);
		Cell& bg_at(size_t s, size_t r, size_t c);

		std::vector<Cell> foreground;
		std::vector<Cell> background;
};