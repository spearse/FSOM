/*
**  Copyright (C) 2011  Stephen Pearse and David Devaney
**
**  This file is part of FSOM (Free Sound Object Mixer).
**  FSOM is free software: you can redistribute it and/or modify
**  it under the terms of the GNU Lesser General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.

**  FSOM is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU Lesser General Public License for more details.

**  You should have received a copy of the GNU Lesser General Public License
**  along with FSOM.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __TABLE_HPP__
#define __TABLE_HPP__

#include <vector>
#include <cmath>
#include "Utilities.hpp"
#include <cstdlib>
#include <memory>

namespace fsom
{

template <class T>
class Table
{
  public:
	enum
	{
		kGuardValues = 1
	};
	typedef std::vector<T> TableDataType;
	typedef typename TableDataType::size_type size_type;
	typedef T element_type;
	typedef typename TableDataType::iterator iterator;
	typedef typename TableDataType::const_iterator const_iterator;

	Table(size_type size)
		: m_table(size, 0)
	{
		fill_sine();
	}

	void copy_guard_values() { m_table.at(m_table.size() - 1) = m_table.at(0); }

	void fill_zero() { std::fill(m_table.begin(), m_table.end(), T(0)); }

	void fill_sine()
	{
		T f = static_cast<T>(TWOPI / (m_table.size() - 1));
		for (typename TableDataType::size_type n = 0; n < m_table.size(); ++n)
		{
			m_table.at(n) = sin(T(n) * f);
		}
		copy_guard_values();
	}

	void fill_sine(float frequency)
	{
		for (typename TableDataType::size_type n = 0; n < m_table.size(); ++n)
		{
			m_table[n] = sin(TWOPI * (float)n * frequency / (float)m_table.size());
		}
		copy_guard_values();
	}
	void fill_cos()
	{
		T f = TWOPI / (m_table.size() - 1);
		for (typename TableDataType::size_type n = 0; n < m_table.size(); ++n)
		{
			m_table.at(n) = cos(T(n) * f);
		}
		copy_guard_values();
	}
	void fill_hann()
	{
		T f = TWOPI / (m_table.size() - 1);
		for (typename TableDataType::size_type n = 0; n < m_table.size(); ++n)
		{
			m_table.at(n) = 0.5f * (1.0f - cos(T(n) * f));
		}
	}
	void fill_linear()
	{
		T f = 1.0f / (m_table.size() - 1);
		for (typename TableDataType::size_type n = 0; n < m_table.size(); ++n)
		{
			m_table.at(n) = n * f;
		}
	}

	void fill_triangle()
	{
		T f = (1.0f / (m_table.size() - 1));
		for (typename TableDataType::size_type n = 0; n < m_table.size(); ++n)
		{
			if (n <= m_table.size() / 2)
			{
				m_table.at(n) = (((m_table.size() - n) * f) - 1.0f);
			}
			else
			{
				m_table.at(n) = ((n * f) - 1.0f);
			}
		}
		copy_guard_values();
	}

	void fill_linear_offset()
	{
		T f = 2.0f / (m_table.size() - 1);
		for (typename TableDataType::size_type n = 0; n < m_table.size(); ++n)
		{
			m_table.at(n) = (n * f) - 1.0f;
		}
	}
	void fill_clipped(float threshold)
	{
		T f = 2.0f / (m_table.size() - 1);
		float t;
		for (typename TableDataType::size_type n = 0; n < m_table.size(); ++n)
		{
			t = (n * f) - 1.0f;
			if (t > threshold)
			{
				m_table.at(n) = threshold;
			}
			else if (t < -threshold)
			{
				m_table.at(n) = -threshold;
			}
			else
			{
				m_table.at(n) = t;
			}
		}
	}
	void fill_noise()
	{
		for (typename TableDataType::size_type n = 0; n < m_table.size(); ++n)
		{
			m_table.at(n) = (((rand() % 1000) * 0.001f) * 2.0f) - 1.0f;
		}
	}
	void fill_square()
	{
		T f = 2.0f / (m_table.size() - 1);
		for (typename TableDataType::size_type n = 0; n < m_table.size(); ++n)
		{
			if (n <= m_table.size() / 2.0f)
			{
				m_table.at(n) = 1.0f;
			}
			else
			{
				m_table.at(n) = -1.0f;
			}
		}
		copy_guard_values();
	}

	T linear_lookup(T fIndex)
	{
		typename TableDataType::size_type aIndex, bIndex;
		double fIntPart, r, va, vb;
		r = modf(fIndex, &fIntPart);
		aIndex = static_cast<typename TableDataType::size_type>(fIntPart);
		bIndex = aIndex + 1;
		va = m_table[aIndex];
		vb = m_table[bIndex];
		return (T(1) - r) * va + r * vb;
	}

	T cubic_lookup(T index)
	{
		FSOM_ASSERT(false);
		return 0.f;
	}

	void print_table()
	{
		for (typename TableDataType::size_type n = 0; n < m_table.size(); ++n)
		{
			fsom::DebugStream << " " << m_table.at(n) << " ";
		}
		fsom::DebugStream << " " << std::endl;
	}

	typename TableDataType::size_type get_size() const
	{
		return m_table.size() - kGuardValues;
	}

	// element access
	T& at(size_type index) { return m_table.at(index); }
	const T& at(size_type index) const { return m_table.at(index); }

	// array style element access
	T& operator[](size_type index) { return m_table[index]; }
	const T& operator[](size_type index) const { return m_table[index]; }

	// iterator access for ranged for
	iterator begin() { return m_table.begin(); }
	iterator end() { return m_table.end(); }
	const_iterator cbegin() const { return m_table.cbegin(); }
	const iterator cend() const { return m_table.cend(); }

  private:
	TableDataType m_table;
};

typedef std::shared_ptr<Table<float> > TablePtr;
typedef std::vector<TablePtr> MultiTableBuffer;
typedef std::shared_ptr<MultiTableBuffer> MultiTablePtr;
}
#endif
