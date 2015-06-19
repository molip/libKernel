#pragma once

#include <sstream>
#include <string>

namespace Kernel  
{
	namespace Debug
	{
		class _Tracer;

		extern _Tracer Trace;

		class _Tracer
		{
		public:
			template <typename T> 
			_Tracer& operator << (T val)
			{
				if (Enabled())
				{
					std::ostringstream stream;
					stream << val;
					Output(stream.str());
				}
				return *this;
			}

			_Tracer& operator << (std::ostream& (*pf)(std::ostream&)) // For std::endl.
			{
				if (Enabled())
				{
					std::ostringstream stream;
					stream << pf;
					Output(stream.str());
				}
				return *this;
			}
			bool Enabled() const;
			void Output(const std::string& str);
		};
	}

	bool DebugBreak();

	class Exception : std::exception
	{
	public:
		Exception() : _file(""), _line(0) {}
		Exception(const char* file, int line, const char* msg = "") : _msg(msg), _file(file), _line(line) {}
		Exception(const char* file, int line, const std::string& msg) : _msg(msg.c_str()), _file(file), _line(line) {}

		virtual ~Exception() {}

		virtual const char* what() const override;

		virtual void Raise() const { throw *this; }

		std::string GetLocation() const;
		const std::string& GetMessage() const { return _msg; }

	protected:
		virtual std::string GetWhat() const;

	private:
		std::string _msg;
		const char* _file;
		int _line;

		mutable std::string _what;
	};

	void Verify(const void* condition, const Exception& e);
	void Verify(bool condition, const Exception& e);
}

#define KERNEL_VERIFY(condition) \
	Verify(condition, Exception(__FILE__, __LINE__))

#define KERNEL_VERIFY_MSG(msg, condition) \
	Verify(condition, Exception(__FILE__, __LINE__, msg))

#ifdef _DEBUG
#define KERNEL_ASSERT(f) (bool) ((f) || Kernel::DebugBreak())
#else
#define KERNEL_ASSERT(f) (f)
#endif