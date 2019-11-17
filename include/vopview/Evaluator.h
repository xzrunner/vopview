#pragma once

#include <blueprint/typedef.h>

#include <vop/typedef.h>

#include <boost/noncopyable.hpp>

#include <unordered_map>

namespace vopv
{

class Evaluator : boost::noncopyable
{
public:
    // update nodes
    void OnAddNode(const bp::Node& node);
    void OnRemoveNode(const bp::Node& node);
    void OnClearAllNodes();

    auto& GetAllNodes() const { return m_nodes_map; }

    vop::NodePtr QueryBackNode(const bp::Node& front_node) const;

private:
    std::unordered_map<const bp::Node*, vop::NodePtr> m_nodes_map;

}; // Evaluator

}