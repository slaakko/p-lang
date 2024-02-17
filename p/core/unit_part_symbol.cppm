// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.core.unit_part_symbol;

import p.ast;
import util;
import p.core.container_symbol;
import p.core.block_symbol;
import soul.ast.span;
import std.core;

export namespace p {

class UnitPartSymbol : public ContainerSymbol
{
public:
    UnitPartSymbol(const soul::ast::Span& span_, const std::string& name_);
    UnitPartSymbol(const soul::ast::Span& span_, const std::string& name_, UnitPartKind unitPartKind_);
    UnitPartKind PartKind() const { return unitPartKind; }
    BlockSymbol* Block() const { return block.get(); }
    void AddSymbol(Symbol* symbol) override;
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void SetImplementationBlock(BlockSymbol* implementationBlock_);
    Symbol* GetSymbol(const std::string& symbolName, Node* node, bool searchBase, bool mustExist) const override;
    void Print(util::CodeFormatter& formatter, bool full, ExecutionContext* context) override;
    void MakeVmts() override;
    void CheckDefined() override;
private:
    UnitPartKind unitPartKind;
    std::unique_ptr<BlockSymbol> block;
    BlockSymbol* implementationBlock;
};

} // namespace p
