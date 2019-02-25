#pragma once

#include <limits>

namespace Kernel
{
	template <typename Object, typename Value = double>
	class MinFinder
	{
	public:
		MinFinder() : m_value(std::numeric_limits<Value>::max()), m_threshold() {}
		explicit MinFinder(Value threshold) : m_value(threshold), m_threshold(threshold) {}

		void SetThreshold(Value threshold) { m_value = m_threshold = threshold; }

		bool Try(const Object& object, Value value, int priority = 0)
		{
			if (priority < m_priority || value >= m_threshold)
				return false;

			if (value < m_value || priority > m_priority)
			{
				m_value = value;
				m_object = object;
				m_priority = priority;
				return true;
			}
			
			return false;
		}

		const Object& GetObject() const { return m_object; }
		Value GetValue() const { return m_value; }

	private:
		Object m_object{};
		Value m_value, m_threshold;
		int m_priority = -1;
	};
}