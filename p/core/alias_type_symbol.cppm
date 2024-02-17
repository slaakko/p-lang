// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.core.alias_type_symbol;

import p.core.type_symbol;
import util;
import soul.ast.span;
import std.core;

export namespace p {

class AliasTypeSymbol : public TypeSymbol
{
public:
    AliasTypeSymbol(const soul::ast::Span& span_, const std::string& name_);
    AliasTypeSymbol(const soul::ast::Span& span_, const std::string& name_, TypeSymbol* type_);
    TypeSymbol* Type() const { return type; }
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void Resolve() override;
    void Print(util::CodeFormatter& formatter, bool full, ExecutionContext* context) override;
private:
    TypeSymbol* type;
    util::uuid typeId;
};

} // namespace p
