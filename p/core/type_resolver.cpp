// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.core.type_resolver;

import p.core.block_symbol;
import p.core.symbol_table;
import p.core.type_symbol;
import p.core.alias_type_symbol;

namespace p {

class TypeResolver : public Visitor
{
public:
    TypeResolver(BlockSymbol* block_);
    TypeSymbol* GetType();
    void Visit(IntegerNode& node) override;
    void Visit(RealNode& node) override;
    void Visit(CharNode& node) override;
    void Visit(BooleanNode& node) override;
    void Visit(StringNode& node) override;
    void Visit(PointerNode& node) override;
    void Visit(IdentifierNode& node) override;
private:
    BlockSymbol* block;
    SymbolTable* symbolTable;
    TypeSymbol* type;
};

TypeResolver::TypeResolver(BlockSymbol* block_) : block(block_), symbolTable(block->GetSymbolTable()), type(nullptr)
{
}

TypeSymbol* TypeResolver::GetType() 
{
    while (type->IsAliasTypeSymbol())
    {
        AliasTypeSymbol* aliasType = static_cast<AliasTypeSymbol*>(type);
        type = aliasType->Type();
    }
    return type;
}

void TypeResolver::Visit(IntegerNode& node)
{
    type = symbolTable->GetType(GetIntegerTypeId(), &node);
}

void TypeResolver::Visit(RealNode& node)
{
    type = symbolTable->GetType(GetRealTypeId(), &node);
}

void TypeResolver::Visit(CharNode& node)
{
    type = symbolTable->GetType(GetCharTypeId(), &node);
}

void TypeResolver::Visit(BooleanNode& node)
{
    type = symbolTable->GetType(GetBooleanTypeId(), &node);
}

void TypeResolver::Visit(StringNode& node)
{
    type = symbolTable->GetType(GetStringTypeId(), &node);
}

void TypeResolver::Visit(PointerNode& node)
{
    type = symbolTable->GetType(GetPointerTypeId(), &node);
}

void TypeResolver::Visit(IdentifierNode& node)
{
    type = block->GetType(node.Str(), &node);
}

TypeSymbol* GetType(Node* node, BlockSymbol* block)
{
    TypeResolver resolver(block);
    node->Accept(resolver);
    return resolver.GetType();
}

} // namespace p
