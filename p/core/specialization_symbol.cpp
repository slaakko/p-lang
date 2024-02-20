// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.core.specialization_symbol;

import p.core.symbol_table;
import p.core.object_type_symbol;
import p.core.writer;
import p.core.reader;
import p.ast;

namespace p {

BoundTypeParamSymbol::BoundTypeParamSymbol(const soul::ast::Span& span_, const std::string& name_) : 
    TypeSymbol(SymbolKind::boundTypeParamSymbol, span_, name_), typeArgument(nullptr)
{
}

BoundTypeParamSymbol::BoundTypeParamSymbol(const soul::ast::Span& span_, const std::string& name_, TypeSymbol* typeArgument_) :
    TypeSymbol(SymbolKind::boundTypeParamSymbol, span_, name_), typeArgument(typeArgument_)
{
}

void BoundTypeParamSymbol::Write(SymbolWriter& writer)
{
    TypeSymbol::Write(writer);
    writer.GetBinaryWriter().Write(typeArgument->Id());
}

void BoundTypeParamSymbol::Read(SymbolReader& reader)
{
    TypeSymbol::Read(reader);
    reader.GetBinaryReader().ReadUuid(typeArgumentId);
}

void BoundTypeParamSymbol::Resolve()
{
    TypeSymbol::Resolve();
    SymbolTable* symbolTable = GetSymbolTable();
    typeArgument = symbolTable->GetType(typeArgumentId, SourceFilePath(), Span());
}

void BoundTypeParamSymbol::Print(util::CodeFormatter& formatter, bool full, ExecutionContext* context)
{
    TypeSymbol::Print(formatter, full, context);
    if (full)
    {
        formatter.Write("type argument: ");
        typeArgument->Print(formatter, false, context);
    }
}

SpecializationSymbol::SpecializationSymbol(const soul::ast::Span& span_, const std::string& name_) : 
    ObjectTypeSymbol(SymbolKind::specializationSymbol, span_, name_), genericType(nullptr), typeArgumentSymbol(nullptr), instantiated(false), printed(false)
{
}

SpecializationSymbol::SpecializationSymbol(const soul::ast::Span& span_, const std::string& name_, ObjectTypeSymbol* genericType_, TypeSymbol* typeArgumentSymbol_) :
    ObjectTypeSymbol(SymbolKind::specializationSymbol, span_, name_), genericType(genericType_), typeArgumentSymbol(typeArgumentSymbol_), instantiated(false), printed(false)
{
}

void SpecializationSymbol::Write(SymbolWriter& writer)
{
    TypeSymbol::Write(writer);
    writer.GetBinaryWriter().Write(genericType->Id());
    writer.GetBinaryWriter().Write(typeArgumentSymbol->Id());
    writer.GetBinaryWriter().Write(instantiated);
}

void SpecializationSymbol::Read(SymbolReader& reader)
{
    TypeSymbol::Read(reader);
    reader.GetBinaryReader().ReadUuid(genericTypeId);
    reader.GetBinaryReader().ReadUuid(typeArgumentId);
    instantiated = reader.GetBinaryReader().ReadBool();
}

void SpecializationSymbol::Resolve()
{
    TypeSymbol::Resolve();
    SymbolTable* symbolTable = GetSymbolTable();
    TypeSymbol* type = symbolTable->GetType(genericTypeId, SourceFilePath(), Span());
    if (type->IsObjectTypeSymbol())
    {
        genericType = static_cast<ObjectTypeSymbol*>(type);
        if (!genericType->IsGeneric())
        {
            ThrowError("error: generic type expected", SourceFilePath(), Span());
        }
    }
    else
    {
        ThrowError("error: generic type expected", SourceFilePath(), Span());
    }
    typeArgumentSymbol = symbolTable->GetType(typeArgumentId, SourceFilePath(), Span());
    symbolTable->MapType(this);
}

void SpecializationSymbol::Print(util::CodeFormatter& formatter, bool full, ExecutionContext* context)
{
    if (!printed)
    {
        printed = true;
        ObjectTypeSymbol::Print(formatter, full, context);
        if (full)
        {
            formatter.Write("generic type: ");
            genericType->Print(formatter, false, context);
            formatter.Write("type argument: ");
            typeArgumentSymbol->Print(formatter, false, context);
            formatter.WriteLine("instantiated: " + instantiated ? "true" : "false");
        }
    }
}

} // namespace p
