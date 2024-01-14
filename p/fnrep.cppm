// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.fnrep;

import p.ast;
import soul.lexer;
import p.subroutine;
import std.core;

export namespace p {

class BinaryOperatorFunction : public Function
{
public:
    BinaryOperatorFunction(Operator op_, Type* type_, Type* resultType_);
    void GenerateCode(Emitter* emitter, int64_t pos) override;
    Operator Op() const { return op; }
    Type* GetType() const { return type; }
    Type* GetResultType() const { return resultType; }
private:
    Operator op;
    Type* type;
    Type* resultType;
};

class UnaryOperatorFunction : public Function
{
public:
    UnaryOperatorFunction(Operator op_, Type* type_, Type* resultType_);
    void GenerateCode(Emitter* emitter, int64_t pos) override;
    Operator Op() const { return op; }
    Type* GetType() const { return type; }
    Type* GetResultType() const { return resultType; }
private:
    Operator op;
    Type* type;
    Type* resultType;
};

class ConversionFunction : public Function
{
public:
    ConversionFunction(Type* targetType_, Type* sourceType_);
    void GenerateCode(Emitter* emitter, int64_t pos) override;
    Type* TargetType() { return targetType; }
    Type* SourceType() { return sourceType; }
private:
    Type* targetType;
    Type* sourceType;
};

Function* GetBinaryOperatorFunction(Operator op, Type* type, soul::lexer::LexerBase<char>& lexer, int64_t pos);
Function* GetUnaryOperatorFunction(Operator op, Type* type, soul::lexer::LexerBase<char>& lexer, int64_t pos);
Function* GetConversionFunction(Type* targetType, Type* sourceType, soul::lexer::LexerBase<char>& lexer, int64_t pos, bool throw_);
void MakeOperatorFunctions(Block* block);

} // namespace p
