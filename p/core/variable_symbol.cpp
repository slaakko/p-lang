// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.core.variable_symbol;

import p.core.type_symbol;
import p.core.symbol_table;
import p.core.writer;
import p.core.reader;
import p.core.execution_context;

namespace p {

VariableSymbol::VariableSymbol(const soul::ast::Span& span_, const std::string& name_) : 
    Symbol(SymbolKind::variableSymbol, span_, name_), type(nullptr), global(false), index(-1), globalId(util::uuid::random())
{
}

VariableSymbol::VariableSymbol(SymbolKind kind_, const soul::ast::Span& span_, const std::string& name_) : 
    Symbol(kind_, span_, name_), global(false), index(-1), globalId(util::uuid::random())
{
}

void VariableSymbol::Write(SymbolWriter& writer)
{
    writer.GetBinaryWriter().Write(type->Id());
    writer.GetBinaryWriter().Write(global);
    writer.GetBinaryWriter().Write(index);
    writer.GetBinaryWriter().Write(globalId);
}

void VariableSymbol::Read(SymbolReader& reader)
{
    reader.GetBinaryReader().ReadUuid(typeId);
    global = reader.GetBinaryReader().ReadBool();
    index = reader.GetBinaryReader().ReadInt();
    reader.GetBinaryReader().ReadUuid(globalId);
}

void VariableSymbol::Resolve()
{
    Symbol::Resolve();
    SymbolTable* symbolTable = GetSymbolTable();
    type = symbolTable->GetType(typeId, SourceFilePath(), Span());
}

void VariableSymbol::Print(util::CodeFormatter& formatter, bool full, ExecutionContext* context)
{
    Symbol::Print(formatter, full, context);
    if (full)
    {
        formatter.Write("type: ");
        type->Print(formatter, false, context);
        if (global)
        {
            formatter.WriteLine("global");
        }
        if (index != -1)
        {
            formatter.WriteLine("index: " + std::to_string(index));
        }
        if (global)
        {
            formatter.WriteLine("global id: " + util::ToString(globalId));
        }
    }
}

ParameterSymbol::ParameterSymbol(const soul::ast::Span& span_, const std::string& name_) : 
    VariableSymbol(SymbolKind::parameterSymbol, span_, name_), qualifier(ParameterQualifier::valueParam)
{
}

void ParameterSymbol::Write(SymbolWriter& writer)
{
    VariableSymbol::Write(writer);
    writer.GetBinaryWriter().Write(static_cast<uint8_t>(qualifier));
}

void ParameterSymbol::Read(SymbolReader& reader)
{
    VariableSymbol::Read(reader);
    qualifier = static_cast<ParameterQualifier>(reader.GetBinaryReader().ReadByte());
}

void ParameterSymbol::Print(util::CodeFormatter& formatter, bool full, ExecutionContext* context)
{
    VariableSymbol::Print(formatter, full, context);
    formatter.WriteLine("qualifier: " + ParameterQualifierStr(qualifier));
}

GlobalVariableMap::GlobalVariableMap()
{
}

void GlobalVariableMap::Import(GlobalVariableMap* that)
{
    for (const auto& idVar : that->variableMap)
    {
        VariableSymbol* variable = idVar.second;
        AddVariable(variable);
    }
}

void GlobalVariableMap::AddVariable(VariableSymbol* variable)
{
    VariableSymbol* var = GetVariable(variable->GlobalId());
    if (!var)
    {
        variable->SetGlobal();
        variableMap[variable->GlobalId()] = variable;
        int32_t index = objectIdMap.size();
        objectIdMap[variable->GlobalId()] = index;
    }
}

VariableSymbol* GlobalVariableMap::GetVariable(const util::uuid& id) const
{
    auto it = variableMap.find(id);
    if (it != variableMap.end())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

void GlobalVariableMap::AllocateFrame()
{
    globalFrame.reset(new Frame(objectIdMap.size(), std::vector<ParameterSymbol*>(), nullptr));
}

int32_t GlobalVariableMap::GetObjectIndex(const util::uuid& id) const
{
    auto it = objectIdMap.find(id);
    if (it != objectIdMap.end())
    {
        return it->second;
    }
    else
    {
        return -1;
    }
}

Object* GlobalVariableMap::GetObject(int32_t index) const
{
    return globalFrame->GetObject(index);
}

void GlobalVariableMap::SetObject(int32_t index, Object* object, ExecutionContext* context)
{
    globalFrame->SetObject(index, object, context);
}

void GlobalVariableMap::Print(util::CodeFormatter& formatter, bool full, ExecutionContext* context)
{
    formatter.WriteLine("global variable map:");
    for (const auto& idVar : variableMap)
    {
        formatter.Write(util::ToString(idVar.first) + ": ");
        idVar.second->Print(formatter, false, context);
    }
    for (const auto& idIndex : objectIdMap)
    {
        formatter.WriteLine(util::ToString(idIndex.first) + ": " + std::to_string(idIndex.second));
    }
}

} // namespace p
