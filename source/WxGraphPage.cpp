#include "vopview/WxGraphPage.h"
#include "vopview/WxStageCanvas.h"
#include "vopview/Evaluator.h"

#include <ee0/SubjectMgr.h>
#include <ee0/WxStageCanvas.h>
#include <blueprint/MessageID.h>
#include <blueprint/CompNode.h>

#include <vop/Evaluator.h>
#include <node0/SceneNode.h>
#include <node0/CompComplex.h>

namespace
{

const uint32_t MESSAGES[] =
{
    ee0::MSG_SCENE_NODE_INSERT,
    ee0::MSG_SCENE_NODE_DELETE,
    ee0::MSG_SCENE_NODE_CLEAR,

    bp::MSG_BP_CONN_INSERT,
    bp::MSG_BP_CONN_DELETE,
    bp::MSG_BP_CONN_REBUILD,
    bp::MSG_BP_NODE_PROP_CHANGED,
};

}

namespace vopv
{

WxGraphPage::WxGraphPage(wxWindow* parent, const ee0::SubjectMgrPtr& sub_mgr)
    : ee0::WxStagePage(parent, sub_mgr)
{
    m_eval = std::make_shared<Evaluator>(nullptr);

    for (auto& msg : MESSAGES) {
        m_sub_mgr->RegisterObserver(msg, this);
    }
}

WxGraphPage::~WxGraphPage()
{
    for (auto& msg : MESSAGES) {
        m_sub_mgr->UnregisterObserver(msg, this);
    }
}

void WxGraphPage::OnNotify(uint32_t msg, const ee0::VariantSet& variants)
{
    ee0::WxStagePage::OnNotify(msg, variants);

	bool dirty = false;
	switch (msg)
	{
	case ee0::MSG_SCENE_NODE_INSERT:
		dirty = InsertSceneObj(variants);
		break;
	case ee0::MSG_SCENE_NODE_DELETE:
		dirty = DeleteSceneObj(variants);
		break;
	case ee0::MSG_SCENE_NODE_CLEAR:
        dirty = ClearAllSceneObjs();
		break;

    case bp::MSG_BP_CONN_INSERT:
        dirty = AfterInsertNodeConn(variants);
        break;
    case bp::MSG_BP_CONN_DELETE:
        dirty = BeforeDeleteNodeConn(variants);
        break;
    case bp::MSG_BP_CONN_REBUILD:
        dirty = m_eval->OnRebuildConnection();
        break;
	}

	if (dirty)
    {
		m_sub_mgr->NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
        m_preview_canvas->SetDirty();
	}
}

void WxGraphPage::Traverse(std::function<bool(const ee0::GameObj&)> func,
                           const ee0::VariantSet& variants , bool inverse) const
{
    m_root->GetSharedComp<n0::CompComplex>().Traverse(func, inverse);
}

void WxGraphPage::SetPreviewCanvas(const std::shared_ptr<ee0::WxStageCanvas>& canvas)
{
    m_preview_canvas = canvas;

    //if (m_preview_canvas)
    //{
    //    auto canvas = std::static_pointer_cast<WxStageCanvas>(m_preview_canvas);
    //    canvas->SetGraphStage(this);
    //}
}

void WxGraphPage::SetRootNode(const ee0::GameObj& root, const std::shared_ptr<vop::Evaluator>& eval)
{
    m_root = root;

    eval->ClearAllNodes();
    m_eval = std::make_shared<Evaluator>(eval);

    if (root && root->HasSharedComp<n0::CompComplex>())
    {
        auto& ccomplex = root->GetSharedComp<n0::CompComplex>();
        for (auto& c : ccomplex.GetAllChildren())
        {
            if (c->HasUniqueComp<bp::CompNode>()) {
                auto& cnode = c->GetUniqueComp<bp::CompNode>();
                auto bp_node = cnode.GetNode();
                if (bp_node) {
                    m_eval->OnAddNode(*bp_node);
                }
            }
        }
    }
    m_eval->OnRebuildConnection();
}

bool WxGraphPage::ClearAllSceneObjs()
{
    // update scene node
    assert(m_root->HasSharedComp<n0::CompComplex>());
    auto& ccomplex = m_root->GetSharedComp<n0::CompComplex>();
    bool dirty = !ccomplex.GetAllChildren().empty();
    ccomplex.RemoveAllChildren();

    // front eval cb
    assert(m_eval);
    m_eval->OnClearAllNodes();

    return true;
}

bool WxGraphPage::InsertSceneObj(const ee0::VariantSet& variants)
{
    auto var = variants.GetVariant("obj");
    GD_ASSERT(var.m_type == ee0::VT_PVOID, "no var in vars: obj");
    const ee0::GameObj* obj = static_cast<const ee0::GameObj*>(var.m_val.pv);
    GD_ASSERT(obj, "err scene obj");

    // update scene node
    assert(m_root->HasSharedComp<n0::CompComplex>());
    auto& ccomplex = m_root->GetSharedComp<n0::CompComplex>();
    ccomplex.AddChild(*obj);

    if ((*obj)->HasUniqueComp<bp::CompNode>())
    {
        auto& bp_node = (*obj)->GetUniqueComp<bp::CompNode>().GetNode();

        // front eval cb
        m_eval->OnAddNode(*bp_node);

        //// update flags
        //auto type = bp_node->get_type();
        //if (type.is_derived_from<Node>()) {
        //    auto sopv_node = std::static_pointer_cast<Node>(bp_node);
        //    if (m_enable_set_node_display) {
        //        sopv_node->SetDisplay(true);
        //    }
        //}
    }

    return true;
}

bool WxGraphPage::DeleteSceneObj(const ee0::VariantSet& variants)
{
	auto var = variants.GetVariant("obj");
	GD_ASSERT(var.m_type == ee0::VT_PVOID, "no var in vars: obj");
    const ee0::GameObj* obj = static_cast<const ee0::GameObj*>(var.m_val.pv);
	GD_ASSERT(obj, "err scene obj");

    // update scene (*obj)
    assert(m_root->HasSharedComp<n0::CompComplex>());
    auto& ccomplex = m_root->GetSharedComp<n0::CompComplex>();
    bool dirty = ccomplex.RemoveChild(*obj);

    if (dirty && (*obj)->HasUniqueComp<bp::CompNode>()) {
        auto& bp_node = (*obj)->GetUniqueComp<bp::CompNode>().GetNode();
        m_eval->OnRemoveNode(*bp_node);
    }

    return dirty;
}

bool WxGraphPage::AfterInsertNodeConn(const ee0::VariantSet& variants)
{
    auto var = variants.GetVariant("conn");
    GD_ASSERT(var.m_type == ee0::VT_PVOID, "no var in vars: conn");
    const std::shared_ptr<bp::Connecting>* conn = static_cast<const std::shared_ptr<bp::Connecting>*>(var.m_val.pv);
    GD_ASSERT(conn, "err conn");

    return m_eval->OnConnected(**conn);
}

bool WxGraphPage::BeforeDeleteNodeConn(const ee0::VariantSet& variants)
{
    auto var = variants.GetVariant("conn");
    GD_ASSERT(var.m_type == ee0::VT_PVOID, "no var in vars: conn");
    const std::shared_ptr<bp::Connecting>* conn = static_cast<const std::shared_ptr<bp::Connecting>*>(var.m_val.pv);
    GD_ASSERT(conn, "err conn");

    return m_eval->OnDisconnecting(**conn);
}

}
