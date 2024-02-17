// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.core.constant_symbol;

import p.core.symbol;
import util;
import std.core;

export namespace p {

class Value;

class ConstantSymbol : public Symbol
{
public:
    ConstantSymbol(const soul::ast::Span& span_, const std::string& name_);
    const util::uuid& Id() const { return id; }
    TypeSymbol* Type() const { return type; }
    void SetType(TypeSymbol* type_) { type = type_; }
    void SetValue(Value* value_);
    Value* GetValue() const { return value.get(); }
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void Resolve() override;
    void Print(util::CodeFormatter& formatter, bool full, ExecutionContext* context) override;
private:
    util::uuid id;
    TypeSymbol* type;
    util::uuid typeId;
    std::unique_ptr<Value> value;
};

} // namespace p
