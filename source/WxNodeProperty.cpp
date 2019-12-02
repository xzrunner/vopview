#include "vopview/WxNodeProperty.h"

#include <ee0/ReflectPropTypes.h>

#include <wx/propgrid/propgrid.h>

namespace vopv
{

WxNodeProperty::WxNodeProperty(wxWindow* parent, const ee0::SubjectMgrPtr& sub_mgr)
	: bp::WxNodeProperty(parent, sub_mgr)
{
}

bool WxNodeProperty::InitView(const rttr::property& prop, const bp::NodePtr& node)
{
    bool ret = true;

    auto ui_info_obj = prop.get_metadata(ee0::UIMetaInfoTag());
    if (!ui_info_obj.is_valid()) {
        return false;
    }

    auto ui_info = ui_info_obj.get_value<ee0::UIMetaInfo>();

    auto prop_type = prop.get_type();
    if (prop_type.is_enumeration())
    {
        auto wx_prop = CreateEnumProp(ui_info.desc, prop_type, prop.get_value(node).get_value<int>());
        m_pg->Append(wx_prop);
    }
    else
    {
        ret = false;
    }

    return ret;
}

bool WxNodeProperty::UpdateView(const rttr::property& prop, const wxPGProperty& wx_prop)
{
    bool ret = true;

    auto ui_info_obj = prop.get_metadata(ee0::UIMetaInfoTag());
    if (!ui_info_obj.is_valid()) {
        return false;
    }

    auto key  = wx_prop.GetName();
    wxAny val = wx_prop.GetValue();

    auto ui_info = ui_info_obj.get_value<ee0::UIMetaInfo>();

    auto prop_type = prop.get_type();
    if (prop_type.is_enumeration() && key == ui_info.desc)
    {
        if (val.CheckType<int>())
        {
            auto t = val.GetType();
            auto idx = wxANY_AS(val, int);
            auto vars = prop_type.get_enumeration().get_values();
            assert(idx >= 0 && idx < static_cast<int>(vars.size()));
            bool find = false;
            for (auto& var : vars) {
                if (var.to_int() == idx) {
                    prop.set_value(m_node, var);
                    find = true;
                    break;
                }
            }
            assert(find);
        }
    }
    else
    {
        ret = false;
    }

    return ret;
}

bool WxNodeProperty::UpdateView(wxPropertyGridEvent& event)
{
    return false;
}

}