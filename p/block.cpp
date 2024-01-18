// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.block;

import p.ast;
import p.binder;
import p.parsing_context;
import p.evaluator;
import p.stdfn;
import p.fnrep;
import p.writer;
import p.reader;
import p.mod;
import p.code;
import p.execute;

namespace p {

Block::Block() : 
    parent(nullptr), subroutine(nullptr), operatorFunctionsAdded(false), returnTypesSet(false), standardFunctionsAdded(false), standardProceduresAdded(false), level(0)
{
    globalTypeMap.reset(new GlobalTypeMap());
    globalConstantMap.reset(new GlobalConstantMap());
    globalVariableMap.reset(new GlobalVariableMap());
    standardProcedureRepository.reset(new StandardProcedureRepository());
    standardFunctionRepository.reset(new StandardFunctionRepository());
    p::AddStandardFunctions(this);
    p::AddStandardProcedures(this);
}

Block::Block(Block* parent_) : 
    parent(parent_), subroutine(nullptr), operatorFunctionsAdded(false), returnTypesSet(false), standardFunctionsAdded(false), standardProceduresAdded(false), level(0)
{
}

void Block::AddFundamentalTypes()
{
    AddType(new IntegerType());
    AddType(new BooleanType());
    AddType(new CharType());
    AddType(new RealType());
    AddType(new StringType());
    AddType(new PointerType());
    AddType(new NilType());
}

Type* Block::GetFundamentalType(TypeKind kind)
{
    switch (kind)
    {
        case TypeKind::booleanType: return GetType("boolean");
        case TypeKind::integerType: return GetType("integer");
        case TypeKind::charType: return GetType("char");
        case TypeKind::realType: return GetType("real");
        case TypeKind::stringType: return GetType("string");
        case TypeKind::pointerType: return GetType("pointer");
        case TypeKind::nilType: return GetType("nil_type");
    }
    throw std::runtime_error("fundamental type " + std::to_string(static_cast<int>(kind)) + " not found");
}

Type* Block::GetFundamentalType(TypeKind kind, soul::lexer::LexerBase<char>& lexer, int64_t pos) const
{
    switch (kind)
    {
        case TypeKind::booleanType: return GetType("boolean", lexer, pos);
        case TypeKind::integerType: return GetType("integer", lexer, pos);
        case TypeKind::charType: return GetType("char", lexer, pos);
        case TypeKind::realType: return GetType("real", lexer, pos);
        case TypeKind::stringType: return GetType("string", lexer, pos);
        case TypeKind::pointerType: return GetType("pointer", lexer, pos);
        case TypeKind::enumeratedType: return GetType("integer", lexer, pos);
        case TypeKind::nilType: return GetType("nil_type", lexer, pos);
    }
    ThrowError("error: fundamental type expected", lexer, pos);
    return nullptr;
}

void Block::AddOperatorFunctions()
{
    if (operatorFunctionsAdded) return;
    operatorFunctionsAdded = true;
    p::MakeOperatorFunctions(this);
}

void Block::SetSubroutine(Subroutine* subroutine_)
{ 
    subroutine = subroutine_; 
}

void Block::Write(Writer& writer)
{
    writer.GetBinaryWriter().Write(level);
    int32_t typeCount = types.size();
    writer.GetBinaryWriter().Write(typeCount);
    for (int32_t i = 0; i < typeCount; ++i)
    {
        Type* type = types[i].get();
        writer.GetBinaryWriter().Write(static_cast<uint8_t>(type->Kind()));
        type->Write(writer);
    }
    int32_t constantCount = constants.size();
    writer.GetBinaryWriter().Write(constantCount);
    for (int32_t i = 0; i < constantCount; ++i)
    {
        Constant* constant = constants[i].get();
        constant->Write(writer);
    }
    int32_t variableCount = variables.size();
    writer.GetBinaryWriter().Write(variableCount);
    for (int32_t i = 0; i < variableCount; ++i)
    {
        Variable* variable = variables[i].get();
        variable->Write(writer);
    }
    int32_t procedureCount = procedures.size();
    writer.GetBinaryWriter().Write(procedureCount);
    for (int32_t i = 0; i < procedureCount; ++i)
    {
        Procedure* procedure = procedures[i].get();
        procedure->Write(writer);
    }
    int32_t functionCount = functions.size();
    writer.GetBinaryWriter().Write(functionCount);
    for (int32_t i = 0; i < functionCount; ++i)
    {
        Function* function = functions[i].get();
        function->Write(writer);
    }
    int32_t constructorCount = constructors.size();
    writer.GetBinaryWriter().Write(constructorCount);
    for (int32_t i = 0; i < constructorCount; ++i)
    {
        Constructor* constructor = constructors[i].get();
        constructor->Write(writer);
    }
}

void Block::Read(Reader& reader)
{
    level = reader.GetBinaryReader().ReadInt();
    ModulePart* modulePart = reader.GetModulePart();
    int32_t typeCount = reader.GetBinaryReader().ReadInt();
    for (int32_t i = 0; i < typeCount; ++i)
    {
        TypeKind kind = static_cast<TypeKind>(reader.GetBinaryReader().ReadByte());
        Type* type = MakeType(kind);
        type->Read(reader);
        if (type->IsArrayType())
        {
            type->Resolve(reader.GetContext());
        }
        AddType(type);
    }
    int32_t constantCount = reader.GetBinaryReader().ReadInt();
    for (int32_t i = 0; i < constantCount; ++i)
    {
        Constant* constant = new Constant();
        constant->Read(reader);
        AddConstant(constant);
    }
    int32_t variableCount = reader.GetBinaryReader().ReadInt();
    for (int32_t i = 0; i < variableCount; ++i)
    {
        Variable* variable = new Variable();
        variable->Read(reader);
        variable->SetBlock(this);
        AddVariable(variable);
    }
    int32_t procedureCount = reader.GetBinaryReader().ReadInt();
    for (int32_t i = 0; i < procedureCount; ++i)
    {
        Procedure* procedure = new Procedure();
        procedure->Read(reader);
        procedure->ResolveHeading(reader.GetContext());
        AddProcedure(procedure);
        if (modulePart)
        {
            modulePart->AddSubroutineRead(procedure);
        }
    }
    int32_t functionCount = reader.GetBinaryReader().ReadInt();
    for (int32_t i = 0; i < functionCount; ++i)
    {
        Function* function = new Function();
        function->Read(reader);
        function->ResolveHeading(reader.GetContext());
        AddFunction(function);
        if (modulePart)
        {
            modulePart->AddSubroutineRead(function);
        }
    }
    int32_t constructorCount = reader.GetBinaryReader().ReadInt();
    for (int32_t i = 0; i < constructorCount; ++i)
    {
        Constructor* constructor = new Constructor();
        constructor->Read(reader);
        constructor->ResolveHeading(reader.GetContext());
        AddConstructor(constructor);
        if (modulePart)
        {
            modulePart->AddSubroutineRead(constructor);
        }
    }
}

void Block::Resolve(Context* context)
{
    for (const auto& type : types)
    {
        type->Resolve(context);
    }
    for (const auto& constant : constants)
    {
        constant->Resolve(context);
    }
    for (const auto& variable : variables)
    {
        variable->Resolve(context);
    }
    for (const auto& procedure : procedures)
    {
        procedure->Resolve(context);
    }
    for (const auto& function : functions)
    {
        function->Resolve(context);
    }
    for (const auto& constructor : constructors)
    {
        constructor->Resolve(context);
    }
}

Type* Block::GetType(const std::string& typeName) const
{
    auto it = typeMap.find(typeName);
    if (it != typeMap.end())
    {
        return it->second;
    }
    else if (parent)
    {
        return parent->GetType(typeName);
    }
    else
    {
        return nullptr;
    }
}

Type* Block::GetType(const std::string& typeName, soul::lexer::LexerBase<char>& lexer, int64_t pos) const
{
    auto it = typeMap.find(typeName);
    if (it != typeMap.end())
    {
        return it->second;
    }
    else if (parent)
    {
        return parent->GetType(typeName, lexer, pos);
    }
    else
    {
        ThrowError("error: type '" + typeName + "' not found", lexer, pos);
    }
    return nullptr;
}

Type* Block::GetType(const util::uuid& typeId) const
{
    GlobalTypeMap* globalTypeMap = GetGlobalTypeMap();
    if (!globalTypeMap)
    {
        throw std::runtime_error("global type map not set");
    }
    return globalTypeMap->GetType(typeId);
}

ArrayType* Block::GetArrayType(const std::string& elementTypeName, soul::lexer::LexerBase<char>& lexer, int64_t pos) const
{
    auto it = arrayTypeMap.find(elementTypeName);
    if (it != arrayTypeMap.end())
    {
        return it->second;
    }
    else if (parent)
    {
        return parent->GetArrayType(elementTypeName, lexer, pos);
    }
    else
    {
        ThrowError("error: array type for element type '" + elementTypeName + "' not found", lexer, pos);
    }
    return nullptr;
}

void Block::AddType(Type* type)
{
    auto it = typeMap.find(type->Name());
    if (it != typeMap.end())
    {
        throw std::runtime_error("type '" + type->Name() + "' not unique");
    }
    types.push_back(std::unique_ptr<Type>(type));
    typeMap[type->Name()] = type;
    if (type->IsArrayType())
    {
        ArrayType* arrayType = static_cast<ArrayType*>(type);
        if (arrayType->ElementType())
        {
            arrayTypeMap[arrayType->ElementType()->Name()] = arrayType;
        }
    }
    GlobalTypeMap* globalTypeMap = GetGlobalTypeMap();
    if (!globalTypeMap)
    {
        throw std::runtime_error("global type map not set");
    }
    globalTypeMap->AddType(type);
}

void Block::AddType(Type* type, soul::lexer::LexerBase<char>& lexer, int64_t pos)
{
    auto it = typeMap.find(type->Name());
    if (it != typeMap.end())
    {
        ThrowError("error: type '" + type->Name() + "' not unique", lexer, pos);
    }
    types.push_back(std::unique_ptr<Type>(type));
    typeMap[type->Name()] = type;
    if (type->IsArrayType())
    {
        ArrayType* arrayType = static_cast<ArrayType*>(type);
        if (arrayType->ElementType())
        {
            arrayTypeMap[arrayType->ElementType()->Name()] = arrayType;
        }
    }
    GlobalTypeMap* globalTypeMap = GetGlobalTypeMap();
    if (!globalTypeMap)
    {
        ThrowError("error: global type map not set", lexer, pos);
    }
    globalTypeMap->AddType(type);
}

Constant* Block::GetConstant(const std::string& constantName) const
{
    auto it = constantMap.find(constantName);
    if (it != constantMap.end())
    {
        return it->second;
    }
    else if (parent)
    {
        return parent->GetConstant(constantName);
    }
    else
    {
        return nullptr;
    }
}

Constant* Block::GetConstant(const std::string& constantName, soul::lexer::LexerBase<char>& lexer, int64_t pos) const
{
    auto it = constantMap.find(constantName);
    if (it != constantMap.end())
    {
        return it->second;
    }
    else if (parent)
    {
        return parent->GetConstant(constantName, lexer, pos);
    }
    else
    {
        ThrowError("error: constant '" + constantName + "' not found", lexer, pos);
    }
    return nullptr;
}

Constant* Block::GetConstant(const util::uuid& constantId) const
{
    GlobalConstantMap* globalConstantMap = GetGlobalConstantMap();
    if (!globalConstantMap)
    {
        throw std::runtime_error("global constant map not set");
    }
    return globalConstantMap->GetConstant(constantId);
}

void Block::AddConstant(Constant* constant)
{
    auto it = constantMap.find(constant->Name());
    if (it != constantMap.end())
    {
        throw std::runtime_error("constant '" + constant->Name() + "' not unique");
    }
    constants.push_back(std::unique_ptr<Constant>(constant));
    constantMap[constant->Name()] = constant;
    GlobalConstantMap* globalConstantMap = GetGlobalConstantMap();
    if (!globalConstantMap)
    {
        throw std::runtime_error("global constant map not set");
    }
    globalConstantMap->AddConstant(constant);
}

void Block::AddConstant(Constant* constant, soul::lexer::LexerBase<char>& lexer, int64_t pos)
{
    auto it = constantMap.find(constant->Name());
    if (it != constantMap.end())
    {
        ThrowError("error: constant '" + constant->Name() + "' not unique", lexer, pos);
    }
    constants.push_back(std::unique_ptr<Constant>(constant));
    constantMap[constant->Name()] = constant;
    GlobalConstantMap* globalConstantMap = GetGlobalConstantMap();
    if (!globalConstantMap)
    {
        throw std::runtime_error("global constant map not set");
    }
    globalConstantMap->AddConstant(constant);
}

Variable* Block::GetVariable(const std::string& variableName) const
{
    auto it = variableMap.find(variableName);
    if (it != variableMap.end())
    {
        return it->second;
    }
    else if (parent)
    {
        return parent->GetVariable(variableName);
    }
    else
    {
        return nullptr;
    }
}

void Block::AddVariable(Variable* variable)
{
    auto it = variableMap.find(variable->Name());
    if (it != variableMap.end())
    {
        throw std::runtime_error("variable '" + variable->Name() + "' not unique");
    }
    if (variable->Index() == -1)
    {
        int32_t parameterCount = 0;
        if (subroutine)
        {
            parameterCount = subroutine->ParameterCount();
            variable->SetIndex(parameterCount + variables.size());
        }
        else
        {
            GlobalVariableMap* globalVariableMap = GetGlobalVariableMap();
            globalVariableMap->AddVariable(variable);
        }
    }
    else if (variable->IsGlobal())
    {
        GlobalVariableMap* globalVariableMap = GetGlobalVariableMap();
        globalVariableMap->AddVariable(variable);
    }
    variables.push_back(std::unique_ptr<Variable>(variable));
    variableMap[variable->Name()] = variable;
}

void Block::AddVariable(Variable* variable, soul::lexer::LexerBase<char>& lexer, int64_t pos)
{
    auto it = variableMap.find(variable->Name());
    if (it != variableMap.end())
    {
        ThrowError("error: variable '" + variable->Name() + "' not unique", lexer, pos);
    }
    if (variable->Index() == -1)
    {
        int32_t parameterCount = 0;
        if (subroutine)
        {
            parameterCount = subroutine->ParameterCount();
            variable->SetIndex(parameterCount + variables.size());
        }
        else
        {
            GlobalVariableMap* globalVariableMap = GetGlobalVariableMap();
            globalVariableMap->AddVariable(variable);
        }
    }
    else if (variable->IsGlobal())
    {
        GlobalVariableMap* globalVariableMap = GetGlobalVariableMap();
        globalVariableMap->AddVariable(variable);
    }
    variables.push_back(std::unique_ptr<Variable>(variable));
    variableMap[variable->Name()] = variable;
}

void Block::AddParameter(Parameter* parameter)
{
    auto it = variableMap.find(parameter->Name());
    if (it != variableMap.end())
    {
        throw std::runtime_error("parameter '" + parameter->Name() + "' not unique");
    }
    variableMap[parameter->Name()] = parameter;
}

void Block::AddProcedure(Procedure* procedure)
{
    auto it = procedureMap.find(procedure->FullName());
    if (it != procedureMap.end())
    {
        Procedure* prev = it->second;
        if (!prev->IsForward() && !prev->IsDeclaration())
        {
            throw std::runtime_error("procedure '" + procedure->InfoName() + "' not unique");
        }
    }
    procedureMap[procedure->FullName()] = procedure;
    procedureMap[procedure->CommonName()] = procedure;
    if (procedure->IsRegularProcedure())
    {
        procedures.push_back(std::unique_ptr<Procedure>(procedure));
    }
}

void Block::AddProcedure(Procedure* procedure, soul::lexer::LexerBase<char>& lexer, int64_t pos)
{
    auto it = procedureMap.find(procedure->FullName());
    if (it != procedureMap.end())
    {
        Procedure* prev = it->second;
        if (!prev->IsForward() && !prev->IsDeclaration())
        {
            ThrowError("error: procedure '" + procedure->InfoName() + "' not unique", lexer, pos);
        }
        else
        {
            procedure->SetDeclaration(prev);
        }
    }
    procedureMap[procedure->FullName()] = procedure;
    procedureMap[procedure->CommonName()] = procedure;
    if (procedure->IsRegularProcedure())
    {
        procedures.push_back(std::unique_ptr<Procedure>(procedure));
    }
}

void Block::AddFunction(Function* function)
{
    auto it = functionMap.find(function->FullName());
    if (it != functionMap.end())
    {
        Function* prev = it->second;
        if (!prev->IsForward() && !prev->IsDeclaration())
        {
            throw std::runtime_error("function '" + function->InfoName() + "' not unique");
        }
    }
    functionMap[function->FullName()] = function;
    functionMap[function->CommonName()] = function;
    if (function->IsRegularFunction())
    {
        functions.push_back(std::unique_ptr<Function>(function));
    }
}

void Block::AddFunction(Function* function, soul::lexer::LexerBase<char>& lexer, int64_t pos)
{
    auto it = functionMap.find(function->FullName());
    if (it != functionMap.end())
    {
        Function* prev = it->second;
        if (!prev->IsForward() && !prev->IsDeclaration())
        {
            ThrowError("error: function '" + function->InfoName() + "' not unique", lexer, pos);
        }
        else
        {
            function->SetDeclaration(prev);
        }
    }
    functionMap[function->FullName()] = function;
    functionMap[function->CommonName()] = function;
    if (function->IsRegularFunction())
    {
        functions.push_back(std::unique_ptr<Function>(function));
    }
}

void Block::AddConstructor(Constructor* constructor)
{
    for (const auto& ctor : constructors)
    {
        int32_t n = ctor->Heading()->Parameters().size();
        if (n == constructor->Heading()->Parameters().size())
        {
            bool equal = true;
            for (int32_t i = 0; i < n; ++i)
            {
                const Parameter& param = ctor->Heading()->Parameters()[i];
                if (param.GetType() != constructor->Heading()->Parameters()[i].GetType())
                {
                    equal = false;
                    break;
                }
            }
            if (equal)
            {
                throw std::runtime_error("constructor " + std::to_string(constructors.size()) + " not unique");
            }
        }
    }
    constructors.push_back(std::unique_ptr<Constructor>(constructor));
}

void Block::AddConstructor(Constructor* constructor, soul::lexer::LexerBase<char>& lexer, int64_t pos)
{
    for (const auto& ctor : constructors)
    {
        int32_t n = ctor->Heading()->Parameters().size();
        if (n == constructor->Heading()->Parameters().size())
        {
            if (constructor->Heading()->GetObjectType() == ctor->Heading()->GetObjectType())
            {
                bool equal = true;
                for (int32_t i = 0; i < n; ++i)
                {
                    const Parameter& param = ctor->Heading()->Parameters()[i];
                    if (param.GetType() != constructor->Heading()->Parameters()[i].GetType())
                    {
                        equal = false;
                        break;
                    }
                }
                if (equal)
                {
                    ThrowError("error: constructor " + std::to_string(constructors.size()) + " not unique", lexer, pos);
                }
            }
        }
    }
    constructors.push_back(std::unique_ptr<Constructor>(constructor));
}

Procedure* Block::GetProcedure(const std::string& procedureName) const
{
    auto it = procedureMap.find(procedureName);
    if (it != procedureMap.end())
    {
        return it->second;
    }
    else if (parent)
    {
        return parent->GetProcedure(procedureName);
    }
    else
    {
        return nullptr;
    }
}

Function* Block::GetFunction(const std::string& functionName) const
{
    auto it = functionMap.find(functionName);
    if (it != functionMap.end())
    {
        return it->second;
    }
    else if (parent)
    {
        return parent->GetFunction(functionName);
    }
    else
    {
        return nullptr;
    }
}

GlobalTypeMap* Block::GetGlobalTypeMap() const
{
    if (globalTypeMap)
    {
        return globalTypeMap.get();
    }
    else  if (parent)
    {
        return parent->GetGlobalTypeMap();
    }
    else
    {
        return nullptr;
    }
}

GlobalConstantMap* Block::GetGlobalConstantMap() const
{
    if (globalConstantMap)
    {
        return globalConstantMap.get();
    }
    else  if (parent)
    {
        return parent->GetGlobalConstantMap();
    }
    else
    {
        return nullptr;
    }
}

GlobalVariableMap* Block::GetGlobalVariableMap() const
{
    if (globalVariableMap)
    {
        return globalVariableMap.get();
    }
    else if (parent)
    {
        return parent->GetGlobalVariableMap();
    }
    else
    {
        return nullptr;
    }
}

StandardProcedureRepository* Block::GetStandardProcedureRepository() const
{
    if (standardProcedureRepository)
    {
        return standardProcedureRepository.get();
    }
    else if (parent)
    {
        return parent->GetStandardProcedureRepository();
    }
    else
    {
        return nullptr;
    }
}

StandardFunctionRepository* Block::GetStandardFunctionRepository() const
{
    if (standardFunctionRepository)
    {
        return standardFunctionRepository.get();
    }
    else if (parent)
    {
        return parent->GetStandardFunctionRepository();
    }
    else
    {
        return nullptr;
    }
}

void Block::ImportModules(ParsingContext* context)
{
    ModulePart* modulePart = context->GetModulePart();
    if (modulePart->IsImplementationPart())
    {
        Import(modulePart->GetModule());
    }
    for (auto importedModule : modulePart->ImportedModules())
    {
        Import(importedModule);
    }
}

void Block::Import(Module* mod)
{
    if (importedModules.find(mod) != importedModules.end()) return;
    importedModules.insert(mod);
    if (mod->RootBlock())
    {
        Import(mod->RootBlock(), mod);
    }
    ModulePart* interfacePart = mod->GetInterfacePart();
    if (interfacePart)
    {
        Block* blk = interfacePart->GetBlock();
        if (blk)
        {
            Import(blk, mod);
        }
        for (auto importedModule : interfacePart->ImportedModules())
        {
            Import(importedModule);
        }
    }
}

void Block::Import(Block* blk, Module* mod)
{
    for (const auto& type : blk->types)
    {
        auto it = typeMap.find(type->Name());
        if (it != typeMap.end())
        {
            throw std::runtime_error("error importing module '" + mod->Name() + "': type name '" + type->Name() + " not unique");
        }
        typeMap[type->Name()] = type.get();
        GlobalTypeMap* globalTypeMap = GetGlobalTypeMap();
        if (!globalTypeMap)
        {
            throw std::runtime_error("global type map not set");
        }
        globalTypeMap->AddType(type.get());
    }
    for (const auto& arrayType : blk->arrayTypeMap)
    {
        arrayTypeMap.insert(arrayType);
    }
    for (const auto& constant : blk->constants)
    {
        auto it = constantMap.find(constant->Name());
        if (it != constantMap.end())
        {
            throw std::runtime_error("error importing module '" + mod->Name() + "': constant name '" + constant->Name() + " not unique");
        }
        constantMap[constant->Name()] = constant.get();
        GlobalConstantMap* globalConstantMap = GetGlobalConstantMap();
        if (!globalConstantMap)
        {
            throw std::runtime_error("global constant map not set");
        }
        globalConstantMap->AddConstant(constant.get());
    }
    for (const auto& variable : blk->variables)
    {
        auto it = variableMap.find(variable->Name());
        if (it != variableMap.end())
        {
            throw std::runtime_error("error importing module '" + mod->Name() + "': variable name '" + variable->Name() + " not unique");
        }
        variableMap[variable->Name()] = variable.get();
    }
    for (const auto& procedure : blk->procedures)
    {
        auto it = procedureMap.find(procedure->FullName());
        if (it != procedureMap.end())
        {
            throw std::runtime_error("error importing module '" + mod->Name() + "': procedure name '" + procedure->InfoName() + " not unique");
        }
        procedureMap[procedure->FullName()] = procedure.get();
    }
    for (const auto& function : blk->functions)
    {
        auto it = functionMap.find(function->FullName());
        if (it != functionMap.end())
        {
            throw std::runtime_error("error importing module '" + mod->Name() + "': function name '" + function->InfoName() + " not unique");
        }
        functionMap[function->FullName()] = function.get();
    }
}

void Block::Print(util::CodeFormatter& formatter, ExecutionContext* context)
{
    if (!types.empty())
    {
        formatter.WriteLine("type");
        formatter.IncIndent();
    }
    for (const auto& type : types)
    {
        type->Print(formatter, context);
    }
    if (!types.empty())
    {
        formatter.DecIndent();
    }
    if (!constants.empty())
    {
        formatter.WriteLine("const");
        formatter.IncIndent();
    }
    for (const auto& constant : constants)
    {
        constant->Print(formatter);
    }
    if (!constants.empty())
    {
        formatter.DecIndent();
    }
    if (!variables.empty())
    {
        formatter.WriteLine("var");
        formatter.IncIndent();
    }
    for (const auto& variable : variables)
    {
        variable->Print(formatter);
    }
    if (!variables.empty())
    {
        formatter.DecIndent();
    }
    for (const auto& procedure : procedures)
    {
        try
        {
            procedure->Print(formatter, context);
        }
        catch (const std::exception& ex)
        {
            throw std::runtime_error("procedure '" + procedure->InfoName() + "': " + ex.what());
        }
    }
    for (const auto& function : functions)
    {
        try
        {
            function->Print(formatter, context);
        }
        catch (const std::exception& ex)
        {
            throw std::runtime_error("function '" + function->InfoName() + "': " + ex.what());
        }
    }
    for (const auto& constructor : constructors)
    {
        try
        {
            constructor->Print(formatter, context);
        }
        catch (const std::exception& ex)
        {
            throw std::runtime_error(std::string("constructor: ") + ex.what());
        }
    }
}

void Block::CheckInterface()
{
    for (const auto& procedure : procedures)
    {
        procedure->CheckInterface();
    }
    for (const auto& function : functions)
    {
        function->CheckInterface();
    }
    for (const auto& constructor : constructors)
    {
        constructor->CheckInterface();
    }
    for (const auto& type : types)
    {
        type->CheckInterface();
    }
}

void Block::MakeVmts(Context* context)
{
    for (const auto& procedure : procedures)
    {
        procedure->MakeVmts(context);
    }
    for (const auto& function : functions)
    {
        function->MakeVmts(context);
    }
    for (const auto& constructor : constructors)
    {
        constructor->MakeVmts(context);
    }
    for (const auto& type : types)
    {
        type->MakeVmt(context);
    }
}

void MakeVariables(ParsingContext* context, const std::vector<std::string>& variableNames, Type* type, soul::lexer::LexerBase<char>& lexer, int64_t pos)
{
    Block* block = context->GetBlock();
    for (const auto& variableName : variableNames)
    {
        Variable* variable = new Variable(variableName);
        variable->SetType(type);
        variable->SetBlock(block);
        block->AddVariable(variable, lexer, pos);
    }
}

void AddSimpleConstant(ParsingContext* context, Node* constantExpression, Type* type, soul::lexer::LexerBase<char>& lexer, int64_t pos)
{
    if (context->MakeConstant())
    {
        Block* block = context->GetBlock();
        Value* value = Evaluate(constantExpression, lexer, block);
        Constant* constant = new Constant(context->ConstantName());
        constant->SetValue(value);
        constant->SetType(type);
        block->AddConstant(constant, lexer, pos);
    }
    else
    {
        Block* block = context->GetBlock();
        Value* value = Evaluate(constantExpression, lexer, block);
        Value* currentValue = context->GetCurrentValue();
        if (currentValue->IsArrayValue())
        {
            ArrayValue* arrayValue = static_cast<ArrayValue*>(currentValue);
            arrayValue->AddElement(value);
        }
        else if (currentValue->IsObjectValue())
        {
            ObjectValue* objectValue = static_cast<ObjectValue*>(currentValue);
            objectValue->AddField(Field(objectValue->Fields().size(), context->FieldName(), value));
        }
        else
        {
            ThrowError("error: array or object constant expected", lexer, pos);
        }
    }
}

void MakeArrayConstant(ParsingContext* context, Type* type, Type*& elementType, soul::lexer::LexerBase<char>& lexer, int64_t pos)
{
    if (context->MakeConstant())
    {
        if (type->IsArrayType())
        {
            ArrayType* arrayType = static_cast<ArrayType*>(type);
            elementType = arrayType->ElementType();
            Block* block = context->GetBlock();
            Constant* constant = new Constant(context->ConstantName());
            ArrayValue* arrayValue = new ArrayValue();
            constant->SetValue(arrayValue);
            constant->SetType(type);
            block->AddConstant(constant, lexer, pos);
            context->PushFlags(Flags::addToConstant);
            context->PushCurrentValue(arrayValue);
        }
        else
        {
            ThrowError("error: array type expected", lexer, pos);
        }
    }
    else if (context->AddToConstant())
    {
        ArrayValue* value = new ArrayValue();
        Value* currentValue = context->GetCurrentValue();
        if (currentValue->IsArrayValue())
        {
            ArrayValue* arrayValue = static_cast<ArrayValue*>(currentValue);
            arrayValue->AddElement(value);
        }
        else if (currentValue->IsObjectValue())
        {
            ObjectValue* objectValue = static_cast<ObjectValue*>(currentValue);
            objectValue->AddField(Field(objectValue->Fields().size(), context->FieldName(), value));
        }
        else
        {
            ThrowError("error: array or object constant expected", lexer, pos);
        }
        context->PushCurrentValue(value);
    }
}

void MakeObjectConstant(ParsingContext* context, Type* type, soul::lexer::LexerBase<char>& lexer, int64_t pos)
{
    if (context->MakeConstant())
    {
        if (type->IsObjectType())
        {
            ObjectType* objectType = static_cast<ObjectType*>(type);
            Block* block = context->GetBlock();
            Constant* constant = new Constant(context->ConstantName());
            ObjectValue* objectValue = new ObjectValue();
            constant->SetValue(objectValue);
            constant->SetType(type);
            block->AddConstant(constant, lexer, pos);
            context->PushFlags(Flags::addToConstant);
            context->PushCurrentValue(objectValue);
        }
        else
        {
            ThrowError("error: object type expected", lexer, pos);
        }
    }
    else if (context->AddToConstant())
    {
        ObjectValue* value = new ObjectValue();
        Value* currentValue = context->GetCurrentValue();
        if (currentValue->IsArrayValue())
        {
            ArrayValue* arrayValue = static_cast<ArrayValue*>(currentValue);
            arrayValue->AddElement(value);
        }
        else if (currentValue->IsObjectValue())
        {
            ObjectValue* objectValue = static_cast<ObjectValue*>(currentValue);
            objectValue->AddField(Field(objectValue->Fields().size(), context->FieldName(), value));
        }
        else
        {
            ThrowError("error: array or object constant expected", lexer, pos);
        }
        context->PushCurrentValue(value);
    }
}

Type* GetFieldType(Type* type, const std::string& fieldName, soul::lexer::LexerBase<char>& lexer, int64_t pos)
{
    if (type->IsObjectType())
    {
        ObjectType* objectType = static_cast<ObjectType*>(type);
        Type* fieldType = objectType->GetFieldType(fieldName, false);
        if (fieldType)
        {
            return fieldType;
        }
        else
        { 
            ThrowError("error: field '" + fieldName + "' not found", lexer, pos);
        }
    }
    else
    {
        ThrowError("error: object type expected", lexer, pos);
    }
    return nullptr;
}

void CompileStatementPart(ParsingContext* context, CompoundStatementNode* compoundStatement, soul::lexer::LexerBase<char>& lexer)
{
    Block* block = context->GetBlock();
    if ((context->GetFlags() & Flags::initializationPart) != Flags::none)
    {
        context->GetModule()->MakeInitBlock(block);
        context->PushBlock(context->GetModule()->InitBlock());
        block = context->GetBlock();
        context->GetModule()->SetInit(context, lexer, compoundStatement->Pos());
        block->SetSubroutine(context->GetModule()->InitProc());
    }
    Subroutine* subroutine = block->GetSubroutine();
    subroutine->SetParamIndeces();
    if (subroutine->IsFunction())
    {
        Function* function = static_cast<Function*>(subroutine);
        Variable* resultVar = new Variable("@result");
        std::vector<Type*> argumentTypes;
        resultVar->SetType(function->ResultType(argumentTypes));
        block->AddVariable(resultVar);
    }
    std::unique_ptr<BoundCompoundStatementNode> boundCompoundStatement = Bind(context, compoundStatement, subroutine, lexer);
    subroutine->SetFrameSize(context);
    GenerateCode(subroutine, block, boundCompoundStatement.get(), lexer, boundCompoundStatement->Pos(), context);
    if ((context->GetFlags() & Flags::initializationPart) != Flags::none)
    {
        context->PopBlock();
    }
}

void NoSubroutineBlock(soul::lexer::LexerBase<char>& lexer, int64_t pos)
{
    lexer.ThrowFarthestError();
}

void GenerateImplementationIds(ParsingContext* context, soul::lexer::LexerBase<char>& lexer, int64_t pos)
{
    Module* mod = context->GetModule();
    std::vector<ObjectType*> objectTypes;
    ModulePart* interfacePart = mod->GetInterfacePart();
    if (interfacePart)
    {
        for (ObjectType* objectType : interfacePart->ObjectTypes())
        {
            objectTypes.push_back(objectType);
        }
    }
    ModulePart* implementationPart = mod->GetImplementationPart();
    if (implementationPart)
    {
        for (ObjectType* objectType : implementationPart->ObjectTypes())
        {
            objectTypes.push_back(objectType);
        }
    }
    for (ObjectType* objectType : objectTypes)
    {
        Constructor* defaultCtor = objectType->GetDefaultConstructor();
        if (defaultCtor)
        {
            if (defaultCtor->IsGenerated() && !defaultCtor->IsImplementationGenerated())
            {
                defaultCtor->SetImplementationGenerated();
                int32_t moduleId = mod->Id();
                Constructor* ctorImplementation = MakeConstructor(context, lexer, pos);
                ctorImplementation->Heading()->SetObjectType(objectType);
                ctorImplementation->Heading()->InsertThisParam();
                ctorImplementation->SetFrameSize(context);
                defaultCtor->SetModuleId(moduleId);
                defaultCtor->SetImplementationId(ctorImplementation->Id());
                defaultCtor->SetImplementation(ctorImplementation);
                defaultCtor->SetFrameSize(context);
            }
        }
    }
}

void GenerateDefaultImplementations(ParsingContext* context, soul::lexer::LexerBase<char>& lexer, int64_t pos)
{
    GenerateImplementationIds(context, lexer, pos);
    Module* mod = context->GetModule();
    std::vector<ObjectType*> objectTypes;
    ModulePart* interfacePart = mod->GetInterfacePart();
    if (interfacePart)
    {
        for (ObjectType* objectType : interfacePart->ObjectTypes())
        {
            objectTypes.push_back(objectType);
        }
    }
    ModulePart* implementationPart = mod->GetImplementationPart();
    if (implementationPart)
    {
        for (ObjectType* objectType : implementationPart->ObjectTypes())
        {
            objectTypes.push_back(objectType);
        }
    }
    for (ObjectType* objectType : objectTypes)
    {
        Constructor* defaultCtor = objectType->GetDefaultConstructor();
        if (defaultCtor)
        {
            if (defaultCtor->IsGenerated())
            {
                Constructor* implementation = defaultCtor->GetImplementation();
                if (!implementation)
                {
                    ThrowError("error: implementation not generated for generated constructor", lexer, pos);
                }
                BoundCompoundStatementNode compoundStatement(pos);
                if (objectType->BaseType())
                {
                    Constructor* baseConstructor = objectType->BaseType()->GetDefaultConstructor();
                    if (!baseConstructor)
                    {
                        ThrowError("error: base type '" + objectType->BaseType()->Name() + "' has no default constructor", lexer, pos);
                    }
                    BoundConstructorCallNode* boundConstructorCall = new BoundConstructorCallNode(baseConstructor, pos);
                    boundConstructorCall->AddArgument(new BoundParameterNode(pos, implementation->Heading()->ThisParam(), objectType->BaseType()));
                    BoundExpressionStatementNode* boundExpressionStatementNode(new BoundExpressionStatementNode(boundConstructorCall, pos));
                    boundExpressionStatementNode->DontPop();
                    compoundStatement.AddStatement(boundExpressionStatementNode);
                }
                GenerateCode(implementation, context->GetBlock(), &compoundStatement, lexer, pos, context);
            }
        }
    }
}

} // namespace p
