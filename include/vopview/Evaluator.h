#pragma once

#include <blueprint/typedef.h>

#include <vop/typedef.h>

#include <boost/noncopyable.hpp>

#include <unordered_map>

namespace bp { class Connecting; }
namespace vop { class Evaluator; }

namespace vopv
{

class Evaluator : boost::noncopyable
{
public:
    Evaluator(const std::shared_ptr<vop::Evaluator>& eval);

    // update nodes
    void OnAddNode(const bp::Node& node);
    void OnRemoveNode(const bp::Node& node);
    void OnClearAllNodes();

    // update node conn
    bool OnConnected(const bp::Connecting& conn);
    bool OnDisconnecting(const bp::Connecting& conn);
    bool OnRebuildConnection();

    auto GetEval() const { return m_eval; }
    //auto& GetAllNodes() const { return m_nodes_map; }

    vop::NodePtr QueryBackNode(const bp::Node& front_node) const;

private:
    std::shared_ptr<vop::Evaluator> m_eval = nullptr;

    std::unordered_map<const bp::Node*, vop::NodePtr> m_nodes_map;

}; // Evaluator

}