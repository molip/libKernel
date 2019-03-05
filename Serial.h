#pragma once

#include "Debug.h"
#include "Xml.h"
#include "Dynamic.h"
#include "EnumTraits.h"

#include <string>
#include <unordered_map>

namespace Kernel
{
	class Dynamic;

	namespace Serial
	{
		///////////////////////////////////////////////////////////////////////////////
		// Saving

		template <typename T> std::string ToString(const T& t)
		{
			std::ostringstream ss;
			ss << t;
			return ss.str();
		}

		class SaveContext
		{
		public:
			SaveContext(SaveNode& node);
			~SaveContext();

			int GetIDForObject(const void* obj)
			{
				return obj ? map.insert({ obj, (int)map.size() }).first->second : -1;
			}
		private:
			SaveNode* m_node{};
			std::unordered_map<const void*, int> map;
		};

		class SaveNode
		{
		public:
			explicit SaveNode(Xml::Element elem, SaveContext* ctx);

			template <typename T> void SaveType(const std::string& name, const T& val);
			template <typename T> void SaveEnum(const std::string& name, const T& val);
			template <typename T> void SaveClass(const std::string& name, const T& obj);
			template <typename T> void SaveClassPtr(const std::string& name, const T* pObj);
			template <typename T> void SaveTypePtr(const std::string& name, const T* pObj);
			template <typename T> void SaveObject(const std::string& name, const T* pObj);
			template <typename T> void SaveClassPtr(const std::string& name, const std::unique_ptr<T>& pObj);
			template <typename T> void SaveTypePtr(const std::string& name, const std::unique_ptr<T>& pObj);
			template <typename T> void SaveObject(const std::string& name, const std::unique_ptr<T>& pObj);

			template <typename T, typename S> void SaveCntr(const std::string& name, const T& cntr, S saver);
			template <typename T, typename S> void SaveArray(const std::string& name, const T& cntr, S saver);
			template <typename T, typename SK, typename SV> void SaveMap(const std::string& name, const T& map, SK keySaver, SV valSaver);
			template <typename T, typename S1, typename S2> void SavePairs(const std::string& name, const T& cntr, S1 saver1, S2 saver2);
			template <typename T, typename S1, typename S2> void SavePair(const std::string& name, const T& pair, S1 saver1, S2 saver2);

			void SetContext(SaveContext* ctx) { m_ctx = ctx; }
			SaveContext* GetContext() const { return m_ctx; }

			void SaveObjectID(const void* obj)
			{
				SaveType("_id", m_ctx->GetIDForObject(obj));
			}

			void SaveObjectRef(const std::string& name, const void* obj)
			{
				SaveType(name, m_ctx->GetIDForObject(obj));
			}

		private:
			Xml::Element m_elem;
			SaveContext* m_ctx;
		};

		struct TypeSaver
		{
			template <typename T> void operator ()(Xml::Element& e, const T& val, SaveContext* ctx) { SaveNode(e, ctx).SaveType("_value", val); }
		};

		struct EnumSaver
		{
			template <typename T> void operator ()(Xml::Element& e, const T& val, SaveContext* ctx) { SaveNode(e, ctx).SaveEnum("_enum", val); }
		};

		struct ClassSaver
		{
			template <typename T> void operator ()(Xml::Element& e, const T& obj, SaveContext* ctx) { obj.Save(SaveNode(e, ctx)); }
		};

		struct PtrSaver
		{
			template <typename T, typename S> void operator ()(Xml::Element& e, const T* pObj, S saver, SaveContext* ctx)
			{
				if (pObj)
					saver(e, *pObj, ctx);
				else
					e.SetAttribute("_null", true);
			}

			template <typename T, typename S> void operator ()(Xml::Element& e, const std::unique_ptr<T>& pObj, S saver, SaveContext* ctx)
			{
				operator ()(e, pObj.get(), saver, ctx);
			}
		};

		struct ClassPtrSaver
		{
			template <typename T> void operator ()(Xml::Element& e, const T* pObj, SaveContext* ctx)
			{
				PtrSaver()(e, pObj, ClassSaver(), ctx);
			}

			template <typename T> void operator ()(Xml::Element& e, const std::unique_ptr<T>& pObj, SaveContext* ctx)
			{
				operator ()(e, pObj.get(), ctx);
			}
		};

		struct TypePtrSaver
		{
			template <typename T> void operator ()(Xml::Element& e, const T* pObj, SaveContext* ctx)
			{
				PtrSaver()(e, pObj, TypeSaver(), ctx);
			}

			template <typename T> void operator ()(Xml::Element& e, const std::unique_ptr<T>& pObj, SaveContext* ctx)
			{
				operator ()(e, pObj.get(), ctx);
			}
		};

		struct ObjectSaver
		{
			template <typename T> void operator ()(Xml::Element& e, const T* pObj, SaveContext* ctx)
			{
				e.SetAttribute("_class", pObj ? typeid(*pObj).name() : "_null");
				if (pObj)
					pObj->Save(SaveNode(e, ctx));
			}

			template <typename T> void operator ()(Xml::Element& e, const std::unique_ptr<T>& pObj, SaveContext* ctx)
			{
				operator ()(e, pObj.get(), ctx);
			}
		};

		template <typename S>
		struct CntrSaver
		{
			template <typename T> void operator ()(Xml::Element& e, const T& cntr, SaveContext* ctx) { SaveNode(e, ctx).SaveCntr("container", cntr, S()); }
		};

		template <typename S1, typename S2>
		struct PairSaver
		{
			template <typename T> void operator ()(Xml::Element& e, const T& pair, SaveContext* ctx) { SaveNode(e, ctx).SavePair("pair", pair, S1(), S2()); }
		};

		template <typename T> void SaveNode::SaveType(const std::string& name, const T& val)
		{
			m_elem.SetAttribute(name, ToString(val));
		}

		template <typename T> void SaveNode::SaveEnum(const std::string& name, const T& val)
		{
			m_elem.SetAttribute(name, EnumToString(val));
		}

		template <typename T> void SaveNode::SaveClass(const std::string& name, const T& obj)
		{
			Xml::Element elem = m_elem.AddElement(name);
			ClassSaver()(elem, obj, m_ctx);
		}

		template <typename T> void SaveNode::SaveClassPtr(const std::string& name, const T* pObj)
		{
			Xml::Element elem = m_elem.AddElement(name);
			ClassPtrSaver()(elem, pObj, m_ctx);
		}

		template <typename T> void SaveNode::SaveTypePtr(const std::string& name, const T* pObj)
		{
			Xml::Element elem = m_elem.AddElement(name);
			TypePtrSaver()(elem, pObj, m_ctx);
		}

		template <typename T> void SaveNode::SaveObject(const std::string& name, const T* pObj)
		{
			Xml::Element elem = m_elem.AddElement(name);
			ObjectSaver()(elem, pObj, m_ctx);
		}

		template <typename T> void SaveNode::SaveClassPtr(const std::string& name, const std::unique_ptr<T>& pObj)
		{
			SaveClassPtr(name, pObj.get());
		}

		template <typename T> void SaveNode::SaveTypePtr(const std::string& name, const std::unique_ptr<T>& pObj)
		{
			SaveTypePtr(name, pObj.get());
		}

		template <typename T> void SaveNode::SaveObject(const std::string& name, const std::unique_ptr<T>& pObj)
		{
			SaveObject(name, pObj.get());
		}

		template <typename T, typename S> void SaveNode::SaveCntr(const std::string& name, const T& cntr, S saver)
		{
			Xml::Element elem = m_elem.AddElement(name);
			for (auto&& v : cntr)
			{
				Xml::Element elem2 = elem.AddElement("_item");
				saver(elem2, v, m_ctx);
			}
		}

		template <typename T, typename S1, typename S2> void SaveNode::SavePairs(const std::string& name, const T& cntr, S1 saver1, S2 saver2)
		{
			Xml::Element elem = m_elem.AddElement(name);
			for (auto&& v : cntr)
			{
				Xml::Element elem2 = elem.AddElement("_item");
				saver1(elem2.AddElement("_first"), v.first, m_ctx);
				saver2(elem2.AddElement("_second"), v.second, m_ctx);
			}
		}

		template <typename T, typename S1, typename S2> void SaveNode::SavePair(const std::string& name, const T& pair, S1 saver1, S2 saver2)
		{
			Xml::Element elem = m_elem.AddElement(name);
			saver1(elem.AddElement("_first"), pair.first, m_ctx);
			saver2(elem.AddElement("_second"), pair.second, m_ctx);
		}

		template <typename T, typename S> void SaveNode::SaveArray(const std::string& name, const T& cntr, S saver)
		{
			Xml::Element elem = m_elem.AddElement(name);
			for (auto&& v : cntr)
			{
				Xml::Element elem2 = elem.AddElement("_item");
				saver(elem2, v, m_ctx);
			}
		}

		template <typename T, typename SK, typename SV> void SaveNode::SaveMap(const std::string& name, const T& map, SK keySaver, SV valSaver)
		{
			Xml::Element elem = m_elem.AddElement(name);
			for (auto& kv : map)
			{
				Xml::Element item = elem.AddElement("_item");
				keySaver(item.AddElement("_k"), kv.first, m_ctx);
				valSaver(item.AddElement("_v"), kv.second, m_ctx);
			}
		}

		template <typename TPath, typename T> bool SaveClass(const TPath& path, const T& obj, SaveContext* ctx = nullptr)
		{
			Xml::Document doc;
			ClassSaver()(doc.AddElement("class"), obj, ctx);

			return doc.SaveToFile(path);
		}

		template <typename TPath, typename T> bool SaveObject(const TPath& path, const T* pObj, SaveContext* ctx = nullptr)
		{
			Xml::Document doc;
			ObjectSaver()(doc.AddElement("object"), pObj, ctx);

			return doc.SaveToFile(path);
		}


		///////////////////////////////////////////////////////////////////////////////
		// Loading

		class LoadContext
		{
		public:
			LoadContext(const LoadNode& node);
			~LoadContext();

			void RegisterObject(int id, void* obj)
			{
				KERNEL_ASSERT(id >= 0);
				m_master->objs.insert({ id, obj });
			}

			void RegisterRef(int id, void** obj)
			{
				if (id >= 0)
					refs.push_back({ id, obj });
			}

			void ResolveRefs()
			{
				for (const auto[index, ref] : refs)
					*ref = m_master->objs[index];

				refs.clear();
			}

		private:
			LoadContext* m_parent;
			LoadContext* m_master;
			const LoadNode& m_node;
			std::vector<std::pair<int, void**>> refs;
			std::unordered_map<int, void*> objs; // Only used in the top level context.
		};

		template <typename T> T FromString(const std::string& s)
		{
			std::istringstream ss(s);
			T t;
			ss >> t;
			return t;
		}

		template <> std::string FromString<std::string>(const std::string& s);

		class LoadNode
		{
		public:
			explicit LoadNode(Xml::Element elem, LoadContext* ctx);

			template <typename T> bool LoadType(const std::string& name, T& val) const;
			template <typename T> bool LoadEnum(const std::string& name, T& val) const;
			template <typename T> bool LoadClass(const std::string& name, T& obj) const;
			template <typename T> bool LoadClassPtr(const std::string& name, T*& pObj) const;
			template <typename T> bool LoadClassPtr(const std::string& name, std::unique_ptr<T>& pObj) const;
			template <typename T> bool LoadTypePtr(const std::string& name, T*& pObj) const;
			template <typename T> bool LoadTypePtr(const std::string& name, std::unique_ptr<T>& pObj) const;
			template <typename T> bool LoadObject(const std::string& name, T*& pObj) const;
			template <typename T> bool LoadObject(const std::string& name, std::unique_ptr<T>& pObj) const;

			template <typename T, typename L> bool LoadCntr(const std::string& name, T& cntr, L loader) const;
			template <typename T, typename L> bool LoadArray(const std::string& name, T& cntr, L loader) const;
			template <typename T, typename LK, typename LV> bool LoadMap(const std::string& name, T& map, LK keyLoader, LV valLoader) const;
			template <typename T, typename L1, typename L2> bool LoadPairs(const std::string& name, T& cntr, L1 loader1, L2 loader2) const;
			template <typename T, typename L1, typename L2> bool LoadPair(const std::string& name, T& pair, L1 loader1, L2 loader2) const;

			void LoadObjectID(void* obj) const
			{
				int id = -1;
				LoadType("_id", id);
				m_ctx->RegisterObject(id, obj);
			}

			template <typename T>
			void LoadObjectRef(const std::string& name, const T* const & obj) const
			{
				int id = -1;
				LoadType(name, id);
				m_ctx->RegisterRef(id, reinterpret_cast<void**>(const_cast<T**>(&obj)));
			}

			LoadContext* GetContext() const { return m_ctx; }
			void SetContext(LoadContext* ctx) const { m_ctx = ctx; }

		private:
			const Xml::Element m_elem;
			mutable LoadContext* m_ctx;
		};

		struct TypeLoader
		{
			template <typename T> void operator ()(const Xml::Element& e, T& val, LoadContext* ctx) { LoadNode(e, ctx).LoadType("_value", val); }
		};

		struct EnumLoader
		{
			template <typename T> void operator ()(const Xml::Element& e, T& val, LoadContext* ctx) { LoadNode(e, ctx).LoadEnum("_enum", val); }
		};

		struct ClassLoader
		{
			template <typename T> void operator ()(const Xml::Element& e, T& obj, LoadContext* ctx) { obj.Load(LoadNode(e, ctx)); }
		};

		struct PtrLoader
		{
			template <typename T, typename L> void operator ()(const Xml::Element& e, T*& pObj, L loader, LoadContext* ctx)
			{
				delete pObj;
				pObj = nullptr;

				bool bNull = false;
				e.QueryAttribute("_null", bNull);
				if (!bNull)
				{
					pObj = new T;
					loader(e, *pObj, ctx);
				}
			}
			template <typename T, typename L> void operator ()(const Xml::Element& e, std::unique_ptr<T>& pObj, L loader, LoadContext* ctx)
			{
				T* p = nullptr;
				operator()(e, p, loader, ctx);
				pObj.reset(p);
			}
		};

		struct ClassPtrLoader
		{
			template <typename T> void operator ()(const Xml::Element& e, T*& pObj, LoadContext* ctx)
			{
				PtrLoader()(e, pObj, ClassLoader(), ctx);
			}
			template <typename T> void operator ()(const Xml::Element& e, std::unique_ptr<T>& pObj, LoadContext* ctx)
			{
				PtrLoader()(e, pObj, ClassLoader(), ctx);
			}
		};

		struct TypePtrLoader
		{
			template <typename T> void operator ()(const Xml::Element& e, T*& pObj, LoadContext* ctx)
			{
				PtrLoader()(e, pObj, TypeLoader(), ctx);
			}
			template <typename T> void operator ()(const Xml::Element& e, std::unique_ptr<T>& pObj, LoadContext* ctx)
			{
				PtrLoader()(e, pObj, TypeLoader(), ctx);
			}
		};

		struct ObjectLoader
		{
			template <typename T> bool operator ()(const Xml::Element& e, T*& pObj, LoadContext* ctx)
			{
				std::string id = e.GetAttributeStr("_class");

				delete pObj;
				pObj = nullptr;

				if (id == "_null")
					return true;

				if (pObj = Dynamic::CreateObject<T>(id))
				{
					pObj->Load(LoadNode(e, ctx));
					return true;
				}
				return false;
			}
			template <typename T> bool operator ()(const Xml::Element& e, std::unique_ptr<T>& pObj, LoadContext* ctx)
			{
				T* p = nullptr;
				bool bOK = operator()(e, p, ctx);
				pObj.reset(p);
				return bOK;
			}
		};

		template <typename L>
		struct CntrLoader
		{
			template <typename T> void operator ()(const Xml::Element& e, T& cntr, LoadContext* ctx) { LoadNode(e, ctx).LoadCntr("container", cntr, L()); }
		};

		template <typename L1, typename L2>
		struct PairLoader
		{
			template <typename T> void operator ()(const Xml::Element& e, T& pair, LoadContext* ctx) { LoadNode(e, ctx).LoadPair("pair", pair, L1(), L2()); }
		};

		template <typename T> bool LoadNode::LoadType(const std::string& name, T& val) const
		{
			std::string attr;
			if (!m_elem.QueryAttribute(name, attr))
				return false;
			val = FromString<T>(attr);
			return true;
		}

		template <typename T> bool LoadNode::LoadEnum(const std::string& name, T& val) const
		{
			std::string s;
			if (LoadType(name, s))
			{
				val = ::EnumFromString<T>(s);
				return true;
			}
			return false;
		}

		template <typename T> bool LoadNode::LoadClass(const std::string& name, T& obj) const
		{
			const Xml::Element e = m_elem.GetFirstChild(name);
			if (e.IsNull())
				return false;

			ClassLoader()(e, obj, m_ctx);
			return true;
		}

		template <typename T> bool LoadNode::LoadClassPtr(const std::string& name, T*& pObj) const
		{
			const Xml::Element e = m_elem.GetFirstChild(name);
			if (e.IsNull())
				return false;

			ClassPtrLoader()(e, pObj, m_ctx);
			return true;
		}

		template <typename T> bool LoadNode::LoadClassPtr(const std::string& name, std::unique_ptr<T>& pObj) const
		{
			T* p = nullptr;
			bool bOK = LoadClassPtr(name, p);
			pObj.reset(p);
			return bOK;
		}

		template <typename T> bool LoadNode::LoadTypePtr(const std::string& name, T*& pObj) const
		{
			const Xml::Element e = m_elem.GetFirstChild(name);
			if (e.IsNull())
				return false;

			TypePtrLoader()(e, pObj, m_ctx);
			return true;
		}

		template <typename T> bool LoadNode::LoadTypePtr(const std::string& name, std::unique_ptr<T>& pObj) const
		{
			T* p = nullptr;
			bool bOK = LoadTypePtr(name, p);
			pObj.reset(p);
			return bOK;
		}

		template <typename T> bool LoadNode::LoadObject(const std::string& name, T*& pObj) const
		{
			pObj = nullptr;

			const Xml::Element e = m_elem.GetFirstChild(name);
			if (e.IsNull())
				return false;

			return ObjectLoader()(e, pObj, m_ctx);
		}

		template <typename T> bool LoadNode::LoadObject(const std::string& name, std::unique_ptr<T>& pObj) const
		{
			T* p = nullptr;
			bool bOK = LoadObject(name, p);
			pObj.reset(p);
			return bOK;
		}

		template <typename T, typename L> bool LoadNode::LoadCntr(const std::string& name, T& cntr, L loader) const
		{
			KERNEL_ASSERT(cntr.empty());

			const Xml::Element e = m_elem.GetFirstChild(name);
			if (e.IsNull())
				return false;

			for (const auto i : Xml::ElementRange(e, "_item"))
			{
				T::value_type v = T::value_type();
				loader(i, v, m_ctx);
				cntr.insert(cntr.end(), std::move(v));
			}
			return true;
		}

		template <typename T, typename L1, typename L2> bool LoadNode::LoadPairs(const std::string& name, T& cntr, L1 loader1, L2 loader2) const
		{
			KERNEL_ASSERT(cntr.empty());

			const Xml::Element e = m_elem.GetFirstChild(name);
			if (e.IsNull())
				return false;

			for (const auto i : Xml::ElementRange(e, "_item"))
			{
				const Xml::Element e1 = i.GetFirstChild("_first");
				T::value_type::first_type v1 = T::value_type::first_type();
				loader1(e1, v1, m_ctx);

				const Xml::Element e2 = i.GetFirstChild("_second");
				T::value_type::second_type v2 = T::value_type::second_type();
				loader2(e2, v2, m_ctx);

				cntr.insert(cntr.end(), std::make_pair(std::move(v1), std::move(v2)));
			}
			return true;
		}

		template <typename T, typename L1, typename L2> bool LoadNode::LoadPair(const std::string& name, T& pair, L1 loader1, L2 loader2) const
		{
			const Xml::Element e = m_elem.GetFirstChild(name);
			if (e.IsNull())
				return false;

			const Xml::Element e1 = e.GetFirstChild("_first");
			loader1(e1, pair.first, m_ctx);

			const Xml::Element e2 = e.GetFirstChild("_second");
			loader2(e2, pair.second, m_ctx);

			return true;
		}

		template <typename T, typename L> bool LoadNode::LoadArray(const std::string& name, T& cntr, L loader) const
		{
			const Xml::Element e = m_elem.GetFirstChild(name);
			if (e.IsNull())
				return false;

			int i = 0;
			for (const auto item : Xml::ElementRange(e, "_item"))
			{
				loader(item, cntr[i++], m_ctx);
			}
			return true;
		}

		template <typename T, typename LK, typename LV> bool LoadNode::LoadMap(const std::string& name, T& map, LK keyLoader, LV valLoader) const
		{
			KERNEL_ASSERT(map.empty());

			const Xml::Element e = m_elem.GetFirstChild(name);
			if (e.IsNull())
				return false;

			for (auto item : Xml::ElementRange(e, "_item"))
			{
				T::key_type key = T::key_type();
				T::mapped_type val = T::mapped_type();

				const Xml::Element k = item.GetFirstChild("_k");
				const Xml::Element v = item.GetFirstChild("_v");
				KERNEL_VERIFY(!k.IsNull() && !v.IsNull());

				keyLoader(k, key, m_ctx);
				valLoader(v, val, m_ctx);
				map.insert(std::make_pair(std::move(key), std::move(val)));
			}
			return true;
		}

		template <typename TPath, typename T> bool LoadClass(const TPath& path, T& obj, LoadContext* ctx = nullptr)
		{
			Xml::Document doc;
			if (doc.LoadFromFile(path))
			{
				const Xml::Element root = doc.GetRoot();
				if (!root.IsNull() && root.GetName() == "class")
				{
					try
					{
						ClassLoader()(doc.GetRoot(), obj, ctx);
					}
					catch (LoadException&)
					{
						return false;
					}
					return true;
				}
			}
			return false;
		}

		template <typename TPath, typename T> bool LoadObject(const TPath& path, T& pObj, LoadContext* ctx = nullptr)
		{
			Xml::Document doc;
			if (doc.LoadFromFile(path))
			{
				const Xml::Element root = doc.GetRoot();
				if (!root.IsNull() && root.GetName() == "object")
					try
				{
					return ObjectLoader()(doc.GetRoot(), pObj, m_ctx);
				}
				catch (LoadException& e)
				{
					return false;
				}
			}
			return false;
		}
	}
}