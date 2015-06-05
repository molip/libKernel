#include "Serial.h"

#include <sstream>
#include <memory>

using namespace Kernel;
using namespace Kernel::Serial;

SaveNode::SaveNode(Xml::Element elem) : m_elem(elem) 
{
	KERNEL_VERIFY(!m_elem.IsNull());
}

LoadNode::LoadNode(Xml::Element elem) : m_elem(elem) 
{
	KERNEL_VERIFY(!m_elem.IsNull());
}

template <> std::string Kernel::Serial::FromString<std::string>(const std::string& s)
{
	return s;
}
