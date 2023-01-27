#include <algorithm>
#include <array>
#include <cstdint>
#include <tuple>
#include <type_traits>
#include <vector>

namespace htl
{

template<typename T, unsigned int D>
class planar
{
public:
	class scope
	{
	public:
		scope() = delete;

		std::array<uint64_t, D> lo;
		std::array<uint64_t, D> hi; 
	protected:
		scope(std::array<uint64_t, D> &t_lo, std::array<uint64_t, D> &t_hi)
			: lo(t_lo), hi(t_hi) {}
		scope(std::array<uint64_t, D> &&t_lo, std::array<uint64_t, D> &&t_hi)
			: lo(t_lo), hi(t_hi) {}
	private:
	};

	planar()
	{
		static_assert(1 <= D && D <= 3, "htl::planar: Template argument D must be an integer between 1 and 3");
		//static_assert(std::is_integral<T>(), "htl::planar: Template argument T must be of integer type");

		m_scope_dimensions = { 0 };
		m_scope_lo_coords = { 0 };
		m_scope_lo_coords_diff = { 0 };
		m_scope_hi_coords = { 0 };
		m_scope_hi_coords_diff = { 0 };
		m_working_coords_external = { 0 };
		m_working_coords_internal = { 0 };
	}

	inline scope get_scope()
	{
		return scope { m_scope_lo_coords, m_scope_hi_coords };
	}

	template <typename... Iargs>
	inline T get(Iargs... Fargs)
	{
		static_assert(sizeof...(Fargs) == D, "htl::planar: Incorrect number of indices passed to htl::planar::get()");

		uint64_t index = index_from_external_coords(Fargs...);

		return m_map.at(index);
	}

	template <typename... Iargs>
	void push_out(T element, Iargs... Fargs)
	{
		static_assert(sizeof...(Fargs) == D, "htl::planar: Incorrect number of indices passed to htl::planar::get()");

		uint8_t cd;

		move_external_coords(0, Fargs...);

		for (cd = 0; cd < D; ++cd)
		{
			m_scope_lo_coords_diff[cd] = m_scope_lo_coords[cd] - m_working_coords_external[cd];
			m_scope_hi_coords_diff[cd] = m_working_coords_external[cd] - m_scope_hi_coords[cd] + 1;
		}

		switch(D)
		{
		case 1:
			// X BOUND DECREASE
			if (m_scope_lo_coords_diff[0] > 0)
			{
				m_map.insert(m_map.begin(), m_scope_lo_coords_diff[0], T());
				m_map.insert(m_map.begin(), element);
				m_scope_lo_coords[0] = m_working_coords_external[0];
			}
			else
			// X BOUND INCREASE
			if (m_scope_hi_coords_diff[0] > 0)
			{
				m_map.insert(m_map.end(), m_scope_hi_coords_diff[0], T());
				m_map.push_back(element);
				m_scope_hi_coords[0] = m_working_coords_external[0];
			}

			m_scope_dimensions[0] = m_scope_hi_coords[0] - m_scope_lo_coords[0];

			break;
		case 2:
			// X BOUND DECREASE
			if (m_scope_lo_coords_diff[0] > 0)
			{
				uint64_t y;

				for (y = 0; y < m_scope_dimensions[1]; ++y)
				{
					m_map.insert(m_map.begin() + y * m_scope_dimensions[0] + y, m_scope_lo_coords_diff[0], T());
				}

				m_scope_lo_coords[0] = m_working_coords_external[0];
			}
			else
			// X BOUND INCREASE
			if (m_scope_hi_coords_diff[0] > 0)
			{
				uint64_t y;

				for (y = 1; y <= m_scope_dimensions[1]; ++y)
				{
					m_map.insert(m_map.begin() + y * m_scope_dimensions[0] + (y - 1), m_scope_hi_coords_diff[0], T());
				}

				m_scope_hi_coords[0] = m_working_coords_external[0];
			}

			m_scope_dimensions[0] = m_scope_hi_coords[0] - m_scope_lo_coords[0];

			// Y BOUND DECREASE
			if (m_scope_lo_coords_diff[1] > 0)
			{
				m_map.insert(m_map.begin(), m_scope_lo_coords_diff[1] * m_scope_dimensions[0], T());

				m_scope_lo_coords[1] = m_working_coords_external[1];
			}
			else
			// Y BOUND INCREASE
			if (m_scope_hi_coords_diff[1] > 0)
			{
				m_map.insert(m_map.end(), m_scope_hi_coords_diff[1] * m_scope_dimensions[0], T());

				m_scope_hi_coords[1] = m_working_coords_external[1];
			}

			m_scope_dimensions[1] = m_scope_hi_coords[1] - m_scope_lo_coords[1];

			break;
		case 3:
			break;
		}

		convert_coords_eti();

		m_map.at(index_from_internal_coords()) = element;
	}
	
protected:
private:
	std::vector<T> m_map;

	// Indexing Objects
	std::array<uint64_t, D> m_scope_dimensions;
	std::array<int64_t, D> m_scope_hi_coords;
	std::array<int64_t, D> m_scope_hi_coords_diff;
	std::array<int64_t, D> m_scope_lo_coords;
	std::array<int64_t, D> m_scope_lo_coords_diff;
	std::array<int64_t, D> m_working_coords_external;
	std::array<uint64_t, D> m_working_coords_internal;

	// Indexing Methods
	template<typename... Iargs>
	inline uint64_t index_from_external_coords(Iargs... Fargs)
	{
		move_external_coords(0, Fargs...);
		convert_coords_eti();

		return index_from_internal_coords();
	}
	
	inline uint64_t index_from_external_coords()
	{
		convert_coords_eti();

		return index_from_internal_coords();
	}

	inline uint64_t index_from_internal_coords()
	{
		switch(D)
		{
		case 1:
			return m_working_coords_internal[0];
		case 2:
			return m_working_coords_internal[0] + m_working_coords_internal[1] * m_scope_dimensions[0];
		case 3:
			return m_working_coords_internal[0] + m_working_coords_internal[1] * m_scope_dimensions[0] + m_working_coords_internal[2] * m_scope_dimensions[1];
		}
	}

	// External-Internal Conversion and Movement Methods
	template<typename I>
	inline void move_external_coords(uint8_t i, I coord)
	{
		m_working_coords_external[i] = coord;
	}

	template<typename I, typename... Iargs>
	inline void move_external_coords(uint8_t i, I coord, Iargs... Fargs)
	{
		m_working_coords_external[i] = coord;

		move_external_coords(i + 1, Fargs...);
	}

	inline void convert_coords_eti()
	{
		uint8_t i;
		for (i = 0; i < D; ++i)
			m_working_coords_internal[i] = m_working_coords_external[i] - m_scope_lo_coords[i];
	}

	inline void convert_coords_ite()
	{
		uint8_t i;
		for (i = 0; i < D; ++i)
			m_working_coords_external[i] = m_working_coords_internal[i] + m_scope_lo_coords[i];
	}
};

}
