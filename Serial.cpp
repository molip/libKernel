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

SaveContext::SaveContext(SaveNode& node)
{
	// We only need one context - ignore nested. 
	if (!node.GetContext())
	{
		m_node = &node;
		node.SetContext(this);
	}
}

SaveContext::~SaveContext()
{
	if (m_node)
		m_node->SetContext(nullptr);
}

LoadContext::LoadContext(const LoadNode& node) : m_node(node)
{
	m_parent = node.GetContext();
	m_master = m_parent ? m_parent->m_master : this;
	m_node.SetContext(this);
}

LoadContext::~LoadContext()
{
	m_node.SetContext(m_parent);
	// Don't call ResolveRefs - we might be unwinding.
}

template <> std::string Kernel::Serial::FromString<std::string>(const std::string& s)
{
	return s;
}
