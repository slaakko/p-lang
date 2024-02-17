// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.core.unit_part_symbol;

import p.core.writer;
import p.core.reader;
import p.core.variable_symbol;

namespace p {

UnitPartSymbol::UnitPartSymbol(const soul::ast::Span& span_, const std::string& name_) : ContainerSymbol(SymbolKind::unitPartSymbol, span_, name_), implementationBlock(nullptr)
{
}

UnitPartSymbol::UnitPartSymbol(const soul::ast::Span& span_, const std::string& name_, UnitPartKind unitPartKind_) :
    ContainerSymbol(SymbolKind::unitPartSymbol, span_, name_), unitPartKind(unitPartKind_), block(new BlockSymbol(span_, name_)), implementationBlock(nullptr)
{
    block->SetParent(this);
}

void UnitPartSymbol::AddSymbol(Symbol* symbol)
{
    ContainerSymbol::AddSymbol(symbol);
    if (symbol->IsVariableSymbol())
    {
        VariableSymbol* variable = static_cast<VariableSymbol*>(symbol);
        GlobalVariableMap* globalVariableMap = GetGlobalVariableMap();
        globalVariableMap->AddVariable(variable);
    }
}

void UnitPartSymbol::Write(SymbolWriter& writer)
{
    ContainerSymbol::Write(writer);
    writer.GetBinaryWriter().Write(static_cast<uint8_t>(unitPartKind));
    writer.WriteSymbol(block.get());
}

void UnitPartSymbol::Read(SymbolReader& reader)
{
    ContainerSymbol::Read(reader);
    unitPartKind = static_cast<UnitPartKind>(reader.GetBinaryReader().ReadByte());
    block.reset(static_cast<BlockSymbol*>(reader.ReadSymbol(this)));
}

void UnitPartSymbol::SetImplementationBlock(BlockSymbol* implementationBlock_)
{
    implementationBlock = implementationBlock_;
}

Symbol* UnitPartSymbol::GetSymbol(const std::string& symbolName, Node* node, bool searchBase, bool mustExist) const
{
    Symbol* symbol = ContainerSymbol::GetSymbol(symbolName, node, false, false);
    if (symbol)
    {
        return symbol;
    }
    else if (implementationBlock)
    {
        return implementationBlock->GetSymbol(symbolName, node, searchBase, mustExist);
    }
    else
    {
        if (mustExist)
        {
            ThrowError("error: symbol '" + symbolName + "' not found", node->FilePath(), node->Span());
        }
    }
    return nullptr;
}

void UnitPartSymbol::MakeVmts()
{
    ContainerSymbol::MakeVmts();
    if (block)
    {
        block->MakeVmts();
    }
}

void UnitPartSymbol::CheckDefined()
{
    ContainerSymbol::CheckDefined();
    if (block)
    {
        block->CheckDefined();
    }
}

void UnitPartSymbol::Print(util::CodeFormatter& formatter, bool full, ExecutionContext* context)
{
    ContainerSymbol::Print(formatter, full, context);
    if (full)
    {
        formatter.WriteLine("unit part: " + UnitPartKindStr(unitPartKind));
        if (block)
        {
            block->Print(formatter, full, context);
        }
    }
}

} // namespace p
