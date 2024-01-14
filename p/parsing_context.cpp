// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.parsing_context;

import p.code;
import p.execute;

namespace p {

ParsingContext::ParsingContext(Context* parent) : 
    mod(new Module()), modulePart(nullptr), flags(Flags::none), currentValue(nullptr), emitter(nullptr), constructor(nullptr), node(nullptr)
{
    if (parent->UpdateUnits())
    {
        SetUpdateUnits();
    }
    AddUpdatedUnits(parent->UpdatedUnits());
}

void ParsingContext::PushNode(Node* node_)
{
    nodeStack.push(node);
    node = node_;
}

void ParsingContext::PopNode()
{
    node = nodeStack.top();
    nodeStack.pop();
}

void ParsingContext::SetModulePart(ModulePart* modulePart_)
{
    modulePart = modulePart_;
}

void ParsingContext::SetTypeName(const std::string& typeName_)
{
    typeName = typeName_;
}

void ParsingContext::SetConstantName(const std::string& constantName_)
{
    constantName = constantName_;
}

void ParsingContext::SetFieldName(const std::string& fieldName_)
{
    fieldName = fieldName_;
}

void ParsingContext::PushFlags(Flags flags_)
{
    flagStack.push(flags);
    flags = flags_;
}

void ParsingContext::PopFlags()
{
    flags = flagStack.top();
    flagStack.pop();
}

void ParsingContext::PushCurrentValue(Value* currentValue_)
{
    valueStack.push(currentValue);
    currentValue = currentValue_;
}

void ParsingContext::PopCurrentValue()
{
    currentValue = valueStack.top();
    valueStack.pop();
}

void ParsingContext::PushConstructor(Constructor* constructor_)
{
    constructorStack.push(constructor);
    constructor = constructor_;
}

void ParsingContext::PopConstructor()
{
    constructor = constructorStack.top();
    constructorStack.pop();
}

} // namespace p
