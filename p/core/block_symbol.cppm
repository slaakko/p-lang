// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.core.block_symbol;

import p.core.container_symbol;
import util;
import soul.ast.span;
import std.core;

export namespace p {

class UnitPartSymbol;

class BlockSymbol : public ContainerSymbol
{
public:
    BlockSymbol(const soul::ast::Span& span_, const std::string& name_);
    void AddSymbol(Symbol* symbol) override;
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    UnitPartSymbol* InterfacePart() const { return interfacePart; }
    UnitPartSymbol* ImplementationPart() const { return implementationPart; }
    UnitPartSymbol* InitializationPart() const { return  initializationPart; }
    int32_t Level() const { return level; }
    void SetLevel(int32_t level_) { level = level_; }
    void Print(util::CodeFormatter& formatter, bool full, ExecutionContext* context) override;
private:
    UnitPartSymbol* interfacePart;
    UnitPartSymbol* implementationPart;
    UnitPartSymbol* initializationPart;
    int32_t level;
};

} // namespace p
