// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.core.function_repository;

import p.ast;
import p.core.subroutine_symbol;
import std.core;

export namespace p {

class TypeSymbol;

class BinaryOperatorFunctionSymbol : public FunctionSymbol
{
public:
    BinaryOperatorFunctionSymbol(const soul::ast::Span& span_, Operator op_, TypeSymbol* type_, TypeSymbol* resultType_);
    Operator Op() const { return op; }
    TypeSymbol* Type() const { return type; }
    TypeSymbol* ResultType(const std::vector<TypeSymbol*>& argumentTypes, Node* node) const override { return resultType; }
    void GenerateCode(Emitter* emitter, const soul::ast::Span& span) override;
private:
    Operator op;
    TypeSymbol* type;
    TypeSymbol* resultType;
};

class UnaryOperatorFunctionSymbol : public FunctionSymbol
{
public:
    UnaryOperatorFunctionSymbol(const soul::ast::Span& span_, Operator op_, TypeSymbol* type_, TypeSymbol* resultType_);
    Operator Op() const { return op; }
    TypeSymbol* Type() const { return type; }
    TypeSymbol* ResultType(const std::vector<TypeSymbol*>& argumentTypes, Node* node) const override { return resultType; }
    void GenerateCode(Emitter* emitter, const soul::ast::Span& span) override;
private:
    Operator op;
    TypeSymbol* type;
    TypeSymbol* resultType;
};

class ConversionFunctionSymbol : public FunctionSymbol
{
public:
    ConversionFunctionSymbol(const soul::ast::Span& span_, TypeSymbol* targetType_, TypeSymbol* sourceType_);
    void GenerateCode(Emitter* emitter, const soul::ast::Span& span) override;
    TypeSymbol* ResultType(const std::vector<TypeSymbol*>& argumentTypes, Node* node) const override { return targetType; }
    TypeSymbol* TargetType() { return targetType; }
    TypeSymbol* SourceType() { return sourceType; }
private:
    TypeSymbol* targetType;
    TypeSymbol* sourceType;
};

FunctionSymbol* GetBinaryOperatorFunction(Operator op, TypeSymbol* type, Node* node);
FunctionSymbol* GetUnaryOperatorFunction(Operator op, TypeSymbol* type, Node* node);
FunctionSymbol* GetConversionFunction(TypeSymbol* targetType, TypeSymbol* sourceType, Node* node, bool throw_);

void MakeOperatorFunctions(SymbolTable* symbolTable, const std::string& filePath, const soul::ast::Span& span);

} // namespace p
