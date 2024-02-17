// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.core.root_symbol;

import p.core.block_symbol;
import p.core.container_symbol;
import soul.ast.span;
import std.core;

export namespace p {

class SymbolTable;

class RootSymbol : public ContainerSymbol
{
public:
    RootSymbol(SymbolKind kind_, const soul::ast::Span& span_, const std::string& name_);
    RootSymbol(SymbolKind kind_, const soul::ast::Span& span_, const std::string& name_, const std::string& sourceFilePath_, const std::string& pcodeFilePath_);
    const std::string& SourceFilePath() const override;
    const std::string& PCodeFilePath() const { return pcodeFilePath; }
    BlockSymbol* Block() const { return rootBlock.get(); }
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void SetSymbolTable(SymbolTable* symbolTable_) { symbolTable = symbolTable_; }
    SymbolTable* GetSymbolTable() const override { return symbolTable; }
    void MakeVmts() override;
    void CheckDefined() override;
    void Print(util::CodeFormatter& formatter, bool full, ExecutionContext* context) override;
private:
    std::string sourceFilePath;
    std::string pcodeFilePath;
    std::unique_ptr<BlockSymbol> rootBlock;
    SymbolTable* symbolTable;
};

} // namespace p
