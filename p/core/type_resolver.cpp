// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.core.type_resolver;

import p.core.block_symbol;
import p.core.symbol_table;
import p.core.type_symbol;
import p.core.alias_type_symbol;
import p.core.array_type_symbol;
import p.core.object_type_symbol;
import p.core.specialization_symbol;
import p.core.context;
import util;

namespace p {

class TypeResolver : public Visitor
{
public:
    TypeResolver(ContainerSymbol* container_, Context* context_);
    TypeSymbol* GetType();
    void Visit(IntegerNode& node) override;
    void Visit(RealNode& node) override;
    void Visit(CharNode& node) override;
    void Visit(BooleanNode& node) override;
    void Visit(StringNode& node) override;
    void Visit(PointerNode& node) override;
    void Visit(ArrayTypeNode& node) override;
    void Visit(SpecializationNode& node) override;
    void Visit(IdentifierNode& node) override;
private:
    ContainerSymbol* container;
    SymbolTable* symbolTable;
    TypeSymbol* type;
    Context* context;
};

TypeResolver::TypeResolver(ContainerSymbol* container_, Context* context_) : container(container_), symbolTable(container->GetSymbolTable()), type(nullptr), context(context_)
{
}

TypeSymbol* TypeResolver::GetType() 
{
    while (type && type->IsAliasTypeSymbol())
    {
        AliasTypeSymbol* aliasType = static_cast<AliasTypeSymbol*>(type);
        type = aliasType->Type();
    }
    return type;
}

void TypeResolver::Visit(IntegerNode& node)
{
    type = symbolTable->GetType(GetIntegerTypeId(), &node, context);
}

void TypeResolver::Visit(RealNode& node)
{
    type = symbolTable->GetType(GetRealTypeId(), &node, context);
}

void TypeResolver::Visit(CharNode& node)
{
    type = symbolTable->GetType(GetCharTypeId(), &node, context);
}

void TypeResolver::Visit(BooleanNode& node)
{
    type = symbolTable->GetType(GetBooleanTypeId(), &node, context);
}

void TypeResolver::Visit(StringNode& node)
{
    type = symbolTable->GetType(GetStringTypeId(), &node, context);
}

void TypeResolver::Visit(PointerNode& node)
{
    type = symbolTable->GetType(GetPointerTypeId(), &node, context);
}

void TypeResolver::Visit(ArrayTypeNode& node)
{
    node.ElementType()->Accept(*this);
    TypeSymbol* elementType = type;
    if (elementType)
    {
        type = container->GetArrayType(elementType->Name());
        if (!type)
        {
            type = symbolTable->GetArrayTypeNoThrow(elementType->Id());
            if (!type)
            {
                std::string arrayTypeName = "@array_type_" + util::GetSha1MessageDigest(util::ToString(util::uuid::random()));
                ArrayTypeSymbol* arrayType = new ArrayTypeSymbol(node.Span(), arrayTypeName);
                arrayType->SetElementType(elementType);
                container->AddSymbol(arrayType);
                type = arrayType;
            }
        }
    }
    else
    {
        if (!context->NoThrow())
        {
            ThrowError("element type for array type not found", node.FilePath(), node.Span());
        }
        type = nullptr;
    }
}

void TypeResolver::Visit(SpecializationNode& node)
{
    node.GenericId()->Accept(*this);
    if (type && type->IsObjectTypeSymbol())
    {
        ObjectTypeSymbol* objectType = static_cast<ObjectTypeSymbol*>(type);
        if (objectType->IsGeneric())
        {
            node.Type()->Accept(*this);
            TypeSymbol* parameterType = type;
            if (parameterType)
            {
                SpecializationSymbol* specialization = symbolTable->GetSpecializationNoThrow(objectType->Id(), parameterType->Id());
                if (specialization)
                {
                    type = specialization;
                }
                else
                {
                    std::string specializationName = context->SpecializationName();
                    if (!specializationName.empty())
                    {
                        specializationName.append(1, '_');
                    }
                    std::string uniqueSpecializationName = "@specialization_" + specializationName + util::GetSha1MessageDigest(util::ToString(util::uuid::random()));
                    SpecializationSymbol* specialization = new SpecializationSymbol(node.Span(), uniqueSpecializationName, objectType, parameterType);
                    container->AddSymbol(specialization);
                    type = specialization;
                }
            }
        }
        else
        {
            if (!context->NoThrow())
            {
                ThrowError("generic object type expected", node.FilePath(), node.GenericId()->Span());
            }
            type = nullptr;
        }
    }
    else
    {
        if (!context->NoThrow())
        {
            ThrowError("generic object type expected", node.FilePath(), node.GenericId()->Span());
        }
        type = nullptr;
    }
}

void TypeResolver::Visit(IdentifierNode& node)
{
    type = container->GetType(node.Str(), &node, context);
    if (type && type->IsBoundTypeParamSymbol())
    {
        BoundTypeParamSymbol* boundTypeParam = static_cast<BoundTypeParamSymbol*>(type);
        type = boundTypeParam->TypeArgument();
    }
}

TypeSymbol* GetType(Node* node, ContainerSymbol* container, Context* context)
{
    TypeResolver resolver(container, context);
    node->Accept(resolver);
    return resolver.GetType();
}

} // namespace p
