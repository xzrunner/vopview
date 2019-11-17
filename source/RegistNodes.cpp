#include "vopview/RegistNodes.h"

#include <ee0/ReflectPropTypes.h>

#define REGIST_NODE_RTTI(type, name, prop)                      \
	rttr::registration::class_<vopv::node::type>("vopv::"#name) \
		.constructor<>()                                        \
		prop                                                    \
	;

#define PROP
#define REGIST_NODE_RTTI_DEFAULT(type, name) REGIST_NODE_RTTI(type, name, PROP)

RTTR_REGISTRATION
{

// base
rttr::registration::class_<vopv::Node>("vopv::node")
.property("name", &vopv::Node::GetName, &vopv::Node::SetName)
(
	rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Name"))
)
;

// globals
REGIST_NODE_RTTI_DEFAULT(GeoGlobalParams, geo_global_params)
REGIST_NODE_RTTI_DEFAULT(GeoOutputVars, geo_output_vars)

}

namespace vopv
{

void nodes_regist_rttr()
{
}

}