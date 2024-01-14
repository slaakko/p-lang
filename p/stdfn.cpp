// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.stdfn;

import p.ast;
import p.value;
import p.block;
import p.execute;
import p.evaluator;
import p.type;

namespace p {

StandardFunction::StandardFunction(const std::string& fullName_) : Function(FunctionKind::standardfn, fullName_), id(-1)
{
}

StandardProcedure::StandardProcedure(const std::string& fullName_) : Procedure(ProcedureKind::standardproc, fullName_), id(-1)
{
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

Ord::Ord() : StandardFunction("Ord")
{
}

Value* Ord::Evaluate(const std::vector<std::unique_ptr<Value>>& argumentValues, soul::lexer::LexerBase<char>& lexer, int64_t pos)
{
    if (argumentValues.size() != 1)
    {
        ThrowError("error: 'Ord' function takes one argument, " + std::to_string(argumentValues.size()) + " arguments supplied", lexer, pos);
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
            ThrowError("error: invalid 'Ord' function argument type", lexer, pos);
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

Chr::Chr() : StandardFunction("Chr")
{
}

Value* Chr::Evaluate(const std::vector<std::unique_ptr<Value>>& argumentValues, soul::lexer::LexerBase<char>& lexer, int64_t pos)
{
    if (argumentValues.size() != 1)
    {
        ThrowError("error: 'Chr' function takes one argument, " + std::to_string(argumentValues.size()) + " arguments supplied", lexer, pos);
    }
    Value* value = argumentValues[0]->GetValue();
    switch (value->Kind())
    {
        case ValueKind::integerValue:
        {
            int32_t intval = value->ToInteger();
            if (intval < 0 || intval > 255)
            {
                ThrowError("error: invalid 'Chr' function argument", lexer, pos);
            }
            return new CharValue(static_cast<char>(intval));
        }
        default:
        {
            ThrowError("error: invalid 'Chr' function argument type", lexer, pos);
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

Sin::Sin() : StandardFunction("Sin")
{
}

void Sin::Execute(ExecutionContext* context)
{
    std::unique_ptr<Object> arg = context->GetStack()->Pop();
    std::unique_ptr<Object> result(new RealValue(std::sin(arg->GetObject()->ToReal())));
    context->GetStack()->Push(result.release());
}

Cos::Cos() : StandardFunction("Cos")
{
}

void Cos::Execute(ExecutionContext* context)
{
    std::unique_ptr<Object> arg = context->GetStack()->Pop();
    std::unique_ptr<Object> result(new RealValue(std::cos(arg->GetObject()->ToReal())));
    context->GetStack()->Push(result.release());
}

ArcTan::ArcTan() : StandardFunction("ArcTan")
{
}

void ArcTan::Execute(ExecutionContext* context)
{
    std::unique_ptr<Object> arg = context->GetStack()->Pop();
    std::unique_ptr<Object> result(new RealValue(std::atan(arg->GetObject()->ToReal())));
    context->GetStack()->Push(result.release());
}

Abs::Abs() : StandardFunction("Abs")
{
}

Type* Abs::ResultType(const std::vector<Type*>& argumentTypes) const
{
    if (argumentTypes.empty())
    {
        throw std::runtime_error("argument types empty");
    }
    return argumentTypes.front();
}

Value* Abs::Evaluate(const std::vector<std::unique_ptr<Value>>& argumentValues, soul::lexer::LexerBase<char>& lexer, int64_t pos)
{
    if (argumentValues.size() != 1)
    {
        ThrowError("error: 'Abs' function takes one argument, " + std::to_string(argumentValues.size()) + " arguments supplied", lexer, pos);
    }
    Value* value = argumentValues[0]->GetValue();
    switch (value->Kind())
    {
        case ValueKind::integerValue:
        {
            return new IntegerValue(std::abs(value->ToInteger()));
        }
        case ValueKind::realValue:
        {
            return new RealValue(std::fabs(value->ToReal()));
        }
        default:
        {
            ThrowError("error: invalid 'Abs' function argument type", lexer, pos);
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
                throw std::runtime_error("Abs: integer or real object expected");
            }
        }
    }
    else
    {
        throw std::runtime_error("Abs: value object expected");
    }
    context->GetStack()->Push(result.release());
}

Succ::Succ() : StandardFunction("Succ")
{
}

Type* Succ::ResultType(const std::vector<Type*>& argumentTypes) const
{
    if (argumentTypes.empty())
    {
        throw std::runtime_error("argument types empty");
    }
    return argumentTypes.front();
}

Value* Succ::Evaluate(const std::vector<std::unique_ptr<Value>>& argumentValues, soul::lexer::LexerBase<char>& lexer, int64_t pos)
{
    if (argumentValues.size() != 1)
    {
        ThrowError("error: 'Succ' function takes one argument, " + std::to_string(argumentValues.size()) + " arguments supplied", lexer, pos);
    }
    Value* value = argumentValues[0]->GetValue();
    return value->Succ();
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
        throw std::runtime_error("Succ: value object expected");
    }
}

Pred::Pred() : StandardFunction("Pred")
{
}

Type* Pred::ResultType(const std::vector<Type*>& argumentTypes) const
{
    if (argumentTypes.empty())
    {
        throw std::runtime_error("argument types empty");
    }
    return argumentTypes.front();
}

Value* Pred::Evaluate(const std::vector<std::unique_ptr<Value>>& argumentValues, soul::lexer::LexerBase<char>& lexer, int64_t pos)
{
    if (argumentValues.size() != 1)
    {
        ThrowError("error: 'Pred' function takes one argument, " + std::to_string(argumentValues.size()) + " arguments supplied", lexer, pos);
    }
    Value* value = argumentValues[0]->GetValue();
    return value->Pred();
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
        throw std::runtime_error("Pred: value object expected");
    }
}

Min::Min() : StandardFunction("Min")
{
}

Type* Min::ResultType(const std::vector<Type*>& argumentTypes) const
{
    if (argumentTypes.size() != 2)
    {
        throw std::runtime_error("Min: two argument types expected");
    }
    if (argumentTypes[0]->IsIntegerType() && argumentTypes[1]->IsIntegerType())
    {
        return argumentTypes[0];
    }
    else if (argumentTypes[0]->IsRealType())
    {
        return argumentTypes[0];
    }
    else if (argumentTypes[1]->IsRealType())
    {
        return argumentTypes[1];
    }
    else
    {
        return argumentTypes[0];
    }
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
                throw std::runtime_error("Min: integer or real objects expected");
            }
        }
    }
    else
    {
        throw std::runtime_error("Min: value objects expected");
    }
    context->GetStack()->Push(result.release());
}

Max::Max() : StandardFunction("Max")
{
}

Type* Max::ResultType(const std::vector<Type*>& argumentTypes) const
{
    if (argumentTypes.size() != 2)
    {
        throw std::runtime_error("Min: two argument types expected");
    }
    if (argumentTypes[0]->IsIntegerType() && argumentTypes[1]->IsIntegerType())
    {
        return argumentTypes[0];
    }
    else if (argumentTypes[0]->IsRealType())
    {
        return argumentTypes[0];
    }
    else if (argumentTypes[1]->IsRealType())
    {
        return argumentTypes[1];
    }
    else
    {
        return argumentTypes[0];
    }
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
                throw std::runtime_error("Max: integer or real objects expected");
            }
        }
    }
    else
    {
        throw std::runtime_error("Max: value objects expected");
    }
    context->GetStack()->Push(result.release());
}

Sqrt::Sqrt() : StandardFunction("Sqrt")
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
        throw std::runtime_error("Sqrt: value object expected");
    }
}

Write::Write() : StandardProcedure("Write")
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

Writeln::Writeln() : StandardProcedure("Writeln")
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

class StandardFunctionRepository
{
public:
    StandardFunctionRepository();
    static StandardFunctionRepository& Instance();
    void AddStandardFunction(StandardFunction* standardFunction);
    StandardFunction* GetStandardFunction(int32_t id) const;
    StandardFunction* GetStandardFunction(const std::string& fullName) const;
private:
    std::vector<std::unique_ptr<StandardFunction>> standardFunctions;
    std::map<std::string, StandardFunction*> standardFunctionMap;
};

StandardFunctionRepository::StandardFunctionRepository()
{
}

StandardFunctionRepository& StandardFunctionRepository::Instance()
{
    static StandardFunctionRepository instance;
    return instance;
}

void StandardFunctionRepository::AddStandardFunction(StandardFunction* standardFunction)
{
    standardFunction->SetId(standardFunctions.size());
    standardFunctions.push_back(std::unique_ptr<StandardFunction>(standardFunction));
    standardFunctionMap[standardFunction->FullName()] = standardFunction;
}

StandardFunction* StandardFunctionRepository::GetStandardFunction(int32_t id) const
{
    if (id >= 0 && id < standardFunctions.size())
    {
        return standardFunctions[id].get();
    }
    else
    {
        throw std::runtime_error("invalid standard function id " + std::to_string(id));
    }
}

StandardFunction* StandardFunctionRepository::GetStandardFunction(const std::string& fullName) const
{
    auto it = standardFunctionMap.find(fullName);
    if (it != standardFunctionMap.end())
    {
        return it->second;
    }
    else
    {
        throw std::runtime_error("invalid standard function name '" + fullName + "'");
    }
}

void AddStandardFunctions(Block* block)
{
    Ord* ord = new Ord();
    StandardFunctionRepository::Instance().AddStandardFunction(ord);
    block->AddFunction(ord);
    Chr* chr = new Chr();
    StandardFunctionRepository::Instance().AddStandardFunction(chr);
    block->AddFunction(chr);
    Sin* sin = new Sin();
    StandardFunctionRepository::Instance().AddStandardFunction(sin);
    block->AddFunction(sin);
    Cos* cos = new Cos();
    StandardFunctionRepository::Instance().AddStandardFunction(cos);
    block->AddFunction(cos);
    ArcTan* arcTan = new ArcTan();
    StandardFunctionRepository::Instance().AddStandardFunction(arcTan);
    block->AddFunction(arcTan);
    Abs* abs = new Abs();
    StandardFunctionRepository::Instance().AddStandardFunction(abs);
    block->AddFunction(abs);
    Succ* succ = new Succ();
    StandardFunctionRepository::Instance().AddStandardFunction(succ);
    block->AddFunction(succ);
    Pred* pred= new Pred();
    StandardFunctionRepository::Instance().AddStandardFunction(pred);
    block->AddFunction(pred);
    Min* min = new Min();
    StandardFunctionRepository::Instance().AddStandardFunction(min);
    block->AddFunction(min);
    Max* max = new Max();
    StandardFunctionRepository::Instance().AddStandardFunction(max);
    block->AddFunction(max);
    Sqrt* sqrt = new Sqrt();
    StandardFunctionRepository::Instance().AddStandardFunction(sqrt);
    block->AddFunction(sqrt);
}

void SetStandardFunctionReturnTypes(Block* block)
{
    if (block->ReturnTypesSet()) return;
    block->SetReturnTypesSet();
    Function* ord = StandardFunctionRepository::Instance().GetStandardFunction("Ord");
    ord->GetFunctionHeading()->SetResultType(block->GetFundamentalType(TypeKind::integerType));
    Function* chr = StandardFunctionRepository::Instance().GetStandardFunction("Chr");
    chr->GetFunctionHeading()->SetResultType(block->GetFundamentalType(TypeKind::charType));
    Function* sin = StandardFunctionRepository::Instance().GetStandardFunction("Sin");
    sin->GetFunctionHeading()->SetResultType(block->GetFundamentalType(TypeKind::realType));
    Function* cos = StandardFunctionRepository::Instance().GetStandardFunction("Cos");
    cos->GetFunctionHeading()->SetResultType(block->GetFundamentalType(TypeKind::realType));
    Function* arcTan = StandardFunctionRepository::Instance().GetStandardFunction("ArcTan");
    arcTan->GetFunctionHeading()->SetResultType(block->GetFundamentalType(TypeKind::realType));
    Function* sqrt = StandardFunctionRepository::Instance().GetStandardFunction("Sqrt");
    sqrt->GetFunctionHeading()->SetResultType(block->GetFundamentalType(TypeKind::realType));
}

StandardFunction* GetStandardFunction(int32_t standardFunctionId)
{
    return StandardFunctionRepository::Instance().GetStandardFunction(standardFunctionId);
}

class StandardProcedureRepository
{
public:
    StandardProcedureRepository();
    static StandardProcedureRepository& Instance();
    void AddStandardProcedure(StandardProcedure* standardProcedure);
    StandardProcedure* GetStandardProcedure(int32_t id) const;
private:
    std::vector<std::unique_ptr<StandardProcedure>> standardProcedures;
};

StandardProcedureRepository::StandardProcedureRepository()
{
}

StandardProcedureRepository& StandardProcedureRepository::Instance()
{
    static StandardProcedureRepository instance;
    return instance;
}

void StandardProcedureRepository::AddStandardProcedure(StandardProcedure* standardProcedure)
{
    standardProcedure->SetId(standardProcedures.size());
    standardProcedures.push_back(std::unique_ptr<StandardProcedure>(standardProcedure));
}

StandardProcedure* StandardProcedureRepository::GetStandardProcedure(int32_t id) const
{
    if (id >= 0 && id < standardProcedures.size())
    {
        return standardProcedures[id].get();
    }
    else
    {
        throw std::runtime_error("invalid standard procedure id " + std::to_string(id));
    }
}


void AddStandardProcedures(Block* block)
{
    Write* write = new Write();
    StandardProcedureRepository::Instance().AddStandardProcedure(write);
    block->AddProcedure(write);
    Writeln* writeln = new Writeln();
    StandardProcedureRepository::Instance().AddStandardProcedure(writeln);
    block->AddProcedure(writeln);
}

StandardProcedure* GetStandardProcedure(int32_t standardProcedureId)
{
    return StandardProcedureRepository::Instance().GetStandardProcedure(standardProcedureId);
}

} // namespace p
