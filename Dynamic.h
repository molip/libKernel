#pragma once

#include "Debug.h"

#include <map>
#include <functional>
#include <string>

namespace Kernel
{
	namespace Serial { class SaveNode; class LoadNode; }

	class LoadException : public Exception { using Exception::Exception; };
	#define KERNEL_VERIFY_LOAD(condition) Verify(condition, LoadException(__FILE__, __LINE__))

	class Dynamic
	{
		typedef std::function<Dynamic*()> Func;
		typedef std::map<std::string, Func> ClassMap;
	public:
		virtual ~Dynamic();

		virtual void Save(Serial::SaveNode& node) const {}
		virtual void Load(const Serial::LoadNode& node) {}

		static void RegisterObject(const std::string& id, Func fn);

		template <typename T> static T* CreateObject(const std::string& id)
		{
			auto it = GetClassMap().find(id);
			KERNEL_VERIFY_LOAD(it != GetClassMap().end());

			T* pObj = dynamic_cast<T*>(it->second());
			KERNEL_VERIFY_LOAD(pObj);

			return pObj;
		}

	private:
		static ClassMap& GetClassMap();
	};

	#define REGISTER_DYNAMIC(T) \
		namespace { \
			struct _Register_##T {\
				_Register_##T() { \
					Kernel::Dynamic::RegisterObject(typeid(T).name(), [] { return new T; } ); \
				} \
			} _register_##T; \
		}
}