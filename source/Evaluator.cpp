#include "vopview/Evaluator.h"
#include "vopview/VOPAdapter.h"
#include "vopview/Node.h"

#include <vop/Node.h>

namespace vopv
{

void Evaluator::OnAddNode(const bp::Node& front)
{
    auto back = VOPAdapter::CreateBackFromFront(front);
    if (!back) {
        return;
    }

    m_nodes_map.insert({ &front, back });

    if (front.get_type().is_derived_from<Node>()) {
        const_cast<Node&>(static_cast<const Node&>(front)).SetName(back->GetName());
    }

    VOPAdapter::UpdatePropBackFromFront(front, *back, *this);
}

void Evaluator::OnRemoveNode(const bp::Node& node)
{
    auto itr = m_nodes_map.find(&node);
    if (itr == m_nodes_map.end()) {
        return;
    }

    m_nodes_map.erase(itr);
}

void Evaluator::OnClearAllNodes()
{
    m_nodes_map.clear();
}

vop::NodePtr Evaluator::QueryBackNode(const bp::Node& front_node) const
{
    auto itr = m_nodes_map.find(&front_node);
    return itr == m_nodes_map.end() ? nullptr : itr->second;
}

}