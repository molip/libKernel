#include "Xml.h"
#include "Debug.h"
#include "tinyxml2/tinyxml2.h"

using namespace tinyxml2;

using namespace Kernel::Xml;

Document::Document()
{
	m_pDoc.reset(new XMLDocument);
}

Document::~Document() {}

std::string Document::SaveToString() const
{
	XMLPrinter printer;
	m_pDoc->Accept(&printer);
	return printer.CStr();
}

bool Document::LoadFromString(const std::string str) const
{
	m_pDoc->Parse(str.c_str());
	return !m_pDoc->Error();
}

bool Document::LoadFromFile(const std::string& path)
{
	return m_pDoc->LoadFile(path.c_str()) == XML_SUCCESS;
}

bool Document::SaveToFile(const std::string& path) const
{
	return m_pDoc->SaveFile(path.c_str()) == XML_SUCCESS;
}

Element Document::AddElement(const std::string& name)
{
	XMLElement* pElem = m_pDoc->NewElement(name.c_str());
	m_pDoc->LinkEndChild(pElem);
	return Element(pElem, m_pDoc.get());
}

Element Document::GetRoot()
{
	auto pRoot = m_pDoc->RootElement();
	KERNEL_VERIFY(!!pRoot);
	return Element(pRoot, m_pDoc.get());
}

//-----------------------------------------------------------------------------

Element::Element(XMLElement* pElem, tinyxml2::XMLDocument* pDoc) : m_pElem(pElem), m_pDoc(pDoc)
{
}

std::string Element::GetName() const
{
	KERNEL_VERIFY(!!m_pElem);
	return m_pElem->Name();
}

Element Element::AddElement(const std::string& name)
{
	KERNEL_VERIFY(!!m_pElem);
	XMLElement* pElem = m_pDoc->NewElement(name.c_str());
	m_pElem->LinkEndChild(pElem);
	return Element(pElem, m_pDoc);
}

void Element::SetAttribute(const std::string& name, const std::string& val)
{
	KERNEL_VERIFY(!!m_pElem);
	m_pElem->SetAttribute(name.c_str(), val.c_str());
}

void Element::SetAttribute(const std::string& name, int val)
{
	KERNEL_VERIFY(!!m_pElem);
	m_pElem->SetAttribute(name.c_str(), val);
}

bool Element::QueryAttribute(const std::string& name, std::string& val) const
{
	KERNEL_VERIFY(!!m_pElem);
	if (auto str = m_pElem->Attribute(name.c_str()))
	{
		val = str;
		return true;
	}
	return false;
}

bool Element::QueryAttribute(const std::string& name, int& val) const
{
	KERNEL_VERIFY(!!m_pElem);
	return m_pElem->QueryIntAttribute(name.c_str(), &val) == XML_SUCCESS;
}

bool Element::QueryAttribute(const std::string& name, bool& val) const
{
	KERNEL_VERIFY(!!m_pElem);
	return m_pElem->QueryBoolAttribute(name.c_str(), &val) == XML_SUCCESS;
}

std::string Element::GetAttributeStr(const std::string& name) const
{
	std::string val;
	KERNEL_VERIFY(QueryAttribute(name, val));
	return val;
}

int Element::GetAttributeInt(const std::string& name) const
{
	int val;
	KERNEL_VERIFY(QueryAttribute(name, val));
	return val;
}

bool Element::GetAttributeBool(const std::string& name) const
{
	bool val;
	KERNEL_VERIFY(QueryAttribute(name, val));
	return val;
}

Element Element::GetFirstChild(const std::string& name) const
{
	KERNEL_VERIFY(!!m_pElem);
	auto pEl = m_pElem->FirstChildElement(name.c_str());
	return Element(pEl, m_pDoc);
}

Element Element::GetNextSibling(const std::string& name) const
{
	KERNEL_VERIFY(!!m_pElem);
	return Element(m_pElem->NextSiblingElement(name.c_str()), m_pDoc);
}
