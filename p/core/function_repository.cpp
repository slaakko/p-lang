// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.core.function_repository;

import p.core.type_symbol;
import p.core.symbol_table;
import p.core.emitter;

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

BinaryOperatorFunctionSymbol::BinaryOperatorFunctionSymbol(const soul::ast::Span& span_, Operator op_, TypeSymbol* type_, TypeSymbol* resultType_) :
    FunctionSymbol(span_, "@binary.operation." + MakeOpName(op_) + "." + type_->Name()), op(op_), type(type_), resultType(resultType_)
{
}

void BinaryOperatorFunctionSymbol::GenerateCode(Emitter* emitter, const soul::ast::Span& span)
{
    switch (type->Kind())
    {
        case SymbolKind::booleanTypeSymbol:
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
                    ThrowError("error: invalid Boolean binary operation", emitter->FilePath(), span);
                    break;
                }
            }
            break;
        }
        case SymbolKind::integerTypeSymbol:
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
                case Operator::or_:
                {
                    emitter->Emit(new OrIntInstruction());
                    break;
                }
                case Operator::xor_:
                {
                    emitter->Emit(new XorIntInstruction());
                    break;
                }
                default:
                {
                    ThrowError("error: invalid integer binary operation", emitter->FilePath(), span);
                    break;
                }
            }
            break;
        }
        case SymbolKind::realTypeSymbol:
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
                    ThrowError("error: invalid real binary operation", emitter->FilePath(), span);
                    break;
                }
            }
            break;
        }
        case SymbolKind::charTypeSymbol:
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
                    ThrowError("error: invalid character binary operation", emitter->FilePath(), span);
                    break;
                }
            }
            break;
        }
        case SymbolKind::stringTypeSymbol:
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
                case Operator::less:
                {
                    emitter->Emit(new LessStringInstruction());
                    break;
                }
                case Operator::greater:
                {
                    emitter->Emit(new GreaterStringInstruction());
                    break;
                }
                case Operator::lessOrEqual:
                {
                    emitter->Emit(new LessOrEqualStringInstruction());
                    break;
                }
                case Operator::greaterOrEqual:
                {
                    emitter->Emit(new GreaterOrEqualStringInstruction());
                    break;
                }
                case Operator::plus:
                {
                    emitter->Emit(new PlusStringInstruction());
                    break;
                }
                default:
                {
                    ThrowError("error: invalid string binary operation", emitter->FilePath(), span);
                    break;
                }
            }
            break;
        }
        case SymbolKind::nilTypeSymbol:
        {
            emitter->Emit(new EqualNilInstruction());
            break;
        }
        default:
        {
            ThrowError("error: invalid binary operation operand type", emitter->FilePath(), span);
        }
    }
}

UnaryOperatorFunctionSymbol::UnaryOperatorFunctionSymbol(const soul::ast::Span& span_, Operator op_, TypeSymbol* type_, TypeSymbol* resultType_) :
    FunctionSymbol(span_, "@unary.operation." + MakeOpName(op) + "." + type_->Name()), op(op_), type(type_), resultType(resultType_)
{
}

void UnaryOperatorFunctionSymbol::GenerateCode(Emitter* emitter, const soul::ast::Span& span)
{
    switch (type->Kind())
    {
        case SymbolKind::booleanTypeSymbol:
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
                    ThrowError("error: invalid Boolean unary operation", emitter->FilePath(), span);
                    break;
                }
            }
            break;
        }
        case SymbolKind::integerTypeSymbol:
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
                    ThrowError("error: invalid integer unary operation", emitter->FilePath(), span);
                    break;
                }
            }
            break;
        }
        case SymbolKind::realTypeSymbol:
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
                    ThrowError("error: invalid real unary operation", emitter->FilePath(), span);
                    break;
                }
            }
            break;
        }
        default:
        {
            ThrowError("error: invalid unary operation operand type", emitter->FilePath(), span);
        }
    }
}

ConversionFunctionSymbol::ConversionFunctionSymbol(const soul::ast::Span& span_, TypeSymbol* targetType_, TypeSymbol* sourceType_) :
    FunctionSymbol(span_, "@conversion.function." + targetType_->Name() + "." + sourceType_->Name()), targetType(targetType_), sourceType(sourceType_)
{
}
void ConversionFunctionSymbol::GenerateCode(Emitter* emitter, const soul::ast::Span& span)
{
    switch (targetType->Kind())
    {
        case SymbolKind::realTypeSymbol:
        {
            if (sourceType->Kind() == SymbolKind::integerTypeSymbol)
            {
                emitter->Emit(new IntToRealInstruction());
            }
            else
            {
                ThrowError("error: invalid type conversion", emitter->FilePath(), span);
            }
            break;
        }
        case SymbolKind::integerTypeSymbol:
        {
            if (sourceType->Kind() == SymbolKind::realTypeSymbol)
            {
                emitter->Emit(new RealToIntInstruction());
            }
            else if (sourceType->IsOrdinalTypeSymbol())
            {
                emitter->Emit(new IdentityInstruction());
            }
            else
            {
                ThrowError("error: invalid type conversion", emitter->FilePath(), span);
            }
            break;
        }
        case SymbolKind::stringTypeSymbol:
        {
            if (sourceType->Kind() == SymbolKind::charTypeSymbol)
            {
                emitter->Emit(new CharToStringInstruction());
            }
            else
            {
                ThrowError("error: invalid type conversion", emitter->FilePath(), span);
            }
            break;
        }
        default:
        {
            if (sourceType->IsOrdinalTypeSymbol() && targetType->IsOrdinalTypeSymbol())
            {
                emitter->Emit(new IdentityInstruction());
            }
            else
            {
                ThrowError("error: invalid type conversion", emitter->FilePath(), span);
            }
            break;
        }
    }
}

class FunctionRepository
{
public:
    static FunctionRepository& Instance();
    void AddBinaryOperatorFunction(BinaryOperatorFunctionSymbol* function);
    FunctionSymbol* GetBinaryOperatorFunction(Operator op, TypeSymbol* type, Node* node) const;
    void AddUnaryOperatorFunction(UnaryOperatorFunctionSymbol* function);
    FunctionSymbol* GetUnaryOperatorFunction(Operator op, TypeSymbol* type, Node* node) const;
    void AddConversionFunction(ConversionFunctionSymbol* function);
    FunctionSymbol* GetConversionFunction(TypeSymbol* targetType, TypeSymbol* sourceType, Node* node, bool throw_);
    void Clear();
private:
    FunctionRepository();
    std::map<std::pair<Operator, TypeSymbol*>, BinaryOperatorFunctionSymbol*> binaryOperatorFunctionMap;
    std::map<std::pair<Operator, TypeSymbol*>, UnaryOperatorFunctionSymbol*> unaryOperatorFunctionMap;
    std::map<std::pair<TypeSymbol*, TypeSymbol*>, ConversionFunctionSymbol*> conversionFunctionMap;
    std::vector<std::unique_ptr<FunctionSymbol>> functions;
};

FunctionRepository& FunctionRepository::Instance()
{
    static FunctionRepository instance;
    return instance;
}

FunctionRepository::FunctionRepository() 
{
}

void FunctionRepository::Clear()
{
    binaryOperatorFunctionMap.clear();
    unaryOperatorFunctionMap.clear();
    conversionFunctionMap.clear();
    functions.clear();
}

void FunctionRepository::AddBinaryOperatorFunction(BinaryOperatorFunctionSymbol* function)
{
    binaryOperatorFunctionMap[std::make_pair(function->Op(), function->Type())] = function;
    functions.push_back(std::unique_ptr<FunctionSymbol>(function));
}

FunctionSymbol* FunctionRepository::GetBinaryOperatorFunction(Operator op, TypeSymbol* type, Node* node) const
{
    auto it = binaryOperatorFunctionMap.find(std::make_pair(op, type));
    if (it != binaryOperatorFunctionMap.end())
    {
        return it->second;
    }
    else
    {
        ThrowError("binary operator function '" + MakeOpName(op) + "' for type '" + type->Name() + "' not found", node->FilePath(), node->Span());
    }
    return nullptr;
}

void FunctionRepository::AddUnaryOperatorFunction(UnaryOperatorFunctionSymbol* function)
{
    unaryOperatorFunctionMap[std::make_pair(function->Op(), function->Type())] = function;
    functions.push_back(std::unique_ptr<FunctionSymbol>(function));
}

FunctionSymbol* FunctionRepository::GetUnaryOperatorFunction(Operator op, TypeSymbol* type, Node* node) const
{
    auto it = unaryOperatorFunctionMap.find(std::make_pair(op, type));
    if (it != unaryOperatorFunctionMap.end())
    {
        return it->second;
    }
    else
    {
        ThrowError("unary operator function '" + MakeOpName(op) + "' for type '" + type->Name() + "' not found", node->FilePath(), node->Span());
    }
    return nullptr;
}

void FunctionRepository::AddConversionFunction(ConversionFunctionSymbol* function)
{
    conversionFunctionMap[std::make_pair(function->TargetType(), function->SourceType())] = function;
    functions.push_back(std::unique_ptr<FunctionSymbol>(function));
}

FunctionSymbol* FunctionRepository::GetConversionFunction(TypeSymbol* targetType, TypeSymbol* sourceType, Node* node, bool throw_) 
{
    auto it = conversionFunctionMap.find(std::make_pair(targetType, sourceType));
    if (it != conversionFunctionMap.end())
    {
        return it->second;
    }
    else
    {
        if (targetType->IsOrdinalTypeSymbol() && sourceType->IsOrdinalTypeSymbol())
        {
            ConversionFunctionSymbol* conversionFunction = new ConversionFunctionSymbol(node->Span(), targetType, sourceType);
            AddConversionFunction(conversionFunction);
            return conversionFunction;
        }
        else
        {
            if (throw_)
            {
                ThrowError("error: conversion from '" + sourceType->Name() + "' to '" + targetType->Name() + "' not found", node->FilePath(), node->Span());
            }
        }
    }
    return nullptr;
}


FunctionSymbol* GetBinaryOperatorFunction(Operator op, TypeSymbol* type, Node* node)
{
    return FunctionRepository::Instance().GetBinaryOperatorFunction(op, type, node);
}

FunctionSymbol* GetUnaryOperatorFunction(Operator op, TypeSymbol* type, Node* node) 
{
    return FunctionRepository::Instance().GetUnaryOperatorFunction(op, type, node);
}

FunctionSymbol* GetConversionFunction(TypeSymbol* targetType, TypeSymbol* sourceType, Node* node, bool throw_)
{
    return FunctionRepository::Instance().GetConversionFunction(targetType, sourceType, node, throw_);
}

void MakeOperatorFunctions(SymbolTable* symbolTable, const std::string& filePath, const soul::ast::Span& span)
{
    FunctionRepository& repository = FunctionRepository::Instance();
    repository.Clear();
    TypeSymbol* booleanType = symbolTable->GetType(GetBooleanTypeId(), filePath, span);
    TypeSymbol* integerType = symbolTable->GetType(GetIntegerTypeId(), filePath, span);
    TypeSymbol* realType = symbolTable->GetType(GetRealTypeId(), filePath, span);
    TypeSymbol* charType = symbolTable->GetType(GetCharTypeId(), filePath, span);
    TypeSymbol* stringType = symbolTable->GetType(GetStringTypeId(), filePath, span);
    TypeSymbol* pointerType = symbolTable->GetType(GetPointerTypeId(), filePath, span);
    TypeSymbol* nilType = symbolTable->GetType(GetNilTypeId(), filePath, span);
    repository.AddBinaryOperatorFunction(new BinaryOperatorFunctionSymbol(span, Operator::equal, booleanType, booleanType));
    repository.AddBinaryOperatorFunction(new BinaryOperatorFunctionSymbol(span, Operator::notEqual, booleanType, booleanType));
    repository.AddBinaryOperatorFunction(new BinaryOperatorFunctionSymbol(span, Operator::and_, booleanType, booleanType));
    repository.AddBinaryOperatorFunction(new BinaryOperatorFunctionSymbol(span, Operator::or_, booleanType, booleanType));
    repository.AddBinaryOperatorFunction(new BinaryOperatorFunctionSymbol(span, Operator::xor_, booleanType, booleanType));
    repository.AddBinaryOperatorFunction(new BinaryOperatorFunctionSymbol(span, Operator::equal, integerType, booleanType));
    repository.AddBinaryOperatorFunction(new BinaryOperatorFunctionSymbol(span, Operator::notEqual, integerType, booleanType));
    repository.AddBinaryOperatorFunction(new BinaryOperatorFunctionSymbol(span, Operator::less, integerType, booleanType));
    repository.AddBinaryOperatorFunction(new BinaryOperatorFunctionSymbol(span, Operator::greater, integerType, booleanType));
    repository.AddBinaryOperatorFunction(new BinaryOperatorFunctionSymbol(span, Operator::lessOrEqual, integerType, booleanType));
    repository.AddBinaryOperatorFunction(new BinaryOperatorFunctionSymbol(span, Operator::greaterOrEqual, integerType, booleanType));
    repository.AddBinaryOperatorFunction(new BinaryOperatorFunctionSymbol(span, Operator::plus, integerType, integerType));
    repository.AddBinaryOperatorFunction(new BinaryOperatorFunctionSymbol(span, Operator::minus, integerType, integerType));
    repository.AddBinaryOperatorFunction(new BinaryOperatorFunctionSymbol(span, Operator::mul, integerType, integerType));
    repository.AddBinaryOperatorFunction(new BinaryOperatorFunctionSymbol(span, Operator::fractionalDivide, integerType, realType));
    repository.AddBinaryOperatorFunction(new BinaryOperatorFunctionSymbol(span, Operator::div, integerType, integerType));
    repository.AddBinaryOperatorFunction(new BinaryOperatorFunctionSymbol(span, Operator::mod, integerType, integerType));
    repository.AddBinaryOperatorFunction(new BinaryOperatorFunctionSymbol(span, Operator::and_, integerType, integerType));
    repository.AddBinaryOperatorFunction(new BinaryOperatorFunctionSymbol(span, Operator::or_, integerType, integerType));
    repository.AddBinaryOperatorFunction(new BinaryOperatorFunctionSymbol(span, Operator::xor_, integerType, integerType));
    repository.AddBinaryOperatorFunction(new BinaryOperatorFunctionSymbol(span, Operator::shl, integerType, integerType));
    repository.AddBinaryOperatorFunction(new BinaryOperatorFunctionSymbol(span, Operator::shr, integerType, integerType));
    repository.AddBinaryOperatorFunction(new BinaryOperatorFunctionSymbol(span, Operator::equal, realType, booleanType));
    repository.AddBinaryOperatorFunction(new BinaryOperatorFunctionSymbol(span, Operator::notEqual, realType, booleanType));
    repository.AddBinaryOperatorFunction(new BinaryOperatorFunctionSymbol(span, Operator::less, realType, booleanType));
    repository.AddBinaryOperatorFunction(new BinaryOperatorFunctionSymbol(span, Operator::greater, realType, booleanType));
    repository.AddBinaryOperatorFunction(new BinaryOperatorFunctionSymbol(span, Operator::lessOrEqual, realType, booleanType));
    repository.AddBinaryOperatorFunction(new BinaryOperatorFunctionSymbol(span, Operator::greaterOrEqual, realType, booleanType));
    repository.AddBinaryOperatorFunction(new BinaryOperatorFunctionSymbol(span, Operator::plus, realType, realType));
    repository.AddBinaryOperatorFunction(new BinaryOperatorFunctionSymbol(span, Operator::minus, realType, realType));
    repository.AddBinaryOperatorFunction(new BinaryOperatorFunctionSymbol(span, Operator::mul, realType, realType));
    repository.AddBinaryOperatorFunction(new BinaryOperatorFunctionSymbol(span, Operator::fractionalDivide, realType, realType));
    repository.AddBinaryOperatorFunction(new BinaryOperatorFunctionSymbol(span, Operator::equal, charType, booleanType));
    repository.AddBinaryOperatorFunction(new BinaryOperatorFunctionSymbol(span, Operator::notEqual, charType, booleanType));
    repository.AddBinaryOperatorFunction(new BinaryOperatorFunctionSymbol(span, Operator::equal, stringType, booleanType));
    repository.AddBinaryOperatorFunction(new BinaryOperatorFunctionSymbol(span, Operator::notEqual, stringType, booleanType));
    repository.AddBinaryOperatorFunction(new BinaryOperatorFunctionSymbol(span, Operator::less, stringType, booleanType));
    repository.AddBinaryOperatorFunction(new BinaryOperatorFunctionSymbol(span, Operator::greater, stringType, booleanType));
    repository.AddBinaryOperatorFunction(new BinaryOperatorFunctionSymbol(span, Operator::lessOrEqual, stringType, booleanType));
    repository.AddBinaryOperatorFunction(new BinaryOperatorFunctionSymbol(span, Operator::greaterOrEqual, stringType, booleanType));
    repository.AddBinaryOperatorFunction(new BinaryOperatorFunctionSymbol(span, Operator::plus, stringType, stringType));
    repository.AddBinaryOperatorFunction(new BinaryOperatorFunctionSymbol(span, Operator::equal, nilType, booleanType));
    repository.AddUnaryOperatorFunction(new UnaryOperatorFunctionSymbol(span, Operator::not_, booleanType, booleanType));
    repository.AddUnaryOperatorFunction(new UnaryOperatorFunctionSymbol(span, Operator::not_, integerType, integerType));
    repository.AddUnaryOperatorFunction(new UnaryOperatorFunctionSymbol(span, Operator::plus, integerType, integerType));
    repository.AddUnaryOperatorFunction(new UnaryOperatorFunctionSymbol(span, Operator::minus, integerType, integerType));
    repository.AddUnaryOperatorFunction(new UnaryOperatorFunctionSymbol(span, Operator::plus, realType, realType));
    repository.AddUnaryOperatorFunction(new UnaryOperatorFunctionSymbol(span, Operator::minus, realType, realType));
    repository.AddConversionFunction(new ConversionFunctionSymbol(span, realType, integerType));
    repository.AddConversionFunction(new ConversionFunctionSymbol(span, integerType, realType));
    repository.AddConversionFunction(new ConversionFunctionSymbol(span, stringType, charType));
}

} // namespace p
