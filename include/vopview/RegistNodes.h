#pragma once

#include "vopview/Node.h"

#include <vop/node/Constant.h>
#include <vop/node/Compare.h>
#include <vop/ComponentType.h>

namespace vopv
{

void nodes_regist_rttr();

namespace node
{

#define VOPV_DEFINE_NODE(type, name, prop) \
class type : public Node                   \
{                                          \
public:                                    \
	type()                                 \
		: Node(#type)                      \
	{                                      \
		InitPins(#name);                   \
	}                                      \
                                           \
	prop                                   \
	RTTR_ENABLE(Node)                      \
};

#define VOPV_NODE_PROP

class Subnetwork : public Node
{
public:
    Subnetwork(const std::string& title = "Subnetwork")
        : Node(title, true)
    {
        InitPins("subnetwork");
    }

    std::vector<bp::NodePtr> children;

    RTTR_ENABLE(Node)
};

// globals
VOPV_DEFINE_NODE(GeoGlobalParams, geo_global_params, VOPV_NODE_PROP)
VOPV_DEFINE_NODE(GeoOutputVars, geo_output_vars, VOPV_NODE_PROP)

// math
VOPV_DEFINE_NODE(Add, add, VOPV_NODE_PROP)
VOPV_DEFINE_NODE(Multiply, multiply, VOPV_NODE_PROP)

// utility
VOPV_DEFINE_NODE(Compare, compare,
    vop::node::Compare::CmpType type = vop::node::Compare::CmpType::Equal; \
)
VOPV_DEFINE_NODE(Constant, constant,
    vop::node::Constant::Type type  = vop::node::Constant::Type::ConstFloat; \
    sm::vec4                  value = sm::vec4(0, 0, 0, 0);                  \
    std::string               name;                                          \
)

// vector
VOPV_DEFINE_NODE(GetVec2Comp, get_vec2_comp,
    vop::CompType2 comp_idx = vop::CompType2::X; \
)
VOPV_DEFINE_NODE(SetVec2Comp, set_vec2_comp,
    vop::CompType2 comp_idx = vop::CompType2::X; \
)
VOPV_DEFINE_NODE(GetVec3Comp, get_vec3_comp,
    vop::CompType3 comp_idx = vop::CompType3::X; \
)
VOPV_DEFINE_NODE(SetVec3Comp, set_vec3_comp,
    vop::CompType3 comp_idx = vop::CompType3::X; \
)
VOPV_DEFINE_NODE(GetVec4Comp, get_vec4_comp,
    vop::CompType4 comp_idx = vop::CompType4::X; \
)
VOPV_DEFINE_NODE(SetVec4Comp, set_vec4_comp,
    vop::CompType4 comp_idx = vop::CompType4::X; \
)

}

}