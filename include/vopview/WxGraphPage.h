#pragma once

#include <ee0/WxStagePage.h>

namespace vop { class Evaluator; }

namespace vopv
{

class Evaluator;

class WxGraphPage : public ee0::WxStagePage
{
public:
	WxGraphPage(wxWindow* parent, const ee0::SubjectMgrPtr& sub_mgr);
    virtual ~WxGraphPage();

    virtual void OnNotify(uint32_t msg, const ee0::VariantSet& variants) override;

    virtual void Traverse(std::function<bool(const ee0::GameObj&)> func,
        const ee0::VariantSet& variants = ee0::VariantSet(), bool inverse = false) const override;

    void SetPreviewCanvas(const std::shared_ptr<ee0::WxStageCanvas>& canvas);

    auto GetEval() const { return m_eval; }

    auto GetRootNode() const { return m_root; }
    void SetRootNode(const ee0::GameObj& root,
        const std::shared_ptr<vop::Evaluator>& eval);

private:
    bool ClearAllSceneObjs();
    bool InsertSceneObj(const ee0::VariantSet& variants);
    bool DeleteSceneObj(const ee0::VariantSet& variants);

    bool AfterInsertNodeConn(const ee0::VariantSet& variants);
    bool BeforeDeleteNodeConn(const ee0::VariantSet& variants);

private:
    n0::SceneNodePtr m_root = nullptr;

    std::shared_ptr<ee0::WxStageCanvas> m_preview_canvas = nullptr;

    std::shared_ptr<Evaluator> m_eval = nullptr;

}; // WxGraphPage

}