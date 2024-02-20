// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.core.specialization_symbol;

import p.core.type_symbol;
import p.core.object_type_symbol;
import soul.ast.span;
import std.core;

export namespace p {

class BoundTypeParamSymbol : public TypeSymbol
{
public:
    BoundTypeParamSymbol(const soul::ast::Span& span_, const std::string& name_);
    BoundTypeParamSymbol(const soul::ast::Span& span_, const std::string& name_, TypeSymbol* typeArgument_);
    TypeSymbol* TypeArgument() const { return typeArgument; }
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void Resolve() override;
    void Print(util::CodeFormatter& formatter, bool full, ExecutionContext* context) override;
private:
    TypeSymbol* typeArgument;
    util::uuid typeArgumentId;
};

class SpecializationSymbol : public ObjectTypeSymbol
{
public:
    SpecializationSymbol(const soul::ast::Span& span_, const std::string& name_);
    SpecializationSymbol(const soul::ast::Span& span_, const std::string& name_, ObjectTypeSymbol* genericType_, TypeSymbol* typeArgumentSymbol_);
    ObjectTypeSymbol* GenericType() const { return genericType; }
    TypeSymbol* TypeArgument() const { return typeArgumentSymbol; }
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void Resolve() override;
    bool Instantiated() const { return instantiated; }
    void SetInstantiated() { instantiated = true; }
    void Print(util::CodeFormatter& formatter, bool full, ExecutionContext* context) override;
    bool Printed() const { return printed; }
    void ResetPrinted() { printed = false; }
private:
    ObjectTypeSymbol* genericType;
    TypeSymbol* typeArgumentSymbol;
    util::uuid genericTypeId;
    util::uuid typeArgumentId;
    bool instantiated;
    bool printed;
};

} // namespace p
