#include "vopview/Evaluator.h"
#include "vopview/VOPAdapter.h"
#include "vopview/Node.h"

#include <blueprint/Connecting.h>
#include <blueprint/Pin.h>

#include <vop/Node.h>
#include <vop/Evaluator.h>

namespace vopv
{

Evaluator::Evaluator(const std::shared_ptr<vop::Evaluator>& eval)
    : m_eval(eval)
{
    if (!m_eval) {
        m_eval = std::make_shared<vop::Evaluator>();
    }
}

void Evaluator::OnAddNode(const bp::Node& front)
{
    vop::NodePtr back = nullptr;
    if (front.get_type().is_derived_from<Node>()) {
        back = m_eval->FindNode(static_cast<const Node&>(front).GetName());
    }
    if (!back) {
        back = VOPAdapter::CreateBackFromFront(front);
    }
    if (!back) {
        return;
    }

    m_eval->AddNode(back);
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

    m_eval->RemoveNode(itr->second);
    m_nodes_map.erase(itr);
}

void Evaluator::OnClearAllNodes()
{
    m_eval->ClearAllNodes();
    m_nodes_map.clear();
}

vop::NodePtr Evaluator::QueryBackNode(const bp::Node& front_node) const
{
    auto itr = m_nodes_map.find(&front_node);
    return itr == m_nodes_map.end() ? nullptr : itr->second;
}


bool Evaluator::OnConnected(const bp::Connecting& conn)
{
    auto f_pin = conn.GetFrom();
    auto t_pin = conn.GetTo();

    auto f_itr = m_nodes_map.find(&f_pin->GetParent());
    auto t_itr = m_nodes_map.find(&t_pin->GetParent());
    if (f_itr == m_nodes_map.end() || t_itr == m_nodes_map.end()) {
        return false;
    }

    if (t_itr->first->GetAllInput().size() > t_itr->second->GetImports().size()) {
        t_itr->second->AddInputPorts(t_itr->first->GetAllInput().size() - t_itr->first->GetAllOutput().size());
    }

    hdiop::make_connecting<hdiop::VarType>(
        { f_itr->second, f_pin->GetPosIdx() },
        { t_itr->second, t_pin->GetPosIdx() }
    );

    return true;
}

bool Evaluator::OnDisconnecting(const bp::Connecting& conn)
{
    auto f_pin = conn.GetFrom();
    auto t_pin = conn.GetTo();

    auto f_itr = m_nodes_map.find(&f_pin->GetParent());
    auto t_itr = m_nodes_map.find(&t_pin->GetParent());
    if (f_itr == m_nodes_map.end() || t_itr == m_nodes_map.end()) {
        return false;
    }

    hdiop::disconnect<hdiop::VarType>(
        { f_itr->second, f_pin->GetPosIdx() },
        { t_itr->second, t_pin->GetPosIdx() }
    );

    return true;
}

bool Evaluator::OnRebuildConnection()
{
    bool dirty = false;

    for (auto& itr : m_nodes_map) {
        itr.second->ClearConnections();
    }

    for (auto& itr : m_nodes_map)
    {
        auto& front = itr.first;
        auto& back  = itr.second;
        for (auto& in : front->GetAllInput())
        {
            for (auto& conn : in->GetConnecting())
            {
                auto f_pin = conn->GetFrom();
                auto t_pin = conn->GetTo();

                auto f_itr = m_nodes_map.find(&f_pin->GetParent());
                auto t_itr = m_nodes_map.find(&t_pin->GetParent());
                if (f_itr == m_nodes_map.end() || t_itr == m_nodes_map.end()) {
                    continue;
                }

                if (t_itr->first->GetAllInput().size() > t_itr->second->GetImports().size()) {
                    t_itr->second->AddInputPorts(t_itr->first->GetAllInput().size() - t_itr->first->GetAllOutput().size());
                }

                hdiop::make_connecting<hdiop::VarType>(
                    { f_itr->second, f_pin->GetPosIdx() },
                    { t_itr->second, t_pin->GetPosIdx() }
                );
                dirty = true;
            }
        }
    }

    return dirty;
}

}