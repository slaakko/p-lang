// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.core.root_symbol;

import p.core.writer;
import p.core.reader;
import p.core.variable_symbol;
import p.core.symbol_table;
import p.core.context;
import p.core.unit_loader;
import std.filesystem;

namespace p {

RootSymbol::RootSymbol(SymbolKind kind_, const soul::ast::Span& span_, const std::string& name_) : ContainerSymbol(kind_, span_, name_), symbolTable(nullptr)
{
}

RootSymbol::RootSymbol(SymbolKind kind_, const soul::ast::Span& span_, const std::string& name_, const std::string& sourceFilePath_, const std::string& pcodeFilePath_) :
    ContainerSymbol(kind_, span_, name_), rootBlock(new BlockSymbol(span_, name_)), sourceFilePath(sourceFilePath_), pcodeFilePath(pcodeFilePath_), symbolTable(nullptr)
{
    SetGlobalVariableMap(new GlobalVariableMap());
    rootBlock->SetParent(this);
}

const std::string& RootSymbol::SourceFilePath() const
{
    return sourceFilePath;
}

void RootSymbol::AddUsedUnitName(const std::string& usedUnitName)
{
    if (std::find(usedUnitNames.begin(), usedUnitNames.end(), usedUnitName) == usedUnitNames.end())
    {
        usedUnitNames.push_back(usedUnitName);
    }
}

void RootSymbol::Write(SymbolWriter& writer)
{
    ContainerSymbol::Write(writer);
    int32_t n = usedUnitNames.size();
    writer.GetBinaryWriter().Write(n);
    for (const auto& usedUnitName : usedUnitNames)
    {
        writer.GetBinaryWriter().Write(usedUnitName);
    }
    writer.GetBinaryWriter().Write(sourceFilePath);
    writer.GetBinaryWriter().Write(pcodeFilePath);
    writer.WriteSymbol(rootBlock.get());
}

void RootSymbol::Read(SymbolReader& reader)
{
    SetSymbolTable(reader.GetSymbolTable());
    SetGlobalVariableMap(new GlobalVariableMap());
    ContainerSymbol::Read(reader);
    int32_t n = reader.GetBinaryReader().ReadInt();
    for (int32_t i = 0; i < n; ++i)
    {
        std::string usedUnitName = reader.GetBinaryReader().ReadUtf8String();
        AddUsedUnitName(usedUnitName);
    }
    sourceFilePath = reader.GetBinaryReader().ReadUtf8String();
    pcodeFilePath = reader.GetBinaryReader().ReadUtf8String();
    if (!reader.ReadOnlyHeader())
    {
        rootBlock.reset(static_cast<BlockSymbol*>(reader.ReadSymbol(this)));
    }
}

void RootSymbol::MakeVmts()
{
    ContainerSymbol::MakeVmts();
    if (rootBlock)
    {
        rootBlock->MakeVmts();
    }
}

void RootSymbol::CheckDefined()
{
    ContainerSymbol::CheckDefined();
    if (rootBlock)
    {
        rootBlock->CheckDefined();
    }
}

void RootSymbol::Print(util::CodeFormatter& formatter, bool full, ExecutionContext* context)
{
    ContainerSymbol::Print(formatter, full, context);
    if (full)
    {
        for (const auto& usedUnitName : usedUnitNames)
        {
            formatter.WriteLine("uses: '" + usedUnitName + "'");
        }
        formatter.WriteLine("source file path: " + sourceFilePath);
        formatter.WriteLine("pcode file path: " + pcodeFilePath);
        rootBlock->Print(formatter, full, context);
    }
}

bool RootSymbol::IsUpToDate(Context* context) const
{
    if (sourceFilePath.empty() || pcodeFilePath.empty()) return false;
    if (!std::filesystem::exists(sourceFilePath) || !std::filesystem::exists(pcodeFilePath)) return false;
    if (std::filesystem::last_write_time(sourceFilePath) > std::filesystem::last_write_time(pcodeFilePath)) return false;
    for (const auto& unitName : UsedUnitNames())
    {
        RootSymbol* usedUnitRoot = context->GetUnitLoader()->GetUnitHeader(unitName, context);
        if (!usedUnitRoot->IsUpToDate(context))
        {
            return false;
        }
    }
    return true;
}

} // namespace p
