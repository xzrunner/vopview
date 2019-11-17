#include "vopview/VOPAdapter.h"
#include "vopview/RegistNodes.h"

#include <vop/Node.h>

namespace vopv
{

void VOPAdapter::UpdatePropBackFromFront(const bp::Node& front,
                                  vop::Node& back,
                                  const Evaluator& eval)
{
}

vop::NodePtr VOPAdapter::CreateBackFromFront(const bp::Node& node)
{
    auto type = node.get_type();
    auto src_type = type.get_name().to_string();
    std::string dst_type;
    std::string lib_str = "vop";
    auto find_lib = src_type.find("vopv::");
    if (find_lib != std::string::npos) {
        dst_type = lib_str + "::" + src_type.substr(find_lib + strlen("vopv::"));
    }

    vop::NodePtr dst = nullptr;

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

}