#include "vopview/VOPAdapter.h"
#include "vopview/RegistNodes.h"
#include "vopview/Evaluator.h"

#include <blueprint/Pin.h>

#include <vop/Node.h>
#include <vop/Evaluator.h>
// utility
#include <vop/node/Compare.h>
#include <vop/node/Constant.h>
// vector
#include <vop/node/GetVectorComponent.h>
#include <vop/node/SetVectorComponent.h>

namespace vopv
{

void VOPAdapter::UpdatePropBackFromFront(const bp::Node& front, vop::Node& back,
                                         const Evaluator& eval)
{
    auto type = front.get_type();
    // utility
    if (type == rttr::type::get<node::Compare>())
    {
        auto& src = static_cast<const node::Compare&>(front);
        auto& dst = static_cast<vop::node::Compare&>(back);
        dst.SetCmpType(src.type);
    }
    else if (type == rttr::type::get<node::Constant>())
    {
        auto& src = static_cast<const node::Constant&>(front);
        auto& dst = static_cast<vop::node::Constant&>(back);
        dst.SetConstType(src.type);
        switch (src.type)
        {
        case vop::node::Constant::Type::ConstFloat:
            dst.SetConstValue(dag::Variable(src.value.x));
            break;
        //case vop::node::Constant::Type::ConstFloat2:
        //    dst.SetConstValue(dag::Variable(sm::vec2(src.value.x, src.value.y)));
        //    break;
        case vop::node::Constant::Type::ConstFloat3:
            dst.SetConstValue(dag::Variable(sm::vec3(src.value.xyzw)));
            break;
        default:
            assert(0);
        }
        dst.SetConstName(src.name);
    }
    // vector
    else if (type == rttr::type::get<node::GetVec2Comp>())
    {
        auto& src = static_cast<const node::GetVec2Comp&>(front);
        auto& dst = static_cast<vop::node::GetVec2Comp&>(back);
        dst.SetCompIndex(src.comp_idx);
    }
    else if (type == rttr::type::get<node::SetVec2Comp>())
    {
        auto& src = static_cast<const node::SetVec2Comp&>(front);
        auto& dst = static_cast<vop::node::SetVec2Comp&>(back);
        dst.SetCompIndex(src.comp_idx);
    }
    else if (type == rttr::type::get<node::GetVec3Comp>())
    {
        auto& src = static_cast<const node::GetVec3Comp&>(front);
        auto& dst = static_cast<vop::node::GetVec3Comp&>(back);
        dst.SetCompIndex(src.comp_idx);
    }
    else if (type == rttr::type::get<node::SetVec3Comp>())
    {
        auto& src = static_cast<const node::SetVec3Comp&>(front);
        auto& dst = static_cast<vop::node::SetVec3Comp&>(back);
        dst.SetCompIndex(src.comp_idx);
    }
    else if (type == rttr::type::get<node::GetVec4Comp>())
    {
        auto& src = static_cast<const node::GetVec4Comp&>(front);
        auto& dst = static_cast<vop::node::GetVec4Comp&>(back);
        dst.SetCompIndex(src.comp_idx);
    }
    else if (type == rttr::type::get<node::SetVec4Comp>())
    {
        auto& src = static_cast<const node::SetVec4Comp&>(front);
        auto& dst = static_cast<vop::node::SetVec4Comp&>(back);
        dst.SetCompIndex(src.comp_idx);
    }
}

std::shared_ptr<vop::Node>
VOPAdapter::CreateBackFromFront(const bp::Node& node)
{
    auto type = node.get_type();
    auto src_type = type.get_name().to_string();
    std::string dst_type;
    std::string lib_str = "vop";
    auto find_lib = src_type.find("vopv::");
    if (find_lib != std::string::npos) {
        dst_type = lib_str + "::" + src_type.substr(find_lib + strlen("vopv::"));
    }

    std::shared_ptr<vop::Node> dst = nullptr;

    if (!dst_type.empty())
    {
	    rttr::type t = rttr::type::get_by_name(dst_type);
        // fixme: specify node type
	    if (!t.is_valid())
        {
            assert(0);
	    }
        else
        {
            rttr::variant var = t.create();
            assert(var.is_valid());

            dst = var.get_value<std::shared_ptr<vop::Node>>();
            assert(dst);
        }
    }

    if (node.get_type().is_derived_from<Node>()) {
        dst->SetName(static_cast<const Node&>(node).GetName());
    }

    return dst;
}

std::shared_ptr<Node>
VOPAdapter::CreateFrontFromBack(const vop::Node& node)
{
    auto type = node.get_type();
    auto src_type = type.get_name().to_string();
    std::string dst_type;
    std::string lib_str = "vopv";
    auto find_lib = src_type.find("vop::");
    if (find_lib != std::string::npos) {
        dst_type = lib_str + "::" + src_type.substr(find_lib + strlen("vop::"));
    }

    std::shared_ptr<Node> dst = nullptr;

    if (!dst_type.empty())
    {
	    rttr::type t = rttr::type::get_by_name(dst_type);
	    if (!t.is_valid())
        {
            assert(0);
	    }
        else
        {
            rttr::variant var = t.create();
            assert(var.is_valid());

            dst = var.get_value<std::shared_ptr<Node>>();
            assert(dst);

            dst->SetName(node.GetName());
        }
    }

    return dst;
}

void VOPAdapter::UpdateBackEval(const std::shared_ptr<vop::Evaluator>& dst_eval,
                                const std::vector<bp::NodePtr>& src_nodes)
{
    dst_eval->ClearAllNodes();
    Evaluator eval(dst_eval);
    for (auto& n : src_nodes) {
        eval.OnAddNode(*n);
    }
    eval.OnRebuildConnection();
}

int VOPAdapter::TypeBackToFront(dag::VarType type)
{
    int ret = -1;
    switch (type)
    {
    case dag::VarType::Any:
        ret = bp::PIN_ANY_VAR;
        break;
    case dag::VarType::Bool:
        ret = bp::PIN_BOOLEAN;
        break;
    case dag::VarType::Int:
        ret = bp::PIN_INTEGER;
        break;
    case dag::VarType::Float:
        ret = bp::PIN_FLOAT1;
        break;
    case dag::VarType::Float2:
        ret = bp::PIN_FLOAT2;
        break;
    case dag::VarType::Float3:
        ret = bp::PIN_FLOAT3;
        break;
    case dag::VarType::Float4:
        ret = bp::PIN_FLOAT4;
        break;
    case dag::VarType::String:
        ret = bp::PIN_STRING;
        break;
    default:
        assert(0);
    }
    return ret;
}

}