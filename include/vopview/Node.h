#pragma once

#include <blueprint/Node.h>

namespace vopv
{

class Node : public bp::Node
{
public:
    Node(const std::string& title, bool props = false);
    Node(const Node& node);
    Node& operator = (const Node& node);
    virtual ~Node();

    virtual void Draw(const n2::RenderParams& rp) const override;

    auto& GetName() const { return m_name; }
    void  SetName(const std::string& name) { m_name = name; }

protected:
    struct PinDesc
    {
        bool operator == (const PinDesc& desc) const {
            return type == desc.type && name == desc.name;
        }

        int         type;
        std::string name;
    };

protected:
    void InitPins(const std::vector<PinDesc>& input,
        const std::vector<PinDesc>& output);
    void InitPins(const std::string& name);

private:
    void InitPinsImpl(const std::vector<PinDesc>& pins,
        bool is_input);

private:
    std::string m_name;

    bool m_bypass = false;

    RTTR_ENABLE(bp::Node)

}; // Node

}