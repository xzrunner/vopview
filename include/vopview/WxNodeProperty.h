#pragma once

#include <blueprint/WxNodeProperty.h>

namespace vopv
{

class WxNodeProperty : public bp::WxNodeProperty
{
public:
	WxNodeProperty(wxWindow* parent, const ee0::SubjectMgrPtr& sub_mgr);

private:
    virtual bool InitView(const rttr::property& prop, const bp::NodePtr& node) override;
    virtual bool UpdateView(const rttr::property& prop, const wxPGProperty& wx_prop) override;
    virtual bool UpdateView(wxPropertyGridEvent& event) override;

}; // WxNodeProperty

}