// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.core.root_symbol;

import p.core.writer;
import p.core.reader;
import p.core.variable_symbol;

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

void RootSymbol::Write(SymbolWriter& writer)
{
    ContainerSymbol::Write(writer);
    writer.GetBinaryWriter().Write(sourceFilePath);
    writer.GetBinaryWriter().Write(pcodeFilePath);
    writer.WriteSymbol(rootBlock.get());
}

void RootSymbol::Read(SymbolReader& reader)
{
    SetSymbolTable(reader.GetSymbolTable());
    SetGlobalVariableMap(new GlobalVariableMap());
    ContainerSymbol::Read(reader);
    sourceFilePath = reader.GetBinaryReader().ReadUtf8String();
    pcodeFilePath = reader.GetBinaryReader().ReadUtf8String();
    rootBlock.reset(static_cast<BlockSymbol*>(reader.ReadSymbol(this)));
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
        formatter.WriteLine("source file path: " + sourceFilePath);
        formatter.WriteLine("pcode file path: " + pcodeFilePath);
        rootBlock->Print(formatter, full, context);
    }
}

} // namespace p
