// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.core.alias_type_symbol;

import p.core.symbol_table;
import p.core.writer;
import p.core.reader;

namespace p {

AliasTypeSymbol::AliasTypeSymbol(const soul::ast::Span& span_, const std::string& name_) : TypeSymbol(SymbolKind::aliasTypeSymbol, span_, name_), type(nullptr)
{
}

AliasTypeSymbol::AliasTypeSymbol(const soul::ast::Span& span_, const std::string& name_, TypeSymbol* type_) : TypeSymbol(SymbolKind::aliasTypeSymbol, span_, name_), type(type_)
{
}

void AliasTypeSymbol::Write(SymbolWriter& writer)
{
    TypeSymbol::Write(writer);
    writer.GetBinaryWriter().Write(type->Id());
}

void AliasTypeSymbol::Read(SymbolReader& reader)
{
    TypeSymbol::Read(reader);
    reader.GetBinaryReader().ReadUuid(typeId);
}

void AliasTypeSymbol::Resolve()
{
    TypeSymbol::Resolve();
    SymbolTable* symbolTable = GetSymbolTable();
    type = symbolTable->GetType(typeId, SourceFilePath(), Span());
}

void AliasTypeSymbol::Print(util::CodeFormatter& formatter, bool full, ExecutionContext* context)
{
    TypeSymbol::Print(formatter, full, context);
    if (full)
    {
        if (type)
        {
            formatter.Write("type: ");
            type->Print(formatter, false, context);
        }
    }
}

} // namespace p
