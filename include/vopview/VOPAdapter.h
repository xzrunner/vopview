#pragma once

#include <blueprint/typedef.h>

#include <dag/VarType.h>

#include <memory>
#include <vector>

namespace vop { class Node; class Evaluator; }
namespace bp { class Node; class Pin; }

namespace vopv
{

class Evaluator;
class Node;

class VOPAdapter
{
public:
    static void UpdatePropBackFromFront(const bp::Node& front,
        vop::Node& back, const Evaluator& eval);

    static std::shared_ptr<vop::Node>
        CreateBackFromFront(const bp::Node& front);
    static std::shared_ptr<Node>
        CreateFrontFromBack(const vop::Node& back);

    static void UpdateBackEval(const std::shared_ptr<vop::Evaluator>& dst_eval,
        const std::vector<bp::NodePtr>& src_nodes);

    static int TypeBackToFront(dag::VarType type);

}; // SOP

}