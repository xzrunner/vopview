#pragma once

#include <vop/typedef.h>

namespace vop { class Node; }
namespace bp { class Node; class Pin; }

namespace vopv
{

class Evaluator;

class VOPAdapter
{
public:
    static void UpdatePropBackFromFront(const bp::Node& front,
        vop::Node& back, const Evaluator& eval);

    static vop::NodePtr CreateBackFromFront(const bp::Node& front);

}; // SOP

}