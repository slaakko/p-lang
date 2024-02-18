// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.core.symbol_table;

import p.core.type_symbol;
import p.core.array_type_symbol;
import p.core.constant_symbol;
import p.core.unit_part_symbol;
import p.core.value;
import p.core.writer;
import p.core.reader;
import p.core.context;
import p.core.unit_loader;
import p.core.variable_symbol;

namespace p {

SymbolTable::SymbolTable() : initRun(false)
{
}

void SymbolTable::SetRoot(RootSymbol* root_)
{
    root.reset(root_);
    root->SetSymbolTable(this);
}

void SymbolTable::Import(SymbolTable* symbolTableToImport)
{
    if (std::find(importedUnits.begin(), importedUnits.end(), symbolTableToImport) != importedUnits.end()) return;
    importedUnits.push_back(symbolTableToImport);
    for (const auto& typeIdTypeSymbol : symbolTableToImport->typeIdMap)
    {
        typeIdMap[typeIdTypeSymbol.first] = typeIdTypeSymbol.second;
    }
    for (const auto& typeNameSymbol : symbolTableToImport->typeNameMap)
    {
        typeNameMap[typeNameSymbol.first] = typeNameSymbol.second;
    }
    for (const auto& arrayTypeIdSymbol : symbolTableToImport->arrayTypeIdMap)
    {
        arrayTypeIdMap[arrayTypeIdSymbol.first] = arrayTypeIdSymbol.second;
    }
    for (const auto& constantIdSymbol : symbolTableToImport->constantIdMap)
    {
        constantIdMap[constantIdSymbol.first] = constantIdSymbol.second;
    }
    for (const auto& subroutineIdSymbol : symbolTableToImport->subroutineIdMap)
    {
        subroutineIdMap[subroutineIdSymbol.first] = subroutineIdSymbol.second;
    }
    root->Block()->Import(symbolTableToImport->Root());
    root->Block()->Import(symbolTableToImport->Root()->Block());
    root->Block()->Import(symbolTableToImport->Root()->Block()->InterfacePart());
    root->Block()->Import(symbolTableToImport->Root()->Block()->InterfacePart()->Block());
    GlobalVariableMap* globalVariableMap = root->GetGlobalVariableMap();
    if (globalVariableMap)
    {
        GlobalVariableMap* globalVariableMapToImport = symbolTableToImport->Root()->GetGlobalVariableMap();
        globalVariableMap->Import(globalVariableMapToImport);
    }
}

void SymbolTable::Write(SymbolWriter& writer)
{
    writer.WriteSymbol(root.get());
}

void SymbolTable::Read(SymbolReader& reader)
{
    reader.SetSymbolTable(this);
    root.reset(static_cast<RootSymbol*>(reader.ReadSymbol(nullptr)));
    Context* context = reader.GetContext();
    UnitLoader* loader = context->GetUnitLoader();
    for (const auto& usedUnitName : root->UsedUnitNames())
    {
        loader->ImportUnit(usedUnitName, root.get(), context);
    }
}

TypeSymbol* SymbolTable::GetType(const util::uuid& id) const
{
    auto it = typeIdMap.find(id);
    if (it != typeIdMap.end())
    {
        return it->second;
    }
    else
    {
        throw std::runtime_error("error: type id " + util::ToString(id) + " not found");
    }
}

TypeSymbol* SymbolTable::GetType(const util::uuid& id, Node* node) const
{
    auto it = typeIdMap.find(id);
    if (it != typeIdMap.end())
    {
        return it->second;
    }
    else
    {
        ThrowError("error: type id " + util::ToString(id) + " not found", node->FilePath(), node->Span());
    }
    return nullptr;
}

TypeSymbol* SymbolTable::GetType(const util::uuid& id, const std::string& sourceFilePath, const soul::ast::Span& span) const
{
    auto it = typeIdMap.find(id);
    if (it != typeIdMap.end())
    {
        return it->second;
    }
    else
    {
        ThrowError("error: type id " + util::ToString(id) + " not found", sourceFilePath, span);
    }
    return nullptr;
}

TypeSymbol* SymbolTable::GetType(const std::string& typeName, const std::string& sourceFilePath, const soul::ast::Span& span) const
{
    auto it = typeNameMap.find(typeName);
    if (it != typeNameMap.end())
    {
        return it->second;
    }
    else
    {
        ThrowError("error: type '" + typeName + " not found", sourceFilePath, span);
    }
    return nullptr;
}

ArrayTypeSymbol* SymbolTable::GetArrayType(const util::uuid& elementTypeId, Node* node) const
{
    auto it = arrayTypeIdMap.find(elementTypeId);
    if (it != arrayTypeIdMap.end())
    {
        return it->second;
    }
    else
    {
        ThrowError("error: array type for element type id " + util::ToString(elementTypeId) + " not found", node->FilePath(), node->Span());
    }
    return nullptr;
}

TypeSymbol* SymbolTable::GetFundamentalType(SymbolKind kind, Node* node) const
{
    switch (kind)
    {
        case SymbolKind::booleanTypeSymbol:
        {
            return GetType(GetBooleanTypeId(), node);
        }
        case SymbolKind::integerTypeSymbol:
        case SymbolKind::enumeratedTypeSymbol:
        {
            return GetType(GetIntegerTypeId(), node);
        }
        case SymbolKind::charTypeSymbol:
        {
            return GetType(GetCharTypeId(), node);
        }
        case SymbolKind::realTypeSymbol:
        {
            return GetType(GetRealTypeId(), node);
        }
        case SymbolKind::stringTypeSymbol:
        {
            return GetType(GetStringTypeId(), node);
        }
        case SymbolKind::pointerTypeSymbol:
        {
            return GetType(GetPointerTypeId(), node);
        }
        case SymbolKind::nilTypeSymbol:
        {
            return GetType(GetNilTypeId(), node);
        }
    }
    ThrowError("error: fundamental type kind expected", node->FilePath(), node->Span());
    return nullptr;
}

void SymbolTable::MapType(TypeSymbol* type)
{
    typeIdMap[type->Id()] = type;
    typeNameMap[type->Name()] = type;
    if (type->IsArrayTypeSymbol())
    {
        ArrayTypeSymbol* arrayType = static_cast<ArrayTypeSymbol*>(type);
        if (arrayType->ElementType())
        {
            arrayTypeIdMap[arrayType->ElementType()->Id()] = arrayType;
        }
    }
}

void SymbolTable::AddTypeToRoot(TypeSymbol* type)
{
    root->Block()->AddSymbol(type);
}

void SymbolTable::MapConstant(ConstantSymbol* constantSymbol)
{
    constantIdMap[constantSymbol->Id()] = constantSymbol;
}

ConstantSymbol* SymbolTable::GetConstant(const util::uuid& id, const std::string& sourceFilePath, const soul::ast::Span& span)
{
    auto it = constantIdMap.find(id);
    if (it != constantIdMap.end())
    {
        return it->second;
    }
    else
    {
        ThrowError("error: constant id " + util::ToString(id) + " not found", sourceFilePath, span);
    }
    return nullptr;
}

ConstantSymbol* SymbolTable::GetConstant(const util::uuid& id) const
{
    auto it = constantIdMap.find(id);
    if (it != constantIdMap.end())
    {
        return it->second;
    }
    else
    {
        throw std::runtime_error("error: constant id " + util::ToString(id) + " not found");
    }
}

void SymbolTable::MapSubroutine(SubroutineSymbol* subroutine)
{
    subroutineIdMap[subroutine->Id()] = subroutine;
}

SubroutineSymbol* SymbolTable::GetSubroutine(const util::uuid& id) const
{
    auto it = subroutineIdMap.find(id);
    if (it != subroutineIdMap.end())
    {
        return it->second;
    }
    else
    {
        throw std::runtime_error("error: subroutine id " + util::ToString(id) + " not found");
    }
}

SubroutineSymbol* SymbolTable::GetSubroutine(const util::uuid& id, const std::string& sourceFilePath, const soul::ast::Span& span) const
{
    auto it = subroutineIdMap.find(id);
    if (it != subroutineIdMap.end())
    {
        return it->second;
    }
    else
    {
        ThrowError("error: subroutine id " + util::ToString(id) + " not found", sourceFilePath, span);
    }
    return nullptr;
}

void SymbolTable::AddSymbol(Symbol* symbol)
{
    symbols.push_back(symbol);
}

void SymbolTable::Resolve()
{
    for (auto symbol : symbols)
    {
        symbol->Resolve();
    }
    symbols.clear();
}

void SymbolTable::MakeVmts()
{
    if (root)
    {
        root->MakeVmts();
    }
}

void SymbolTable::CheckDefined()
{
    if (root)
    {
        root->CheckDefined();
    }
}

void SymbolTable::RunInits(ExecutionContext* context)
{
    for (auto importedUnit : importedUnits)
    {
        importedUnit->RunInits(context);
    }
    if (initRun) return;
    initRun = true;
    UnitPartSymbol* initializationPart = root->Block()->InitializationPart();
    if (initializationPart)
    {
        ProcedureSymbol* init = initializationPart->Block()->GetProcedure("@init");
        if (init)
        {
            init->Execute(context);
        }
    }
}

void SymbolTable::Print(util::CodeFormatter& formatter, ExecutionContext* context)
{
    root->Print(formatter, true, context);
}

} // namespace p
