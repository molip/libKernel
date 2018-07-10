#include "Serial.h"

#include <sstream>
#include <memory>

using namespace Kernel;
using namespace Kernel::Serial;

SaveNode::SaveNode(Xml::Element elem, SaveContext* ctx) : m_elem(elem), m_ctx(ctx)
{
	KERNEL_VERIFY(!m_elem.IsNull());
}

LoadNode::LoadNode(Xml::Element elem, LoadContext* ctx) : m_elem(elem), m_ctx(ctx)
{
	KERNEL_VERIFY(!m_elem.IsNull());
}

SaveContext::SaveContext(SaveNode& node) : m_node(node)
{
	m_node.SetContext(this); // TODO: push/pop.
}

SaveContext::~SaveContext()
{
	m_node.SetContext(nullptr);
}

LoadContext::LoadContext(const LoadNode& node) : m_node(node)
{
	m_node.SetContext(this); // TODO: push/pop.
}

LoadContext::~LoadContext()
{
	m_node.SetContext(nullptr);
	// Don't call ResolveRefs - we might be unwinding.
}

template <> std::string Kernel::Serial::FromString<std::string>(const std::string& s)
{
	return s;
}
