// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.core.context;

namespace p {

Context::Context() : unitLoader(nullptr), symbolTable(nullptr), currentConstructor(nullptr), node(nullptr), currentUnitPartKind(UnitPartKind::none), verbose(false)
{
}

void Context::PushNode(Node* node_)
{
    nodeStack.push(node);
    node = node_;
}

void Context::PopNode()
{
    node = nodeStack.top();
    nodeStack.pop();
}

} // namespace p
