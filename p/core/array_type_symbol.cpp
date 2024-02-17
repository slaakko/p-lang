// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.core.array_type_symbol;

import p.core.symbol_table;
import p.core.writer;
import p.core.reader;

namespace p {

ArrayTypeSymbol::ArrayTypeSymbol(const soul::ast::Span& span_, const std::string& name_) : TypeSymbol(SymbolKind::arrayTypeSymbol, span_, name_)
{
}

void ArrayTypeSymbol::Write(SymbolWriter& writer)
{
    TypeSymbol::Write(writer);
    writer.GetBinaryWriter().Write(elementType->Id());
}

void ArrayTypeSymbol::Read(SymbolReader& reader)
{
    TypeSymbol::Read(reader);
    reader.GetBinaryReader().ReadUuid(elementTypeId);
}

void ArrayTypeSymbol::Resolve()
{
    TypeSymbol::Resolve();
    SymbolTable* symbolTable = GetSymbolTable();
    elementType = symbolTable->GetType(elementTypeId, SourceFilePath(), Span());
    symbolTable->MapType(this);
}

void ArrayTypeSymbol::Print(util::CodeFormatter& formatter, bool full, ExecutionContext* context)
{
    TypeSymbol::Print(formatter, full, context);
    if (full)
    {
        formatter.Write("element type: ");
        elementType->Print(formatter, false, context);
    }
}

} // namespace p
