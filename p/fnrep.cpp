// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.fnrep;

import p.type;
import p.code;
import p.block;

namespace p {

std::string MakeOpName(Operator op)
{
    switch (op)
    {
        case Operator::equal:
        {
            return "equal";
        }
        case Operator::notEqual:
        {
            return "not.equal";
        }
        case Operator::less:
        {
            return "less";
        }
        case Operator::lessOrEqual:
        {
            return "less.equal";
        }
        case Operator::greater:
        {
            return "greater";
        }
        case Operator::greaterOrEqual:
        {
            return "greater.equal";
        }
        case Operator::plus:
        {
            return "plus";
        }
        case Operator::minus:
        {
            return "minus";
        }
        case Operator::or_:
        {
            return "or";
        }
        case Operator::xor_:
        {
            return "xor";
        }
        case Operator::mul:
        {
            return "mul";
        }
        case Operator::fractionalDivide:
        {
            return "fract.divide";
        }
        case Operator::div:
        {
            return "div";
        }
        case Operator::mod:
        {
            return "mod";
        }
        case Operator::and_:
        {
            return "and";
        }
        case Operator::shl:
        {
            return "shl";
        }
        case Operator::shr:
        {
            return "shr";
        }
        case Operator::not_:
        {
            return "not";
        }
    }
    return std::string();
}

BinaryOperatorFunction::BinaryOperatorFunction(Operator op_, Type* type_, Type* resultType_) :
    Function(FunctionKind::operatorfn, "@binary.operation." + MakeOpName(op_) + "." + type_->Name()), op(op_), type(type_), resultType(resultType_)
{
    GetFunctionHeading()->SetResultType(resultType);
}

// equal, notEqual, less, lessOrEqual, greater, greaterOrEqual, plus, minus, or_, xor_, mul, fractionalDivide, div, mod, and_, shl, shr, not_,

void BinaryOperatorFunction::GenerateCode(Emitter* emitter, int64_t pos)
{
    switch (type->Kind())
    {
        case TypeKind::booleanType:
        {
            switch (op)
            {
                case Operator::equal:
                {
                    emitter->Emit(new EqualBoolInstruction());
                    break;
                }
                case Operator::notEqual:
                {
                    emitter->Emit(new NotEqualBoolInstruction());
                    break;
                }
                case Operator::and_:
                {
                    emitter->Emit(new AndBoolInstruction());
                    break;
                }
                case Operator::or_:
                {
                    emitter->Emit(new OrBoolInstruction());
                    break;
                }
                case Operator::xor_:
                {
                    emitter->Emit(new XorBoolInstruction());
                    break;
                }
                default:
                {
                    ThrowError("error: invalid Boolean binary operation", emitter->Lexer(), pos);
                    break;
                }
            }
            break;
        }
        case TypeKind::integerType:
        {
            switch (op)
            {
                case Operator::equal:
                {
                    emitter->Emit(new EqualIntInstruction());
                    break;
                }
                case Operator::notEqual:
                {
                    emitter->Emit(new NotEqualIntInstruction());
                    break;
                }
                case Operator::less:
                {
                    emitter->Emit(new LessIntInstruction());
                    break;
                }
                case Operator::greater:
                {
                    emitter->Emit(new GreaterIntInstruction());
                    break;
                }
                case Operator::lessOrEqual:
                {
                    emitter->Emit(new LessOrEqualIntInstruction());
                    break;
                }
                case Operator::greaterOrEqual:
                {
                    emitter->Emit(new GreaterOrEqualIntInstruction());
                    break;
                }
                case Operator::plus:
                {
                    emitter->Emit(new PlusIntInstruction());
                    break;
                }
                case Operator::minus:
                {
                    emitter->Emit(new MinusIntInstruction());
                    break;
                }
                case Operator::mul:
                {
                    emitter->Emit(new MultiplyIntInstruction());
                    break;
                }
                case Operator::fractionalDivide:
                {
                    emitter->Emit(new FractionalDivideIntInstruction());
                    break;
                }
                case Operator::div:
                {
                    emitter->Emit(new DivIntInstruction());
                    break;
                }
                case Operator::mod:
                {
                    emitter->Emit(new ModIntInstruction());
                    break;
                }
                case Operator::and_:
                {
                    emitter->Emit(new AndIntInstruction());
                    break;
                }
                case Operator::shl:
                {
                    emitter->Emit(new ShlIntInstruction());
                    break;
                }
                case Operator::shr:
                {
                    emitter->Emit(new ShrIntInstruction());
                    break;
                }
                default:
                {
                    ThrowError("error: invalid integer binary operation", emitter->Lexer(), pos);
                    break;
                }
            }
            break;
        }
        case TypeKind::realType:
        {
            switch (op)
            {
                case Operator::equal:
                {
                    emitter->Emit(new EqualRealInstruction());
                    break;
                }
                case Operator::notEqual:
                {
                    emitter->Emit(new NotEqualRealInstruction());
                    break;
                }
                case Operator::less:
                {
                    emitter->Emit(new LessRealInstruction());
                    break;
                }
                case Operator::greater:
                {
                    emitter->Emit(new GreaterRealInstruction());
                    break;
                }
                case Operator::lessOrEqual:
                {
                    emitter->Emit(new LessOrEqualRealInstruction());
                    break;
                }
                case Operator::greaterOrEqual:
                {
                    emitter->Emit(new GreaterOrEqualRealInstruction());
                    break;
                }
                case Operator::plus:
                {
                    emitter->Emit(new PlusRealInstruction());
                    break;
                }
                case Operator::minus:
                {
                    emitter->Emit(new MinusRealInstruction());
                    break;
                }
                case Operator::mul:
                {
                    emitter->Emit(new MultiplyRealInstruction());
                    break;
                }
                case Operator::fractionalDivide:
                {
                    emitter->Emit(new FractionalDivideRealInstruction());
                    break;
                }
                default:
                {
                    ThrowError("error: invalid real binary operation", emitter->Lexer(), pos);
                    break;
                }
            }
            break;
        }
        case TypeKind::charType:
        {
            switch (op)
            {
                case Operator::equal:
                {
                    emitter->Emit(new EqualCharInstruction());
                    break;
                }
                case Operator::notEqual:
                {
                    emitter->Emit(new NotEqualCharInstruction());
                    break;
                }
                default:
                {
                    ThrowError("error: invalid character binary operation", emitter->Lexer(), pos);
                    break;
                }
            }
            break;
        }
        case TypeKind::stringType:
        {
            switch (op)
            {
                case Operator::equal:
                {
                    emitter->Emit(new EqualStringInstruction());
                    break;
                }
                case Operator::notEqual:
                {
                    emitter->Emit(new NotEqualStringInstruction());
                    break;
                }
                case Operator::plus:
                {
                    emitter->Emit(new PlusStringInstruction());
                    break;
                }
                default:
                {
                    ThrowError("error: invalid string binary operation", emitter->Lexer(), pos);
                    break;
                }
            }
            break;
        }
        case TypeKind::nilType:
        {
            emitter->Emit(new EqualNilInstruction());
            break;
        }
        default:
        {
            ThrowError("error: invalid binary operation operand type", emitter->Lexer(), pos);
        }
    }
}

UnaryOperatorFunction::UnaryOperatorFunction(Operator op_, Type* type_, Type* resultType_) : 
    Function(FunctionKind::operatorfn, "@unary.operation." + MakeOpName(op_) + "." + type_->Name()), op(op_), type(type_), resultType(resultType_)
{
    GetFunctionHeading()->SetResultType(resultType);
}

void UnaryOperatorFunction::GenerateCode(Emitter* emitter, int64_t pos)
{
    switch (type->Kind())
    {
        case TypeKind::booleanType:
        {
            switch (op)
            {
                case Operator::not_:
                {
                    emitter->Emit(new NotBoolInstruction());
                    break;
                }
                default:
                {
                    ThrowError("error: invalid Boolean unary operation", emitter->Lexer(), pos);
                    break;
                }
            }
            break;
        }
        case TypeKind::integerType:
        {
            switch (op)
            {
                case Operator::not_:
                {
                    emitter->Emit(new NotIntInstruction());
                    break;
                }
                case Operator::plus:
                {
                    emitter->Emit(new UnaryPlusIntInstruction());
                    break;
                }
                case Operator::minus:
                {
                    emitter->Emit(new UnaryMinusIntInstruction());
                    break;
                }
                default:
                {
                    ThrowError("error: invalid integer unary operation", emitter->Lexer(), pos);
                    break;
                }
            }
            break;
        }
        case TypeKind::realType:
        {
            switch (op)
            {
                case Operator::plus:
                {
                    emitter->Emit(new UnaryPlusRealInstruction());
                    break;
                }
                case Operator::minus:
                {
                    emitter->Emit(new UnaryMinusRealInstruction());
                    break;
                }
                default:
                {
                    ThrowError("error: invalid real unary operation", emitter->Lexer(), pos);
                    break;
                }
            }
            break;
        }
        default:
        {
            ThrowError("error: invalid unary operation operand type", emitter->Lexer(), pos);
        }
    }
}

ConversionFunction::ConversionFunction(Type* targetType_, Type* sourceType_) : 
    Function(FunctionKind::conversionfn, "@conversion.function." + targetType_->Name() + "." + sourceType_->Name()),
    targetType(targetType_), sourceType(sourceType_)
{
    GetFunctionHeading()->SetResultType(targetType);
}
    
void ConversionFunction::GenerateCode(Emitter* emitter, int64_t pos)
{
    switch (targetType->Kind())
    {
        case TypeKind::realType:
        {
            if (sourceType->Kind() == TypeKind::integerType)
            {
                emitter->Emit(new IntToRealInstruction());
            }
            else
            {
                ThrowError("error: invalid type conversion", emitter->Lexer(), pos);
            }
            break;
        }
        case TypeKind::stringType:
        {
            if (sourceType->Kind() == TypeKind::charType)
            {
                emitter->Emit(new CharToStringInstruction());
            }
            else
            {
                ThrowError("error: invalid type conversion", emitter->Lexer(), pos);
            }
            break;
        }
        default:
        {
            ThrowError("error: invalid type conversion", emitter->Lexer(), pos);
            break;
        }

    }
}

class FunctionRepository
{
public:
    static FunctionRepository& Instance();
    void AddBinaryOperatorFunction(BinaryOperatorFunction* function);
    Function* GetBinaryOperatorFunction(Operator op, Type* type, soul::lexer::LexerBase<char>& lexer, int64_t pos) const;
    void AddUnaryOperatorFunction(UnaryOperatorFunction* function);
    Function* GetUnaryOperatorFunction(Operator op, Type* type, soul::lexer::LexerBase<char>& lexer, int64_t pos) const;
    void AddConversionFunction(ConversionFunction* function);
    Function* GetConversionFunction(Type* targetType, Type* sourceType, soul::lexer::LexerBase<char>& lexer, int64_t pos, bool throw_) const;
private:
    FunctionRepository();
    std::map<std::pair<Operator, Type*>, BinaryOperatorFunction*> binaryOperatorFunctionMap;
    std::map<std::pair<Operator, Type*>, UnaryOperatorFunction*> unaryOperatorFunctionMap;
    std::map<std::pair<Type*, Type*>, ConversionFunction*> conversionFunctionMap;
    std::vector<std::unique_ptr<Function>> functions;
};

FunctionRepository::FunctionRepository()
{
}

void FunctionRepository::AddBinaryOperatorFunction(BinaryOperatorFunction* function)
{
    binaryOperatorFunctionMap[std::make_pair(function->Op(), function->GetType())] = function;
    functions.push_back(std::unique_ptr<Function>(function));
}

Function* FunctionRepository::GetBinaryOperatorFunction(Operator op, Type* type, soul::lexer::LexerBase<char>& lexer, int64_t pos) const
{
    auto it = binaryOperatorFunctionMap.find(std::make_pair(op, type));
    if (it != binaryOperatorFunctionMap.end())
    {
        return it->second;
    }
    else
    {
        ThrowError("error: binary operator '" + MakeOpName(op)  + "' for type '" + type->Name() + " not found", lexer, pos);
    }
    return nullptr;
}

void FunctionRepository::AddUnaryOperatorFunction(UnaryOperatorFunction* function)
{
    unaryOperatorFunctionMap[std::make_pair(function->Op(), function->GetType())] = function;
    functions.push_back(std::unique_ptr<Function>(function));
}

Function* FunctionRepository::GetUnaryOperatorFunction(Operator op, Type* type, soul::lexer::LexerBase<char>& lexer, int64_t pos) const
{
    auto it = unaryOperatorFunctionMap.find(std::make_pair(op, type));
    if (it != unaryOperatorFunctionMap.end())
    {
        return it->second;
    }
    else
    {
        ThrowError("error: unary operator '" + MakeOpName(op) + "' for type '" + type->Name() + " not found", lexer, pos);
    }
    return nullptr;
}

void FunctionRepository::AddConversionFunction(ConversionFunction* function)
{
    conversionFunctionMap[std::make_pair(function->TargetType(), function->SourceType())] = function;
    functions.push_back(std::unique_ptr<Function>(function));
}

Function* FunctionRepository::GetConversionFunction(Type* targetType, Type* sourceType, soul::lexer::LexerBase<char>& lexer, int64_t pos, bool throw_) const
{
    auto it = conversionFunctionMap.find(std::make_pair(targetType, sourceType));
    if (it != conversionFunctionMap.end())
    {
        return it->second;
    }
    else
    {
        if (throw_)
        {
            ThrowError("error: conversion from '" + sourceType->Name() + "' to '" + targetType->Name() + "' not found", lexer, pos);
        }
    }
    return nullptr;
}

FunctionRepository& FunctionRepository::Instance()
{
    static FunctionRepository instance;
    return instance;
}

void MakeOperatorFunctions(Block* block)
{
    Type* booleanType = block->GetType("boolean");
    Type* integerType = block->GetType("integer");
    Type* realType = block->GetType("real");
    Type* charType = block->GetType("char");
    Type* stringType = block->GetType("string");
    Type* pointerType = block->GetType("pointer");
    Type* nilType = block->GetFundamentalType(TypeKind::nilType);
    FunctionRepository::Instance().AddBinaryOperatorFunction(new BinaryOperatorFunction(Operator::equal, booleanType, booleanType));
    FunctionRepository::Instance().AddBinaryOperatorFunction(new BinaryOperatorFunction(Operator::notEqual, booleanType, booleanType));
    FunctionRepository::Instance().AddBinaryOperatorFunction(new BinaryOperatorFunction(Operator::and_, booleanType, booleanType));
    FunctionRepository::Instance().AddBinaryOperatorFunction(new BinaryOperatorFunction(Operator::or_, booleanType, booleanType));
    FunctionRepository::Instance().AddBinaryOperatorFunction(new BinaryOperatorFunction(Operator::xor_, booleanType, booleanType));
    FunctionRepository::Instance().AddBinaryOperatorFunction(new BinaryOperatorFunction(Operator::equal, integerType, booleanType));
    FunctionRepository::Instance().AddBinaryOperatorFunction(new BinaryOperatorFunction(Operator::notEqual, integerType, booleanType));
    FunctionRepository::Instance().AddBinaryOperatorFunction(new BinaryOperatorFunction(Operator::less, integerType, booleanType));
    FunctionRepository::Instance().AddBinaryOperatorFunction(new BinaryOperatorFunction(Operator::greater, integerType, booleanType));
    FunctionRepository::Instance().AddBinaryOperatorFunction(new BinaryOperatorFunction(Operator::lessOrEqual, integerType, booleanType));
    FunctionRepository::Instance().AddBinaryOperatorFunction(new BinaryOperatorFunction(Operator::greaterOrEqual, integerType, booleanType));
    FunctionRepository::Instance().AddBinaryOperatorFunction(new BinaryOperatorFunction(Operator::plus, integerType, integerType));
    FunctionRepository::Instance().AddBinaryOperatorFunction(new BinaryOperatorFunction(Operator::minus, integerType, integerType));
    FunctionRepository::Instance().AddBinaryOperatorFunction(new BinaryOperatorFunction(Operator::mul, integerType, integerType));
    FunctionRepository::Instance().AddBinaryOperatorFunction(new BinaryOperatorFunction(Operator::fractionalDivide, integerType, realType));
    FunctionRepository::Instance().AddBinaryOperatorFunction(new BinaryOperatorFunction(Operator::div, integerType, integerType));
    FunctionRepository::Instance().AddBinaryOperatorFunction(new BinaryOperatorFunction(Operator::mod, integerType, integerType));
    FunctionRepository::Instance().AddBinaryOperatorFunction(new BinaryOperatorFunction(Operator::and_, integerType, integerType));
    FunctionRepository::Instance().AddBinaryOperatorFunction(new BinaryOperatorFunction(Operator::or_, integerType, integerType));
    FunctionRepository::Instance().AddBinaryOperatorFunction(new BinaryOperatorFunction(Operator::xor_, integerType, integerType));
    FunctionRepository::Instance().AddBinaryOperatorFunction(new BinaryOperatorFunction(Operator::shl, integerType, integerType));
    FunctionRepository::Instance().AddBinaryOperatorFunction(new BinaryOperatorFunction(Operator::shr, integerType, integerType));
    FunctionRepository::Instance().AddBinaryOperatorFunction(new BinaryOperatorFunction(Operator::equal, realType, booleanType));
    FunctionRepository::Instance().AddBinaryOperatorFunction(new BinaryOperatorFunction(Operator::notEqual, realType, booleanType));
    FunctionRepository::Instance().AddBinaryOperatorFunction(new BinaryOperatorFunction(Operator::less, realType, booleanType));
    FunctionRepository::Instance().AddBinaryOperatorFunction(new BinaryOperatorFunction(Operator::greater, realType, booleanType));
    FunctionRepository::Instance().AddBinaryOperatorFunction(new BinaryOperatorFunction(Operator::lessOrEqual, realType, booleanType));
    FunctionRepository::Instance().AddBinaryOperatorFunction(new BinaryOperatorFunction(Operator::greaterOrEqual, realType, booleanType));
    FunctionRepository::Instance().AddBinaryOperatorFunction(new BinaryOperatorFunction(Operator::plus, realType, realType));
    FunctionRepository::Instance().AddBinaryOperatorFunction(new BinaryOperatorFunction(Operator::minus, realType, realType));
    FunctionRepository::Instance().AddBinaryOperatorFunction(new BinaryOperatorFunction(Operator::mul, realType, realType));
    FunctionRepository::Instance().AddBinaryOperatorFunction(new BinaryOperatorFunction(Operator::fractionalDivide, realType, realType));
    FunctionRepository::Instance().AddBinaryOperatorFunction(new BinaryOperatorFunction(Operator::equal, charType, booleanType));
    FunctionRepository::Instance().AddBinaryOperatorFunction(new BinaryOperatorFunction(Operator::notEqual, charType, booleanType));
    FunctionRepository::Instance().AddBinaryOperatorFunction(new BinaryOperatorFunction(Operator::equal, stringType, booleanType));
    FunctionRepository::Instance().AddBinaryOperatorFunction(new BinaryOperatorFunction(Operator::notEqual, stringType, booleanType));
    FunctionRepository::Instance().AddBinaryOperatorFunction(new BinaryOperatorFunction(Operator::plus, stringType, stringType));
    FunctionRepository::Instance().AddBinaryOperatorFunction(new BinaryOperatorFunction(Operator::equal, nilType, booleanType));
    FunctionRepository::Instance().AddUnaryOperatorFunction(new UnaryOperatorFunction(Operator::not_, booleanType, booleanType));
    FunctionRepository::Instance().AddUnaryOperatorFunction(new UnaryOperatorFunction(Operator::not_, integerType, integerType));
    FunctionRepository::Instance().AddUnaryOperatorFunction(new UnaryOperatorFunction(Operator::plus, integerType, integerType));
    FunctionRepository::Instance().AddUnaryOperatorFunction(new UnaryOperatorFunction(Operator::minus, integerType, integerType));
    FunctionRepository::Instance().AddUnaryOperatorFunction(new UnaryOperatorFunction(Operator::plus, realType, realType));
    FunctionRepository::Instance().AddUnaryOperatorFunction(new UnaryOperatorFunction(Operator::minus, realType, realType));
    FunctionRepository::Instance().AddConversionFunction(new ConversionFunction(realType, integerType));
    FunctionRepository::Instance().AddConversionFunction(new ConversionFunction(stringType, charType));
}

Function* GetBinaryOperatorFunction(Operator op, Type* type, soul::lexer::LexerBase<char>& lexer, int64_t pos)
{
    return FunctionRepository::Instance().GetBinaryOperatorFunction(op, type, lexer, pos);
}

Function* GetUnaryOperatorFunction(Operator op, Type* type, soul::lexer::LexerBase<char>& lexer, int64_t pos)
{
    return FunctionRepository::Instance().GetUnaryOperatorFunction(op, type, lexer, pos);
}

Function* GetConversionFunction(Type* targetType, Type* sourceType, soul::lexer::LexerBase<char>& lexer, int64_t pos, bool throw_)
{
    return FunctionRepository::Instance().GetConversionFunction(targetType, sourceType, lexer, pos, throw_);
}

} // namespace p
