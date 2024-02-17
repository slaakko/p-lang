// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.core.type_symbol;

import p.core.container_symbol;
import util;
import soul.ast.span;
import std.core;

export namespace p {

class SymbolTable;
class ConstantSymbol;
class Value;

const util::uuid& GetBooleanTypeId();
const util::uuid& GetIntegerTypeId();
const util::uuid& GetCharTypeId();
const util::uuid& GetRealTypeId();
const util::uuid& GetStringTypeId();
const util::uuid& GetPointerTypeId();
const util::uuid& GetNilTypeId();

class TypeSymbol : public ContainerSymbol
{
public:
    TypeSymbol(SymbolKind kind_, const soul::ast::Span& span_, const std::string& name_);
    TypeSymbol(SymbolKind kind_, const soul::ast::Span& span_, const std::string& name_, const util::uuid& id_);
    const util::uuid& Id() const { return id; }
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    bool IsTypeSymbol() const override { return true; }
    virtual bool IsOrdinalTypeSymbol() const { return false; }
    void Print(util::CodeFormatter& formatter, bool full, ExecutionContext* context) override;
private:
    util::uuid id;
};

class OrdinalTypeSymbol : public TypeSymbol
{
public:
    OrdinalTypeSymbol(SymbolKind kind_, const soul::ast::Span& span_, const std::string& name_);
    OrdinalTypeSymbol(SymbolKind kind_, const soul::ast::Span& span_, const std::string& name_, const util::uuid& id_);
    bool IsOrdinalTypeSymbol() const override { return true; }
};

class BooleanTypeSymbol : public OrdinalTypeSymbol
{
public:
    BooleanTypeSymbol(const soul::ast::Span& span_, const std::string& name_);
    BooleanTypeSymbol(const soul::ast::Span& span_);
    static bool Not(bool operand) { return !operand; }
    static bool Equal(bool left, bool right) { return left == right; }
    static bool NotEqual(bool left, bool right) { return left != right; }
    static bool And(bool left, bool right) { return left && right; }
    static bool Or(bool left, bool right) { return left || right; }
    static bool Xor(bool left, bool right) { return left != right; }
};

class IntegerTypeSymbol : public OrdinalTypeSymbol
{
public:
    IntegerTypeSymbol(const soul::ast::Span& span_, const std::string& name_);
    IntegerTypeSymbol(const soul::ast::Span& span_);
    static bool Equal(int32_t left, int32_t right) { return left == right; }
    static bool NotEqual(int32_t left, int32_t right) { return left != right; }
    static bool Less(int32_t left, int32_t right) { return left < right; }
    static bool LessOrEqual(int32_t left, int32_t right) { return left <= right; }
    static bool Greater(int32_t left, int32_t right) { return left > right; }
    static bool GreaterOrEqual(int32_t left, int32_t right) { return left >= right; }
    static int32_t Not(int32_t operand) { return ~operand; }
    static int32_t UnaryPlus(int32_t operand) { return operand; }
    static int32_t UnaryMinus(int32_t operand) { return -operand; }
    static int32_t And(int32_t left, int32_t right) { return left & right; }
    static int32_t Or(int32_t left, int32_t right) { return left | right; }
    static int32_t Xor(int32_t left, int32_t right) { return left ^ right; }
    static int32_t Shl(int32_t left, int32_t right) { return left << right; }
    static int32_t Shr(int32_t left, int32_t right) { return left >> right; }
    static int32_t Plus(int32_t left, int32_t right) { return left + right; }
    static int32_t Minus(int32_t left, int32_t right) { return left - right; }
    static int32_t Multiply(int32_t left, int32_t right) { return left * right; }
    static float FractionalDivide(int32_t left, int32_t right) { return (1.0f * left) / right; }
    static int32_t Div(int32_t left, int32_t right) { return left / right; }
    static int32_t Mod(int32_t left, int32_t right) { return left % right; }
};

class CharTypeSymbol : public OrdinalTypeSymbol
{
public:
    CharTypeSymbol(const soul::ast::Span& span_, const std::string& name_);
    CharTypeSymbol(const soul::ast::Span& span_);
    static bool Equal(char left, char right) { return left == right; }
    static bool NotEqual(char left, char right) { return left != right; }
};

class EnumeratedTypeSymbol : public OrdinalTypeSymbol
{
public:
    EnumeratedTypeSymbol(const soul::ast::Span& span_, const std::string& name_);
    void AddEnumConstant(ConstantSymbol* enumConstant);
    const std::vector<ConstantSymbol*>& EnumConstants() const { return enumConstants; }
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void Resolve() override;
private:
    std::vector<ConstantSymbol*> enumConstants;
    std::vector<util::uuid> enumConstantIds;
};

class SubrangeTypeSymbol : public OrdinalTypeSymbol
{
public:
    SubrangeTypeSymbol(const soul::ast::Span& span_, const std::string& name_);
    SubrangeTypeSymbol(const soul::ast::Span& span_, const std::string& name_, Value* rangeStart_, Value* rangeEnd_);
    Value* RangeStart() const { return rangeStart.get(); }
    Value* RangeEnd() const { return rangeEnd.get(); }
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void Print(util::CodeFormatter& formatter, bool full, ExecutionContext* context) override;
private:
    std::unique_ptr<Value> rangeStart;
    std::unique_ptr<Value> rangeEnd;
};

class RealTypeSymbol : public TypeSymbol
{
public:
    RealTypeSymbol(const soul::ast::Span& span_, const std::string& name_);
    RealTypeSymbol(const soul::ast::Span& span_);
    static bool Equal(float left, float right) { return left == right; }
    static bool NotEqual(float left, float right) { return left != right; }
    static bool Less(float left, float right) { return left < right; }
    static bool LessOrEqual(float left, float right) { return left <= right; }
    static bool Greater(float left, float right) { return left > right; }
    static bool GreaterOrEqual(float left, float right) { return left >= right; }
    static float Plus(float left, float right) { return left + right; }
    static float Minus(float left, float right) { return left - right; }
    static float Multiply(float left, float right) { return left * right; }
    static float FractionalDivide(float left, float right) { return left / right; }
    static float UnaryPlus(float operand) { return operand; }
    static float UnaryMinus(float operand) { return -operand; }
};

class StringTypeSymbol : public TypeSymbol
{
public:
    StringTypeSymbol(const soul::ast::Span& span_, const std::string& name_);
    StringTypeSymbol(const soul::ast::Span& span_);
    static bool Equal(const std::string& left, const std::string& right) { return left == right; }
    static bool NotEqual(const std::string& left, const std::string& right) { return left != right; }
    static bool Less(const std::string& left, const std::string& right) { return left < right; }
    static bool LessOrEqual(const std::string& left, const std::string& right) { return left <= right; }
    static bool Greater(const std::string& left, const std::string& right) { return left > right; }
    static bool GreaterOrEqual(const std::string& left, const std::string& right) { return left >= right; }
    static std::string Plus(const std::string& left, const std::string& right) { return left + right; }
};

class PointerTypeSymbol : public TypeSymbol
{
public:
    PointerTypeSymbol(const soul::ast::Span& span_, const std::string& name_);
    PointerTypeSymbol(const soul::ast::Span& span_);
};

class NilTypeSymbol : public TypeSymbol
{
public:
    NilTypeSymbol(const soul::ast::Span& span_, const std::string& name_);
    NilTypeSymbol(const soul::ast::Span& span_);
};

TypeSymbol* CommonType(TypeSymbol* left, TypeSymbol* right);

void CreateFundamentalTypes(SymbolTable* symbolTable, const soul::ast::Span& span);

} // namespace p
