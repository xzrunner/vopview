#pragma once

#include "vopview/Node.h"

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

// globals
VOPV_DEFINE_NODE(GeoGlobalParams, geo_global_params, VOPV_NODE_PROP)
VOPV_DEFINE_NODE(GeoOutputVars, geo_output_vars, VOPV_NODE_PROP)

}

}