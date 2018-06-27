#pragma once

#include <limits>

namespace Kernel
{
	template <typename Object, typename Value = double>
	class MinFinder
	{
	public:
		MinFinder() : m_value(std::numeric_limits<Value>::max()) {}
		explicit MinFinder(Value threshold) : m_value(threshold) {}

		void SetThreshold(Value threshold) { m_value = threshold; }

		bool Try(const Object& object, Value value)
		{
			if (value < m_value)
			{
				m_value = value;
				m_object = object;
				return true;
			}
			
			return false;
		}

		const Object& GetObject() const { return m_object; }
		Value GetValue() const { return m_value; }

	private:
		Object m_object{};
		Value m_value;
	};
}