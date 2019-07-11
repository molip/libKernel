#pragma once

#include <functional>

namespace Kernel
{
	template <typename T> class IndexRange;

	template <typename T> class IndexIter
	{
	public:
		IndexIter(const IndexRange<T>& range, size_t i) : m_index(i), m_range(range) {}
		bool operator !=(const IndexIter& rhs) const { return m_index != rhs.m_index; }
		T operator* () const;
		void operator++ () { ++m_index; }
	private:
		size_t m_index;
		const IndexRange<T>& m_range;
	};
 
	template <typename T> class IndexRange
	{
	public:
		IndexRange(std::function<T(size_t)> fnGet, size_t count, bool reverse = false) : m_fnGet(fnGet), m_count(count), m_reverse(reverse) {}
		IndexIter<T> begin() const { return IndexIter<T>(*this, 0); }
		IndexIter<T> end() const { return IndexIter<T>(*this, m_count); }
		T Get(size_t index) const
		{
			return m_fnGet(m_reverse ? m_count - 1 - index : index);
		}
	private:
		std::function<T(size_t)> m_fnGet;
		size_t m_count;
		bool m_reverse;
	};

	template <typename T> 
	T IndexIter<T>::operator* () const { return m_range.Get(m_index); }

	#define KERNEL_DEFINE_INDEXRANGE(rangename, classname, type, getTypeFn, getCountFn) \
	class rangename : public Kernel::IndexRange<type>\
	{\
	public:\
		rangename(const classname& obj) : IndexRange( [&](size_t i) { return obj.getTypeFn(i); }, obj.getCountFn()) {}\
	};
}