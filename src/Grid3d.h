#pragma once
#include <vector>
#include <cassert>
#include <iostream>



template<typename T>
class Grid3d
{
	public:
		const size_t N;

		Grid3d(size_t N, const T& v = T{})
			: N{ N }, data(N* N* N, v)
		{}

		T& operator()(size_t s, size_t r, size_t c)
		{
			assert(s < N and r < N and c < N);
			return data[N * N * s + N * r + c];
		}

		const T& operator()(size_t s, size_t r, size_t c)const
		{
			assert(s < N and r < N and c < N);
			return data[N * N * s + N * r + c];
		}

		const T& get_or(size_t s, size_t r, size_t c, const T& d_val = T{})const
		{
			return (s < N and r < N and c < N) ? data[N * N * s + N * r + c] : d_val;
		}

		void set_if(size_t s, size_t r, size_t c, const T& val)
		{
			if (s < N and r < N and c < N)
			{
				data[N * N * s + N * r + c] = val;
			}
		}

	private:
		
		std::vector<T>data;
};

enum class StateOfMatter { Gas, Liquid, Solid };


struct Cell
{
	StateOfMatter state = StateOfMatter::Gas;
	float temperature;
	float mass;
	float capacity;
	float lambda;
	float alpha;
	
};