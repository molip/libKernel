#pragma once

#include "Debug.h"

#include <bitset>

namespace Kernel
{
	template <typename E>
	class EnumBitset : public std::bitset<(int)E::_Count>
	{
		using Base = std::bitset<(int)E::_Count>;
	public:
		EnumBitset() : Base({}) {}
		EnumBitset(std::initializer_list<E> i)
		{
			for (E e : i)
				set((int)e);
		}

		bool operator[] (E e) const { return Base::operator[]((int)e); }
		typename Base::reference operator[] (E e) { return Base::operator[]((int)e); }
	};
}