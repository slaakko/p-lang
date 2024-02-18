// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.core.context;

namespace p {

Context::Context() : unitLoader(nullptr), symbolTable(nullptr), currentConstructor(nullptr), node(nullptr), currentUnitPartKind(UnitPartKind::none), verbose(false), updateUnits(false)
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

void Context::AddUpdatedUnit(const std::string& updatedUnitName)
{
    updatedUnits.insert(updatedUnitName);
}

bool Context::UnitUpdated(const std::string& unitName) const
{
    return updatedUnits.find(unitName) != updatedUnits.end();
}

} // namespace p
