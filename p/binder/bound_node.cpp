// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.binder.bound_node;

namespace p {

BoundNode::BoundNode(BoundNodeKind kind_, const soul::ast::Span& span_) : kind(kind_), span(span_), parent(nullptr)
{
}

BoundNode::~BoundNode()
{
}

const std::string& BoundNode::FilePath() const
{
    if (parent)
    {
        return parent->FilePath();
    }
    else
    {
        static std::string empty;
        return empty;
    }
}

} // namespace p
