// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.core.array_type_symbol;

import p.core.type_symbol;
import util;
import soul.ast.span;
import std.core;

export namespace p {

class ArrayTypeSymbol : public TypeSymbol
{
public:
    ArrayTypeSymbol(const soul::ast::Span& span_, const std::string& name_);
    TypeSymbol* ElementType() const { return elementType; }
    void SetElementType(TypeSymbol* elementType_) { elementType = elementType_; }
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void Resolve() override;
    void Print(util::CodeFormatter& formatter, bool full, ExecutionContext* context) override;
private:
    TypeSymbol* elementType;
    util::uuid elementTypeId;
};

} // namespace p
