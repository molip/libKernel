#pragma once

namespace Kernel
{
	template <typename T>
	class ConstReverseAdapter
	{
	public:
		explicit ConstReverseAdapter(const T& t) : m_t(t) {}

		typename T::const_reverse_iterator begin() const { return m_t.rbegin(); }
		typename T::const_reverse_iterator end() const { return m_t.rend(); }
	private:
		const T& m_t;
	};

	template <typename T>
	class ReverseAdapter
	{
	public:
		explicit ReverseAdapter(T& t) : m_t(t) {}

		typename T::reverse_iterator begin() const { return m_t.rbegin(); }
		typename T::reverse_iterator end() const { return m_t.rend(); }
	private:
		T& m_t;
	};

	template <typename T>
	ConstReverseAdapter<T> Reverse(const T& t) { return ConstReverseAdapter<T>(t); }

	template <typename T>
	ReverseAdapter<T> Reverse(T& t) { return ReverseAdapter<T>(t); }

	template <typename T>
	class Iterable
	{
	public:
		Iterable(T begin, T end) : m_begin(begin), m_end(end) {}

		T begin() const { return m_begin; }
		T end() const { return m_end; }

	private:
		T m_begin, m_end;
	};
}