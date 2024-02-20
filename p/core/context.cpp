// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.core.context;

namespace p {

Context::Context() : 
    unitLoader(nullptr), 
    symbolTable(nullptr), 
    currentConstructor(nullptr), 
    node(nullptr), 
    currentUnitPartKind(UnitPartKind::none), 
    flags(ContextFlags::none),
    specialization(nullptr)
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

void Context::PushFlags(ContextFlags flags_)
{
    flagStack.push(flags);
    flags = flags_;
}

void Context::PopFlags()
{
    flags = flagStack.top();
    flagStack.pop();
}

void Context::PushInstantiatingGeneric()
{
    PushFlags(flags);
    SetFlag(ContextFlags::instantiatingGeneric);
}

void Context::PopInstantiatingGeneric()
{
    PopFlags();
}

void Context::PushNoThrow()
{
    PushFlags(flags);
    SetFlag(ContextFlags::noThrow);
}

void Context::PopNoThrow()
{
    PopFlags();
}

void Context::PushSpecialization(SpecializationSymbol* specialization_)
{
    specializationStack.push(specialization);
    specialization = specialization_;
}

void Context::PopSpecialization()
{
    specialization = specializationStack.top();
    specializationStack.pop();
}

void Context::SetSpecializationName(const std::string& specializationName_)
{
    specializationName = specializationName_;
}

} // namespace p
