// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.context;

import p.instruction;
import p.subroutine;

namespace p {

Context::Context() : moduleMap(nullptr), block(nullptr), updatedUnits(), updateUnits(false), verbose(false)
{
}

void Context::PushBlock(Block* block_)
{
    blockStack.push(block);
    block = block_;
}

void Context::PopBlock()
{
    block = blockStack.top();
    blockStack.pop();
}

void Context::Reset()
{
    block = nullptr;
    blockStack = std::stack<Block*>();
}

void Context::AddUpdatedUnit(const std::string& updatedUnitName)
{
    updatedUnits.insert(updatedUnitName);
}

bool Context::UnitUpdated(const std::string& unitName) const
{
    return updatedUnits.find(unitName) != updatedUnits.end();
}

void Context::AddUpdatedUnits(const std::set<std::string>& updatedUnits_)
{
    for (const auto& updateUnit : updatedUnits_)
    {
        updatedUnits.insert(updateUnit);
    }
}

} // namespace p
