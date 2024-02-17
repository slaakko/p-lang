// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.core.block_symbol;

import p.core.unit_part_symbol;
import p.core.writer;
import p.core.reader;

namespace p {

BlockSymbol::BlockSymbol(const soul::ast::Span& span_, const std::string& name_) : 
    ContainerSymbol(SymbolKind::blockSymbol, span_, name_), interfacePart(nullptr), implementationPart(nullptr), initializationPart(nullptr), level(0)
{
}

void BlockSymbol::AddSymbol(Symbol* symbol)
{
    ContainerSymbol::AddSymbol(symbol);
    if (symbol->IsUnitPartSymbol())
    {
        UnitPartSymbol* unitPartSymbol = static_cast<UnitPartSymbol*>(symbol);
        switch (unitPartSymbol->PartKind())
        {
            case UnitPartKind::interfacePart:
            {
                interfacePart = unitPartSymbol;
                break;
            }
            case UnitPartKind::implementationPart:
            {
                implementationPart = unitPartSymbol;
                break;
            }
            case UnitPartKind::initializationPart:
            {
                initializationPart = unitPartSymbol;
                break;
            }
        }
    }
}

void BlockSymbol::Write(SymbolWriter& writer)
{
    ContainerSymbol::Write(writer);
    writer.GetBinaryWriter().Write(level);
}

void BlockSymbol::Read(SymbolReader& reader)
{
    ContainerSymbol::Read(reader);
    level = reader.GetBinaryReader().ReadInt();
}

void BlockSymbol::Print(util::CodeFormatter& formatter, bool full, ExecutionContext* context)
{
    ContainerSymbol::Print(formatter, full, context);
    if (level != -1)
    {
        formatter.WriteLine("level: " + std::to_string(level));
    }
}

} // namespace p
