// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.core.constant_symbol;

import p.core.value;
import p.core.type_symbol;
import p.core.symbol_table;
import p.core.writer;
import p.core.reader;

namespace p {

ConstantSymbol::ConstantSymbol(const soul::ast::Span& span_, const std::string& name_) : Symbol(SymbolKind::constantSymbol, span_, name_), id(util::uuid::random()), type(nullptr)
{
}

void ConstantSymbol::SetValue(Value* value_)
{
    value.reset(value_);
}

void ConstantSymbol::Write(SymbolWriter& writer)
{
    writer.GetBinaryWriter().Write(id);
    writer.GetBinaryWriter().Write(type->Id());
    writer.GetBinaryWriter().Write(static_cast<uint8_t>(value->Kind()));
    value->Write(writer);
}

void ConstantSymbol::Read(SymbolReader& reader)
{
    reader.GetBinaryReader().ReadUuid(id);
    reader.GetBinaryReader().ReadUuid(typeId);
    ValueKind kind = static_cast<ValueKind>(reader.GetBinaryReader().ReadByte());
    value.reset(MakeValue(kind));
    value->Read(reader);
}

void ConstantSymbol::Resolve()
{
    Symbol::Resolve();
    SymbolTable* symbolTable = GetSymbolTable();
    type = symbolTable->GetType(typeId, SourceFilePath(), Span());
}

void ConstantSymbol::Print(util::CodeFormatter& formatter, bool full, ExecutionContext* context)
{
    Symbol::Print(formatter, full, context);
    formatter.WriteLine("id: " + util::ToString(id));
    formatter.Write("type: "); 
    type->Print(formatter, false, context);
}

} // namespace p
