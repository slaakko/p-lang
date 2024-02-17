// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.core.standard_subroutine;

import p.ast;
import p.core.execution_context;
import p.core.symbol_table;
import p.core.type_symbol;
import p.core.object;
import p.core.value;

namespace p {

StandardFunctionSymbol::StandardFunctionSymbol(SymbolKind kind_, const soul::ast::Span& span_, const std::string& name_) : FunctionSymbol(kind_, span_, name_)
{
    SetStandard();
}

StandardProcedureSymbol::StandardProcedureSymbol(SymbolKind kind_, const soul::ast::Span& span_, const std::string& name_) : ProcedureSymbol(kind_, span_, name_)
{
    SetStandard();
}

Ord::Ord(Node* node, SymbolTable* symbolTable) : StandardFunctionSymbol(SymbolKind::ordSymbol, node->Span(), "Ord")
{
    SetResultType(symbolTable->GetType(GetIntegerTypeId(), node));
}

Ord::Ord(const soul::ast::Span& span_, const std::string& name_) : StandardFunctionSymbol(SymbolKind::ordSymbol, span_, name_)
{
}

Value* Ord::Evaluate(const std::vector<std::unique_ptr<Value>>& argumentValues, Context* context, Node* node) const
{
    if (argumentValues.size() != 1)
    {
        ThrowError("error: 'Ord' function takes one argument, " + std::to_string(argumentValues.size()) + " arguments supplied", node->FilePath(), node->Span());
    }
    Value* value = argumentValues[0]->GetValue();
    switch (value->Kind())
    {
        case ValueKind::booleanValue:
        case ValueKind::integerValue:
        case ValueKind::charValue:
        case ValueKind::enumerationValue:
        {
            return new IntegerValue(value->ToInteger());
        }
        default:
        {
            ThrowError("error: invalid 'Ord' function argument type", node->FilePath(), node->Span());
        }
    }
    return nullptr;
}

void Ord::Execute(ExecutionContext* context)
{
    std::unique_ptr<Object> arg = context->GetStack()->Pop();
    Object* argObj = arg->GetObject();
    std::unique_ptr<Object> result(new IntegerValue(argObj->ToInteger()));
    context->GetStack()->Push(result.release());
}

Chr::Chr(Node* node, SymbolTable* symbolTable) : StandardFunctionSymbol(SymbolKind::chrSymbol, node->Span(), "Chr")
{
    SetResultType(symbolTable->GetType(GetCharTypeId(), node));
}

Chr::Chr(const soul::ast::Span& span_, const std::string& name_) : StandardFunctionSymbol(SymbolKind::chrSymbol, span_, name_)
{
}

Value* Chr::Evaluate(const std::vector<std::unique_ptr<Value>>& argumentValues, Context* context, Node* node) const
{
    if (argumentValues.size() != 1)
    {
        ThrowError("error: 'Chr' function takes one argument, " + std::to_string(argumentValues.size()) + " arguments supplied", node->FilePath(), node->Span());
    }
    Value* value = argumentValues[0]->GetValue();
    switch (value->Kind())
    {
        case ValueKind::integerValue:
        {
            int32_t intval = value->ToInteger();
            if (intval < 0 || intval > 255)
            {
                ThrowError("error: invalid 'Chr' function argument", node->FilePath(), node->Span());
            }
            return new CharValue(static_cast<char>(intval));
        }
        default:
        {
            ThrowError("error: invalid 'Chr' function argument type", node->FilePath(), node->Span());
        }
    }
    return nullptr;
}

void Chr::Execute(ExecutionContext* context)
{
    std::unique_ptr<Object> arg = context->GetStack()->Pop();
    std::unique_ptr<Object> result(new CharValue(static_cast<char>(arg->GetObject()->ToInteger())));
    context->GetStack()->Push(result.release());
}

Sin::Sin(Node* node, SymbolTable* symbolTable) : StandardFunctionSymbol(SymbolKind::sinSymbol, node->Span(), "Sin")
{
    SetResultType(symbolTable->GetType(GetRealTypeId(), node));
}

Sin::Sin(const soul::ast::Span& span_, const std::string& name_) : StandardFunctionSymbol(SymbolKind::sinSymbol, span_, name_)
{
}

void Sin::Execute(ExecutionContext* context)
{
    std::unique_ptr<Object> arg = context->GetStack()->Pop();
    std::unique_ptr<Object> result(new RealValue(std::sin(arg->GetObject()->ToReal())));
    context->GetStack()->Push(result.release());
}

Cos::Cos(Node* node, SymbolTable* symbolTable) : StandardFunctionSymbol(SymbolKind::cosSymbol, node->Span(), "Cos")
{
    SetResultType(symbolTable->GetType(GetRealTypeId(), node));
}

Cos::Cos(const soul::ast::Span& span_, const std::string& name_) : StandardFunctionSymbol(SymbolKind::cosSymbol, span_, name_)
{
}

void Cos::Execute(ExecutionContext* context)
{
    std::unique_ptr<Object> arg = context->GetStack()->Pop();
    std::unique_ptr<Object> result(new RealValue(std::cos(arg->GetObject()->ToReal())));
    context->GetStack()->Push(result.release());
}

ArcTan::ArcTan(Node* node, SymbolTable* symbolTable) : StandardFunctionSymbol(SymbolKind::arcTanSymbol, node->Span(), "ArcTan")
{
    SetResultType(symbolTable->GetType(GetRealTypeId(), node));
}

ArcTan::ArcTan(const soul::ast::Span& span_, const std::string& name_) : StandardFunctionSymbol(SymbolKind::arcTanSymbol, span_, name_)
{
}

void ArcTan::Execute(ExecutionContext* context)
{
    std::unique_ptr<Object> arg = context->GetStack()->Pop();
    std::unique_ptr<Object> result(new RealValue(std::atan(arg->GetObject()->ToReal())));
    context->GetStack()->Push(result.release());
}

Sqrt::Sqrt(Node* node, SymbolTable* symbolTable) : StandardFunctionSymbol(SymbolKind::sqrtSymbol, node->Span(), "Sqrt")
{
    SetResultType(symbolTable->GetType(GetRealTypeId(), node));
}

Sqrt::Sqrt(const soul::ast::Span& span_, const std::string& name_) : StandardFunctionSymbol(SymbolKind::sqrtSymbol, span_, name_)
{
}

void Sqrt::Execute(ExecutionContext* context)
{
    std::unique_ptr<Object> arg = context->GetStack()->Pop();
    Object* argObject = arg->GetObject();
    if (argObject->IsValueObject())
    {
        Value* value = static_cast<Value*>(argObject);
        context->GetStack()->Push(new RealValue(std::sqrt(value->ToReal())));
    }
    else
    {
        throw std::runtime_error("error: sqrt: value object expected");
    }
}

Abs::Abs(Node* node, SymbolTable* symbolTable) : StandardFunctionSymbol(SymbolKind::absSymbol, node->Span(), "Abs")
{
}

Abs::Abs(const soul::ast::Span& span_, const std::string& name_) : StandardFunctionSymbol(SymbolKind::absSymbol, span_, name_)
{
}

Value* Abs::Evaluate(const std::vector<std::unique_ptr<Value>>& argumentValues, Context* context, Node* node) const
{
    if (argumentValues.size() != 1)
    {
        ThrowError("error: 'Abs' function takes one argument, " + std::to_string(argumentValues.size()) + " arguments supplied", node->FilePath(), node->Span());
    }
    Value* value = argumentValues[0]->GetValue();
    switch (value->Kind())
    {
        case ValueKind::integerValue:
        {
            IntegerValue* integerValue = new IntegerValue(value->ToInteger());
            integerValue->SetType(context->GetSymbolTable()->GetType(GetIntegerTypeId(), node));
            return integerValue;
        }
        case ValueKind::realValue:
        {
            RealValue* realValue = new RealValue(value->ToReal());
            realValue->SetType(context->GetSymbolTable()->GetType(GetRealTypeId(), node));
            return realValue;
        }
        default:
        {
            ThrowError("error: invalid 'Abs' function argument type", node->FilePath(), node->Span());
        }
    }
    return nullptr;
}

void Abs::Execute(ExecutionContext* context)
{
    std::unique_ptr<Object> arg = context->GetStack()->Pop();
    Object* argObject = arg->GetObject();
    std::unique_ptr<Object> result;
    if (argObject->IsValueObject())
    {
        Value* value = static_cast<Value*>(argObject);
        switch (value->Kind())
        {
            case ValueKind::integerValue:
            {
                result.reset(new IntegerValue(std::abs(value->ToInteger())));
            }
            case ValueKind::realValue:
            {
                result.reset(new RealValue(std::fabs(value->ToReal())));
            }
            default:
            {
                throw std::runtime_error("error: Abs: integer or real object expected");
            }
        }
    }
    else
    {
        throw std::runtime_error("error: Abs: value object expected");
    }
    context->GetStack()->Push(result.release());
}

Succ::Succ(Node* node, SymbolTable* symbolTable) : StandardFunctionSymbol(SymbolKind::succSymbol, node->Span(), "Succ")
{
}

Succ::Succ(const soul::ast::Span& span_, const std::string& name_) : StandardFunctionSymbol(SymbolKind::succSymbol, span_, name_)
{
}

Value* Succ::Evaluate(const std::vector<std::unique_ptr<Value>>& argumentValues, Context* context, Node* node) const
{
    if (argumentValues.size() != 1)
    {
        ThrowError("error: 'Succ' function takes one argument, " + std::to_string(argumentValues.size()) + " arguments supplied", node->FilePath(), node->Span());
    }
    Value* value = argumentValues[0]->GetValue();
    return value->Succ(node);
}

void Succ::Execute(ExecutionContext* context)
{
    std::unique_ptr<Object> arg = context->GetStack()->Pop();
    Object* argObject = arg->GetObject();
    if (argObject->IsValueObject())
    {
        Value* value = static_cast<Value*>(argObject);
        context->GetStack()->Push(value->Succ());
    }
    else
    {
        throw std::runtime_error("error: Succ: value object expected");
    }
}

Pred::Pred(Node* node, SymbolTable* symbolTable) : StandardFunctionSymbol(SymbolKind::predSymbol, node->Span(), "Pred")
{
}

Pred::Pred(const soul::ast::Span& span_, const std::string& name_) : StandardFunctionSymbol(SymbolKind::predSymbol, span_, name_)
{
}

Value* Pred::Evaluate(const std::vector<std::unique_ptr<Value>>& argumentValues, Context* context, Node* node) const
{
    if (argumentValues.size() != 1)
    {
        ThrowError("error: 'Succ' function takes one argument, " + std::to_string(argumentValues.size()) + " arguments supplied", node->FilePath(), node->Span());
    }
    Value* value = argumentValues[0]->GetValue();
    return value->Pred(node);
}

void Pred::Execute(ExecutionContext* context)
{
    std::unique_ptr<Object> arg = context->GetStack()->Pop();
    Object* argObject = arg->GetObject();
    if (argObject->IsValueObject())
    {
        Value* value = static_cast<Value*>(argObject);
        context->GetStack()->Push(value->Pred());
    }
    else
    {
        throw std::runtime_error("error: Succ: value object expected");
    }
}

Min::Min(Node* node, SymbolTable* symbolTable) : StandardFunctionSymbol(SymbolKind::minSymbol, node->Span(), "Min")
{
}

Min::Min(const soul::ast::Span& span_, const std::string& name_) : StandardFunctionSymbol(SymbolKind::minSymbol, span_, name_)
{
}

TypeSymbol* Min::ResultType(const std::vector<TypeSymbol*>& argumentTypes, Node* node) const
{
    if (argumentTypes.size() != 2)
    {
        ThrowError("error: min: two arguments expected", node->FilePath(), node->Span());
    }
    TypeSymbol* commonType = CommonType(argumentTypes[0], argumentTypes[1]);
    if (commonType && (commonType->IsIntegerTypeSymbol() || commonType->IsRealTypeSymbol()))
    {
        return commonType;
    }
    else
    {
        ThrowError("error: min: invalid argument types", node->FilePath(), node->Span());
    }
    return nullptr;
}

void Min::Execute(ExecutionContext* context)
{
    std::unique_ptr<Object> right = context->GetStack()->Pop();
    std::unique_ptr<Object> left = context->GetStack()->Pop();
    Object* leftObject = left->GetObject();
    Object* rightObject = right->GetObject();
    std::unique_ptr<Object> result;
    if (leftObject->IsValueObject() && rightObject->IsValueObject())
    {
        Value* leftValue = static_cast<Value*>(leftObject);
        Value* rightValue = static_cast<Value*>(rightObject);
        ValueKind commonType = CommonType(leftValue->Kind(), rightValue->Kind());
        switch (commonType)
        {
            case ValueKind::integerValue:
            {
                result.reset(new IntegerValue(std::min(leftValue->ToInteger(), rightValue->ToInteger())));
                break;
            }
            case ValueKind::realValue:
            {
                result.reset(new RealValue(std::min(leftValue->ToReal(), rightValue->ToReal())));
                break;
            }
            default:
            {
                throw std::runtime_error("error: Min: integer or real objects expected");
            }
        }
    }
    else
    {
        throw std::runtime_error("error: Min: value objects expected");
    }
    context->GetStack()->Push(result.release());
}

Max::Max(Node* node, SymbolTable* symbolTable) : StandardFunctionSymbol(SymbolKind::maxSymbol, node->Span(), "Max")
{
}

Max::Max(const soul::ast::Span& span_, const std::string& name_) : StandardFunctionSymbol(SymbolKind::maxSymbol, span_, name_)
{
}

TypeSymbol* Max::ResultType(const std::vector<TypeSymbol*>& argumentTypes, Node* node) const
{
    if (argumentTypes.size() != 2)
    {
        ThrowError("error: max: two arguments expected", node->FilePath(), node->Span());
    }
    TypeSymbol* commonType = CommonType(argumentTypes[0], argumentTypes[1]);
    if (commonType && (commonType->IsIntegerTypeSymbol() || commonType->IsRealTypeSymbol()))
    {
        return commonType;
    }
    else
    {
        ThrowError("error: max: invalid argument types", node->FilePath(), node->Span());
    }
    return nullptr;
}

void Max::Execute(ExecutionContext* context)
{
    std::unique_ptr<Object> right = context->GetStack()->Pop();
    std::unique_ptr<Object> left = context->GetStack()->Pop();
    Object* leftObject = left->GetObject();
    Object* rightObject = right->GetObject();
    std::unique_ptr<Object> result;
    if (leftObject->IsValueObject() && rightObject->IsValueObject())
    {
        Value* leftValue = static_cast<Value*>(leftObject);
        Value* rightValue = static_cast<Value*>(rightObject);
        ValueKind commonType = CommonType(leftValue->Kind(), rightValue->Kind());
        switch (commonType)
        {
            case ValueKind::integerValue:
            {
                result.reset(new IntegerValue(std::max(leftValue->ToInteger(), rightValue->ToInteger())));
                break;
            }
            case ValueKind::realValue:
            {
                result.reset(new RealValue(std::max(leftValue->ToReal(), rightValue->ToReal())));
                break;
            }
            default:
            {
                throw std::runtime_error("error: Max: integer or real objects expected");
            }
        }
    }
    else
    {
        throw std::runtime_error("error: Max: value objects expected");
    }
    context->GetStack()->Push(result.release());
}

void WriteValue(BooleanValue* value)
{
    if (value->GetBooleanValue())
    {
        std::cout << "true";
    }
    else
    {
        std::cout << "false";
    }
}

void WriteValue(IntegerValue* value)
{
    int32_t v = value->GetIntegerValue();
    std::cout << v;
}

void WriteValue(CharValue* value)
{
    char v = value->GetCharValue();
    std::cout << std::string(1, v);
}

void WriteValue(RealValue* value)
{
    float v = value->GetRealValue();
    std::cout << v;
}

void WriteValue(StringValue* value)
{
    std::cout << value->GetStringValue();
}

void WriteValue(Value* value)
{
    switch (value->Kind())
    {
        case ValueKind::booleanValue:
        {
            WriteValue(static_cast<BooleanValue*>(value));
            break;
        }
        case ValueKind::integerValue:
        {
            WriteValue(static_cast<IntegerValue*>(value));
            break;
        }
        case ValueKind::charValue:
        {
            WriteValue(static_cast<CharValue*>(value));
            break;
        }
        case ValueKind::realValue:
        {
            WriteValue(static_cast<RealValue*>(value));
            break;
        }
        case ValueKind::stringValue:
        {
            WriteValue(static_cast<StringValue*>(value));
            break;
        }
    }
}

Write::Write(Node* node, SymbolTable* symbolTable) : StandardProcedureSymbol(SymbolKind::writeSymbol, node->Span(), "Write")
{
}

Write::Write(const soul::ast::Span& span_, const std::string& name_) : StandardProcedureSymbol(SymbolKind::writeSymbol, span_, name_)
{
}

void Write::Execute(ExecutionContext* context)
{
    std::vector<std::unique_ptr<Object>> args;
    int32_t argumentCount = context->ArgumentCount();
    args.resize(argumentCount);
    for (int32_t i = 0; i < argumentCount; ++i)
    {
        std::unique_ptr<Object> arg = context->GetStack()->Pop();
        args[argumentCount - i - 1].reset(arg.release());
    }
    for (int32_t i = 0; i < argumentCount; ++i)
    {
        Object* argObj = args[i].get();
        Object* arg = argObj->GetObject();
        if (arg->IsValueObject())
        {
            Value* value = static_cast<Value*>(arg);
            p::WriteValue(value);
        }
        else if (arg->IsStringObject())
        {
            StringObject* string = static_cast<StringObject*>(arg);
            std::cout << string->Value();
        }
    }
}

Writeln::Writeln(Node* node, SymbolTable* symbolTable) : StandardProcedureSymbol(SymbolKind::writelnSymbol, node->Span(), "Writeln")
{
}

Writeln::Writeln(const soul::ast::Span& span_, const std::string& name_) : StandardProcedureSymbol(SymbolKind::writelnSymbol, span_, name_)
{
}

void Writeln::Execute(ExecutionContext* context)
{
    std::vector<std::unique_ptr<Object>> args;
    int32_t argumentCount = context->ArgumentCount();
    args.resize(argumentCount);
    for (int32_t i = 0; i < argumentCount; ++i)
    {
        std::unique_ptr<Object> arg = context->GetStack()->Pop();
        args[argumentCount - i - 1].reset(arg.release());
    }
    for (int32_t i = 0; i < argumentCount; ++i)
    {
        Object* argObj = args[i].get();
        Object* arg = argObj->GetObject();
        if (arg->IsValueObject())
        {
            Value* value = static_cast<Value*>(arg);
            p::WriteValue(value);
        }
        else if (arg->IsStringObject())
        {
            StringObject* string = static_cast<StringObject*>(arg);
            std::cout << string->Value();
        }
    }
    std::cout << "\n";
}

void CreateStandardSubroutines(Node* node, SymbolTable* symbolTable)
{
    symbolTable->Root()->Block()->AddSymbol(new Ord(node, symbolTable));
    symbolTable->Root()->Block()->AddSymbol(new Chr(node, symbolTable));
    symbolTable->Root()->Block()->AddSymbol(new Sin(node, symbolTable));
    symbolTable->Root()->Block()->AddSymbol(new Cos(node, symbolTable));
    symbolTable->Root()->Block()->AddSymbol(new ArcTan(node, symbolTable));
    symbolTable->Root()->Block()->AddSymbol(new Sqrt(node, symbolTable));
    symbolTable->Root()->Block()->AddSymbol(new Abs(node, symbolTable));
    symbolTable->Root()->Block()->AddSymbol(new Succ(node, symbolTable));
    symbolTable->Root()->Block()->AddSymbol(new Pred(node, symbolTable));
    symbolTable->Root()->Block()->AddSymbol(new Min(node, symbolTable));
    symbolTable->Root()->Block()->AddSymbol(new Max(node, symbolTable));
    symbolTable->Root()->Block()->AddSymbol(new Write(node, symbolTable));
    symbolTable->Root()->Block()->AddSymbol(new Writeln(node, symbolTable));
}

} // namespace p
