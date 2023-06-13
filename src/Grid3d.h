#pragma once
#include <vector>
#include <cassert>
#include <iostream>
#include <concepts>


// {} -> T
template<typename Fn, typename T>
concept NullaryFunction = std::invocable<Fn> and std::same_as<std::invoke_result_t<Fn>, T>;

// T -> T
template<typename Fn, typename T>
concept UnaryFunction = std::invocable<Fn, T> and std::same_as<std::invoke_result_t<Fn, T>, T>;


//(i, j, k) -> T
template<typename Fn, typename T>
concept IndexFunction = std::invocable<Fn, size_t, size_t, size_t> and std::same_as<std::invoke_result_t<Fn, size_t, size_t, size_t>, T>;


template<typename Fn, typename T>
concept IndexValFunction = std::invocable<Fn, size_t, size_t, size_t, T> and std::same_as<std::invoke_result_t<Fn, size_t, size_t, size_t, T>, T>;




template<typename T>
class Grid3d
{
	public:
		explicit Grid3d(size_t N, const T& val = T{})
			: N{N}, data(N * N * N, val)
		{}

		const T& operator()(size_t s, size_t r, size_t c)const
		{
			assert(s < N and r < N and c < N);
			return data[N * N * s + N * r + c];
		}

		T& operator()(size_t s, size_t r, size_t c)
		{
			assert(s < N and r < N and c < N);
			return data[N * N * s + N * r + c];
		}

		const T& get_or(size_t s, size_t r, size_t c, const T& d_val = T{})const
		{
			return (s < N and r < N and c < N) ? data[N * N * s + N * r + c] : d_val;
		}


		template<bool Ignore_return = false>
		void apply_for_each(const IndexValFunction<T> auto& fn)
		{
			for (size_t s = 0u; s < N; ++s)
			{
				for (size_t r = 0u; r < N; ++r)
				{
					for (size_t c = 0u; c < N; ++c)
					{
						if constexpr (not Ignore_return)
							(*this)(s, r, c) = fn(s, r, c, (*this)(s, r, c));
						else
							fn(s, r, c, (*this)(s, r, c));
					}
				}
			}
		}


	private:
		size_t N;
		std::vector<T> data;
};




struct Cell
{
	//enum class Type { Air } cell_type = Type::Air;
	float temperature;
};