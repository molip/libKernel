#pragma once

#include <functional>

namespace Kernel
{
	template <typename T> class IndexIter
	{
	public:
		IndexIter(std::function<T(size_t)> fnGet, size_t i) : m_index(i), m_fnGet(fnGet) {}
		bool operator !=(const IndexIter& rhs) const { return m_index != rhs.m_index; }
		T operator* () const { return m_fnGet(m_index); }
		void operator++ () { ++m_index; }
	private:
		size_t m_index;
		std::function<T(size_t)> m_fnGet;
	};
 
	template <typename T> class IndexRange
	{
	public:
		IndexRange(std::function<T(size_t)> fnGet, size_t count) : m_fnGet(fnGet), m_count(count) {}
		IndexIter<T> begin() const { return IndexIter<T>(m_fnGet, 0); }
		IndexIter<T> end() const { return IndexIter<T>(m_fnGet, m_count); }
	private:
		std::function<T(size_t)> m_fnGet;
		size_t m_count;
	};

	#define KERNEL_DEFINE_INDEXRANGE(rangename, classname, type, getTypeFn, getCountFn) \
	class rangename : public Kernel::IndexRange<type>\
	{\
	public:\
		rangename(const classname& obj) : IndexRange( [&](size_t i) { return obj.getTypeFn(i); }, obj.getCountFn()) {}\
	};
}