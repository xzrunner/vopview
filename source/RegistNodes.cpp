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

// math
REGIST_NODE_RTTI_DEFAULT(Add, add)
REGIST_NODE_RTTI_DEFAULT(Multiply, multiply)

// utility
REGIST_NODE_RTTI(Compare, compare,
.property("type", &vopv::node::Compare::type)                             \
(                                                                         \
    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Compare Type")) \
)                                                                         \
)
REGIST_NODE_RTTI(Constant, constant,
.property("type", &vopv::node::Constant::type)                              \
(                                                                           \
    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Cosntant Type"))  \
)                                                                           \
.property("value", &vopv::node::Constant::value)                            \
(                                                                           \
    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Cosntant Value")) \
)                                                                           \
.property("name", &vopv::node::Constant::name)                              \
(                                                                           \
    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Cosntant Name"))  \
)                                                                           \
)
REGIST_NODE_RTTI_DEFAULT(Subnetwork, subnetwork)

// vector
REGIST_NODE_RTTI(GetVec2Comp, get_vec2_comp,
.property("comp_idx", &vopv::node::GetVec2Comp::comp_idx)              \
(                                                                      \
    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Component")) \
)                                                                      \
)
REGIST_NODE_RTTI(SetVec2Comp, set_vec2_comp,
.property("comp_idx", &vopv::node::SetVec2Comp::comp_idx)              \
(                                                                      \
    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Component")) \
)                                                                      \
)
REGIST_NODE_RTTI(GetVec3Comp, get_vec3_comp,
.property("comp_idx", &vopv::node::GetVec3Comp::comp_idx)              \
(                                                                      \
    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Component")) \
)                                                                      \
)
REGIST_NODE_RTTI(SetVec3Comp, set_vec3_comp,
.property("comp_idx", &vopv::node::SetVec3Comp::comp_idx)              \
(                                                                      \
    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Component")) \
)                                                                      \
)
REGIST_NODE_RTTI(GetVec4Comp, get_vec4_comp,
.property("comp_idx", &vopv::node::GetVec4Comp::comp_idx)              \
(                                                                      \
    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Component")) \
)                                                                      \
)
REGIST_NODE_RTTI(SetVec4Comp, set_vec4_comp,
.property("comp_idx", &vopv::node::SetVec4Comp::comp_idx)              \
(                                                                      \
    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Component")) \
)                                                                      \
)

}

namespace vopv
{

void nodes_regist_rttr()
{
}

}