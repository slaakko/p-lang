    // =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.core.container_symbol;

import p.ast;
import p.core.writer;
import p.core.reader;
import p.core.type_symbol;
import p.core.object_type_symbol;
import p.core.array_type_symbol;
import p.core.constant_symbol;
import p.core.subroutine_symbol;
import p.core.variable_symbol;
import p.core.value;
import p.core.symbol_table;
import p.core.execution_context;
import p.core.context;

namespace p {

ContainerSymbol::ContainerSymbol(SymbolKind kind_, const soul::ast::Span& span_, const std::string& name_) : 
    Symbol(kind_, span_, name_), constructorGroup(nullptr), globalVariableMap(nullptr)
{
}

ContainerSymbol::~ContainerSymbol()
{
    if (globalVariableMap)
    {
        delete globalVariableMap;
        globalVariableMap = nullptr;
    }
}

void ContainerSymbol::AddSymbol(Symbol* symbol)
{
    symbol->SetParent(this);
    symbols.push_back(std::unique_ptr<Symbol>(symbol));
    MapSymbol(symbol);
    if (symbol->IsTypeSymbol())
    {
        TypeSymbol* type = static_cast<TypeSymbol*>(symbol);
        SymbolTable* symbolTable = GetSymbolTable();
        symbolTable->MapType(type);
        if (type->IsArrayTypeSymbol())
        {
            ArrayTypeSymbol* arrayType = static_cast<ArrayTypeSymbol*>(type);
            MapArrayTypeSymbol(arrayType);
        }
    }
    else if (symbol->IsConstantSymbol())
    {
        ConstantSymbol* constant = static_cast<ConstantSymbol*>(symbol);
        SymbolTable* symbolTable = GetSymbolTable();
        symbolTable->MapConstant(constant);
    }
    else if (symbol->IsConstructorGroupSymbol())
    {
        constructorGroup = static_cast<ConstructorGroupSymbol*>(symbol);
    }
    else if (symbol->IsSubroutineSymbol())
    {
        SubroutineSymbol* subroutineSymbol = static_cast<SubroutineSymbol*>(symbol);
        SymbolTable* symbolTable = GetSymbolTable();
        symbolTable->MapSubroutine(subroutineSymbol);
    }
}

void ContainerSymbol::Resolve()
{
    Symbol::Resolve();
    for (const auto& arrayType : arrayTypesToMap)
    {
        MapArrayTypeSymbol(arrayType);
    }
    arrayTypesToMap.clear();
}

void ContainerSymbol::Import(ContainerSymbol* that)
{
    for (const auto& nameSymbol : that->symbolMap)
    {
        Symbol* symbol = nameSymbol.second;
        MapSymbol(symbol);
    }
    for (const auto& elementNameArrayTypeSymbol : that->arrayTypeSymbolMap)
    {
        ArrayTypeSymbol* arrayTypeSymbol = elementNameArrayTypeSymbol.second;
        MapArrayTypeSymbol(arrayTypeSymbol);
    }
}

void ContainerSymbol::MapSymbol(Symbol* symbol)
{
    auto it = symbolMap.find(symbol->Name());
    if (it == symbolMap.end())
    {
        symbolMap[symbol->Name()] = symbol;
    }
    else if (it->second != symbol)
    { 
        ThrowError("error: symbol '" + symbol->Name() + "' not unique", SourceFilePath(), Span());
    }
}

void ContainerSymbol::MapArrayTypeSymbol(ArrayTypeSymbol* arrayTypeSymbol)
{
    if (!arrayTypeSymbol->ElementType())
    {
        arrayTypesToMap.insert(arrayTypeSymbol);
    }
    else
    {
        auto it = arrayTypeSymbolMap.find(arrayTypeSymbol->ElementType()->Name());
        if (it == arrayTypeSymbolMap.end())
        {
            arrayTypeSymbolMap[arrayTypeSymbol->ElementType()->Name()] = arrayTypeSymbol;
        }
    }
}

Symbol* ContainerSymbol::GetSymbol(const std::string& symbolName, Node* node, Context* context) const
{
    if (context->NoThrow())
    {
        return GetSymbol(symbolName, node, false, false);
    }
    else
    {
        return GetSymbol(symbolName, node, false, true);
    }
}

Symbol* ContainerSymbol::GetSymbol(const std::string& symbolName, Node* node, bool searchBase, bool mustExist) const
{
    auto it = symbolMap.find(symbolName);
    if (it != symbolMap.end())
    {
        return it->second;
    }
    else 
    {
        ContainerSymbol* parent = Parent();
        if (parent)
        {
            return parent->GetSymbol(symbolName, node, searchBase, mustExist);
        }
        else
        {
            if (mustExist)
            {
                ThrowError("error: symbol '" + symbolName + "' not found", node->FilePath(), node->Span());
            }
        }
    }
    return nullptr;
}

TypeSymbol* ContainerSymbol::GetType(const std::string& typeName, Node* node, Context* context) const
{
    Symbol* symbol = GetSymbol(typeName, node, context);
    if (symbol && symbol->IsTypeSymbol())
    {
        return static_cast<TypeSymbol*>(symbol);
    }
    else
    {
        if (!context->NoThrow())
        {
            ThrowError("error: type symbol '" + typeName + "' expected", node->FilePath(), node->Span());
        }
    }
    return nullptr;
}

ArrayTypeSymbol* ContainerSymbol::GetArrayType(const std::string& elementTypeName) const
{
    auto it = arrayTypeSymbolMap.find(elementTypeName);
    if (it != arrayTypeSymbolMap.end())
    {
        return it->second;
    }
    else
    {
        ContainerSymbol* parent = Parent();
        if (parent)
        {
            return parent->GetArrayType(elementTypeName);
        }
        else
        {
            return nullptr;
        }
    }
}

VariableSymbol* ContainerSymbol::GetVariable(const std::string& variableName, Node* node, Context* context) const
{
    Symbol* symbol = GetSymbol(variableName, node, context);
    if (symbol && symbol->IsVariableSymbol())
    {
        return static_cast<VariableSymbol*>(symbol);
    }
    else
    {
        if (!context->NoThrow())
        {
            ThrowError("error: variable symbol '" + variableName + "' expected", node->FilePath(), node->Span());
        }
    }
    return nullptr;
}

ObjectTypeSymbol* ContainerSymbol::GetObjectType(const std::string& objectTypeName, Node* node, Context* context) const
{
    Symbol* symbol = GetSymbol(objectTypeName, node, context);
    if (symbol && symbol->IsObjectTypeOrSpecializationSymbol())
    {
        return static_cast<ObjectTypeSymbol*>(symbol);
    }
    else
    {
        if (!context->NoThrow())
        {
            ThrowError("error: object type symbol '" + objectTypeName + "' expected", node->FilePath(), node->Span());
        }
    }
    return nullptr;
}

ProcedureSymbol* ContainerSymbol::GetProcedure(const std::string& procedureName) const
{
    auto it = symbolMap.find(procedureName);
    if (it != symbolMap.end())
    {
        Symbol* symbol = it->second;
        if (symbol->IsProcedureSymbol())
        {
            return static_cast<ProcedureSymbol*>(symbol);
        }
    }
    return nullptr;
}

ProcedureSymbol* ContainerSymbol::GetProcedure(const std::string& procedureName, Node* node, bool mustExist) const
{
    Symbol* symbol = GetSymbol(procedureName, node, false, mustExist);
    if (!mustExist && !symbol)
    {
        return nullptr;
    }
    if (symbol->IsProcedureSymbol())
    {
        return static_cast<ProcedureSymbol*>(symbol);
    }
    else
    {
        ThrowError("error: procedure symbol '" + procedureName + "' expected", node->FilePath(), node->Span());
    }
    return nullptr;
}

FunctionSymbol* ContainerSymbol::GetFunction(const std::string& functionName, Node* node, bool mustExist) const
{
    Symbol* symbol = GetSymbol(functionName, node, false, mustExist);
    if (!mustExist && !symbol)
    {
        return nullptr;
    }
    if (symbol->IsFunctionSymbol())
    {
        return static_cast<FunctionSymbol*>(symbol);
    }
    else
    {
        ThrowError("error: function symbol '" + functionName + "' expected", node->FilePath(), node->Span());
    }
    return nullptr;
}

SubroutineSymbol* ContainerSymbol::GetSubroutineNoThrow(const std::string& subroutineName) const
{
    auto it = symbolMap.find(subroutineName);
    if (it != symbolMap.end())
    {
        Symbol* symbol = it->second;
        if (symbol->IsSubroutineSymbol())
        {
            return static_cast<SubroutineSymbol*>(symbol);
        }
    }
    return nullptr;
}

ConstructorSymbol* ContainerSymbol::GetConstructor(const std::vector<TypeSymbol*>& parameterTypes) const
{
    if (constructorGroup)
    {
        ConstructorSymbol* constructor = constructorGroup->GetConstructor(parameterTypes);
        return constructor;
    }
    else
    {
        return nullptr;
    }
}

ConstructorSymbol* ContainerSymbol::GetOrInsertConstructor(std::vector<std::unique_ptr<ParameterSymbol>>& parameters, Node* node, bool mustExist)
{
    if (!constructorGroup)
    {
        AddSymbol(new ConstructorGroupSymbol(node->Span(), "@constructor_group"));
    }
    if (constructorGroup)
    {
        ConstructorSymbol* constructor = constructorGroup->GetConstructor(parameters);
        if (!constructor)
        {
            if (mustExist)
            {
                ThrowError("error: constructor not found", node->FilePath(), node->Span());
            }
            constructor = new ConstructorSymbol(node->Span(), "constructor");
            constructor->SetParameters(parameters);
            constructorGroup->AddConstructor(constructor, node);
        }
        return constructor;
    }
    else
    {
        ThrowError("error: constructor group not added", node->FilePath(), node->Span());
    }
    return nullptr;
}

void ContainerSymbol::Write(SymbolWriter& writer)
{
    int32_t n = symbols.size();
    writer.GetBinaryWriter().Write(n);
    for (const auto& symbol : symbols)
    {
        writer.WriteSymbol(symbol.get());
    }
}

void ContainerSymbol::Read(SymbolReader& reader)
{
    int32_t n = reader.GetBinaryReader().ReadInt();
    for (int32_t i = 0; i < n; ++i)
    {
        Symbol* symbol = reader.ReadSymbol(this);
        AddSymbol(symbol);
    }
}

FieldSymbol* ContainerSymbol::GetField(const std::string& fieldName) const
{
    auto it = symbolMap.find(fieldName);
    if (it != symbolMap.end())
    {
        Symbol* symbol = it->second;
        if (symbol->IsFieldSymbol())
        {
            return static_cast<FieldSymbol*>(symbol);
        }
        else
        {
            ThrowError("error: symbol '" + symbol->Name() + "' does not denote a field", SourceFilePath(), Span());
        }
    }
    else
    {
        ThrowError("error: field '" + fieldName + "' not found", SourceFilePath(), Span());
    }
    return nullptr;
}

FieldSymbol* ContainerSymbol::GetFieldNoThrow(const std::string& fieldName) const
{
    auto it = symbolMap.find(fieldName);
    if (it != symbolMap.end())
    {
        Symbol* symbol = it->second;
        if (symbol->IsFieldSymbol())
        {
            return static_cast<FieldSymbol*>(symbol);
        }
    }
    return nullptr;
}

void ContainerSymbol::MakeVmts()
{
    for (const auto& symbol : symbols)
    {
        symbol->MakeVmts();
    }
}

void ContainerSymbol::CheckDefined()
{
    for (const auto& symbol : symbols)
    {
        symbol->CheckDefined();
    }
}

void ContainerSymbol::SetGlobalVariableMap(GlobalVariableMap* globalVariableMap_)
{
    globalVariableMap = globalVariableMap_;
}

GlobalVariableMap* ContainerSymbol::GetGlobalVariableMap() const
{
    if (globalVariableMap)
    {
        return globalVariableMap;
    }
    else
    {
        ContainerSymbol* parent = Parent();
        if (parent)
        {
            return parent->GetGlobalVariableMap();
        }
        else
        {
            throw std::runtime_error("global variable map not set");
        }
    }
}

void ContainerSymbol::Print(util::CodeFormatter& formatter, bool full, ExecutionContext* context)
{
    Symbol::Print(formatter, full, context);
    if (full)
    {
        formatter.IncIndent();
        for (const auto& symbol : symbols)
        {
            symbol->Print(formatter, full, context);
        }
        if (globalVariableMap)
        {
            globalVariableMap->Print(formatter, full, context);
        }
        formatter.DecIndent();
    }
}

} // namespace p
