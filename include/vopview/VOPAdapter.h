#pragma once

#include <blueprint/typedef.h>

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

    static std::shared_ptr<vop::Evaluator>
        CreateBackEval(const std::vector<bp::NodePtr>& nodes);

}; // SOP

}