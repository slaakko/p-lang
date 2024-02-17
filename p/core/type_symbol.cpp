// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.core.type_symbol;

import p.core.writer;
import p.core.reader;
import p.core.symbol_table;
import p.core.constant_symbol;
import p.core.value;

namespace p {

util::uuid BooleanTypeId()
{
    util::uuid typeId;
    typeId.data[15] = 1;
    return typeId;
}

util::uuid IntegerTypeId()
{
    util::uuid typeId;
    typeId.data[15] = 2;
    return typeId;
}

util::uuid CharTypeId()
{
    util::uuid typeId;
    typeId.data[15] = 3;
    return typeId;
}

util::uuid RealTypeId()
{
    util::uuid typeId;
    typeId.data[15] = 4;
    return typeId;
}

util::uuid StringTypeId()
{
    util::uuid typeId;
    typeId.data[15] = 5;
    return typeId;
}

util::uuid PointerTypeId()
{
    util::uuid typeId;
    typeId.data[15] = 6;
    return typeId;
}

util::uuid NilTypeId()
{
    util::uuid typeId;
    typeId.data[15] = 7;
    return typeId;
}

class FundamentalTypeIdRepository
{
public:
    FundamentalTypeIdRepository();
    static FundamentalTypeIdRepository& Instance();
    const util::uuid& GetBooleanTypeId() const { return booleanTypeId; }
    const util::uuid& GetIntegerTypeId() const { return integerTypeId; }
    const util::uuid& GetCharTypeId() const { return charTypeId; }
    const util::uuid& GetRealTypeId() const { return realTypeId; }
    const util::uuid& GetStringTypeId() const { return stringTypeId; }
    const util::uuid& GetPointerTypeId() const { return pointerTypeId; }
    const util::uuid& GetNilTypeId() const { return nilTypeId; }
private:
    util::uuid booleanTypeId;
    util::uuid integerTypeId;
    util::uuid charTypeId;
    util::uuid realTypeId;
    util::uuid stringTypeId;
    util::uuid pointerTypeId;
    util::uuid nilTypeId;
};

FundamentalTypeIdRepository::FundamentalTypeIdRepository()
{
    booleanTypeId = BooleanTypeId();
    integerTypeId = IntegerTypeId();
    charTypeId = CharTypeId();
    realTypeId = RealTypeId();
    stringTypeId = StringTypeId();
    pointerTypeId = PointerTypeId();
    nilTypeId = NilTypeId();
}

FundamentalTypeIdRepository& FundamentalTypeIdRepository::Instance()
{
    static FundamentalTypeIdRepository instance;
    return instance;
}

const util::uuid& GetBooleanTypeId()
{
    return FundamentalTypeIdRepository::Instance().GetBooleanTypeId();
}

const util::uuid& GetIntegerTypeId()
{
    return FundamentalTypeIdRepository::Instance().GetIntegerTypeId();
}

const util::uuid& GetCharTypeId()
{
    return FundamentalTypeIdRepository::Instance().GetCharTypeId();
}

const util::uuid& GetRealTypeId()
{
    return FundamentalTypeIdRepository::Instance().GetRealTypeId();
}

const util::uuid& GetStringTypeId()
{
    return FundamentalTypeIdRepository::Instance().GetStringTypeId();
}

const util::uuid& GetPointerTypeId()
{
    return FundamentalTypeIdRepository::Instance().GetPointerTypeId();
}

const util::uuid& GetNilTypeId()
{
    return FundamentalTypeIdRepository::Instance().GetNilTypeId();
}

TypeSymbol::TypeSymbol(SymbolKind kind_, const soul::ast::Span& span_, const std::string& name_) : ContainerSymbol(kind_, span_, name_), id(util::uuid::random())
{
}

TypeSymbol::TypeSymbol(SymbolKind kind_, const soul::ast::Span& span_, const std::string& name_, const util::uuid& id_) : ContainerSymbol(kind_, span_, name_), id(id_)
{
}

void TypeSymbol::Write(SymbolWriter& writer)
{
    ContainerSymbol::Write(writer);
    writer.GetBinaryWriter().Write(id);
}

void TypeSymbol::Read(SymbolReader& reader)
{
    ContainerSymbol::Read(reader);
    reader.GetBinaryReader().ReadUuid(id);
}

void TypeSymbol::Print(util::CodeFormatter& formatter, bool full, ExecutionContext* context)
{
    ContainerSymbol::Print(formatter, full, context);
    formatter.WriteLine("id: " + util::ToString(id));
}

OrdinalTypeSymbol::OrdinalTypeSymbol(SymbolKind kind_, const soul::ast::Span& span_, const std::string& name_) : TypeSymbol(kind_, span_, name_)
{
}

OrdinalTypeSymbol::OrdinalTypeSymbol(SymbolKind kind_, const soul::ast::Span& span_, const std::string& name_, const util::uuid& id_) : TypeSymbol(kind_, span_, name_, id_)
{
}

BooleanTypeSymbol::BooleanTypeSymbol(const soul::ast::Span& span_, const std::string& name_) : OrdinalTypeSymbol(SymbolKind::booleanTypeSymbol, span_, name_)
{
}

BooleanTypeSymbol::BooleanTypeSymbol(const soul::ast::Span& span_) : 
    OrdinalTypeSymbol(SymbolKind::booleanTypeSymbol, span_, "boolean", FundamentalTypeIdRepository().Instance().GetBooleanTypeId())
{
}

IntegerTypeSymbol::IntegerTypeSymbol(const soul::ast::Span& span_, const std::string& name_) : OrdinalTypeSymbol(SymbolKind::integerTypeSymbol, span_, name_)
{
}

IntegerTypeSymbol::IntegerTypeSymbol(const soul::ast::Span& span_) : OrdinalTypeSymbol(SymbolKind::integerTypeSymbol, span_, "integer",
    FundamentalTypeIdRepository().Instance().GetIntegerTypeId())
{
}

CharTypeSymbol::CharTypeSymbol(const soul::ast::Span& span_, const std::string& name_) : OrdinalTypeSymbol(SymbolKind::charTypeSymbol, span_, name_)
{
}

CharTypeSymbol::CharTypeSymbol(const soul::ast::Span& span_) : OrdinalTypeSymbol(SymbolKind::charTypeSymbol, span_, "char",
    FundamentalTypeIdRepository().Instance().GetCharTypeId())
{
}

EnumeratedTypeSymbol::EnumeratedTypeSymbol(const soul::ast::Span& span_, const std::string& name_) : OrdinalTypeSymbol(SymbolKind::enumeratedTypeSymbol, span_, name_)
{
}

void EnumeratedTypeSymbol::AddEnumConstant(ConstantSymbol* enumConstant)
{
    enumConstants.push_back(enumConstant);
}

void EnumeratedTypeSymbol::Write(SymbolWriter& writer)
{
    TypeSymbol::Write(writer);
    writer.GetBinaryWriter().Write(static_cast<int32_t>(enumConstants.size()));
    for (const auto& enumConstant : enumConstants)
    {
        writer.GetBinaryWriter().Write(enumConstant->Id());
    }
}

void EnumeratedTypeSymbol::Read(SymbolReader& reader)
{
    TypeSymbol::Read(reader);
    int32_t n = reader.GetBinaryReader().ReadInt();
    for (int32_t i = 0; i < n; ++i)
    {
        util::uuid enumConstantId;
        reader.GetBinaryReader().ReadUuid(enumConstantId);
        enumConstantIds.push_back(enumConstantId);
    }
}

void EnumeratedTypeSymbol::Resolve()
{
    TypeSymbol::Resolve();
    SymbolTable* symbolTable = GetSymbolTable();
    for (const auto& enumConstantId : enumConstantIds)
    {
        ConstantSymbol* enumConstant = symbolTable->GetConstant(enumConstantId, SourceFilePath(), Span());
        enumConstants.push_back(enumConstant);
    }
}

SubrangeTypeSymbol::SubrangeTypeSymbol(const soul::ast::Span& span_, const std::string& name_) : OrdinalTypeSymbol(SymbolKind::subrangeTypeSymbol, span_, name_)
{
}

SubrangeTypeSymbol::SubrangeTypeSymbol(const soul::ast::Span& span_, const std::string& name_, Value* rangeStart_, Value* rangeEnd_) : 
    OrdinalTypeSymbol(SymbolKind::subrangeTypeSymbol, span_, name_), rangeStart(rangeStart_), rangeEnd(rangeEnd_)
{
}

void SubrangeTypeSymbol::Write(SymbolWriter& writer)
{
    TypeSymbol::Write(writer);
    writer.WriteValue(rangeStart.get());
    writer.WriteValue(rangeEnd.get());
}

void SubrangeTypeSymbol::Read(SymbolReader& reader)
{
    TypeSymbol::Read(reader);
    rangeStart.reset(reader.ReadValue());
    rangeEnd.reset(reader.ReadValue());
}

void SubrangeTypeSymbol::Print(util::CodeFormatter& formatter, bool full, ExecutionContext* context)
{
    OrdinalTypeSymbol::Print(formatter, full, context);
    if (full)
    {
        formatter.WriteLine("range start: " + rangeStart->ToValueString());
        formatter.WriteLine("range end: " + rangeEnd->ToValueString());
    }
}

RealTypeSymbol::RealTypeSymbol(const soul::ast::Span& span_, const std::string& name_) : TypeSymbol(SymbolKind::realTypeSymbol, span_, name_)
{
}

RealTypeSymbol::RealTypeSymbol(const soul::ast::Span& span_) : TypeSymbol(SymbolKind::realTypeSymbol, span_, "real", 
    FundamentalTypeIdRepository().Instance().GetRealTypeId())
{
}

StringTypeSymbol::StringTypeSymbol(const soul::ast::Span& span_, const std::string& name_) : TypeSymbol(SymbolKind::stringTypeSymbol, span_, name_)
{
}

StringTypeSymbol::StringTypeSymbol(const soul::ast::Span& span_) : TypeSymbol(SymbolKind::stringTypeSymbol, span_, "string", 
    FundamentalTypeIdRepository().Instance().GetStringTypeId())
{
}

PointerTypeSymbol::PointerTypeSymbol(const soul::ast::Span& span_, const std::string& name_) : TypeSymbol(SymbolKind::pointerTypeSymbol, span_, name_)
{
}

PointerTypeSymbol::PointerTypeSymbol(const soul::ast::Span& span_) : TypeSymbol(SymbolKind::pointerTypeSymbol, span_, "pointer", 
    FundamentalTypeIdRepository().Instance().GetPointerTypeId())
{
}

NilTypeSymbol::NilTypeSymbol(const soul::ast::Span& span_, const std::string& name_) : TypeSymbol(SymbolKind::nilTypeSymbol, span_, name_)
{
}

NilTypeSymbol::NilTypeSymbol(const soul::ast::Span& span_) : TypeSymbol(SymbolKind::nilTypeSymbol, span_, "@nil_type", 
    FundamentalTypeIdRepository().Instance().GetNilTypeId())
{
}

TypeSymbol* CommonType(TypeSymbol* left, TypeSymbol* right)
{
    if (left == right) return left;
    if (left->IsIntegerTypeSymbol() && right->IsRealTypeSymbol()) return right;
    if (left->IsRealTypeSymbol() && right->IsIntegerTypeSymbol()) return left;
    return nullptr;
}

void CreateFundamentalTypes(SymbolTable* symbolTable, const soul::ast::Span& span)
{
    symbolTable->AddTypeToRoot(new BooleanTypeSymbol(span));
    symbolTable->AddTypeToRoot(new IntegerTypeSymbol(span));
    symbolTable->AddTypeToRoot(new CharTypeSymbol(span));
    symbolTable->AddTypeToRoot(new RealTypeSymbol(span));
    symbolTable->AddTypeToRoot(new StringTypeSymbol(span));
    symbolTable->AddTypeToRoot(new PointerTypeSymbol(span));
    symbolTable->AddTypeToRoot(new NilTypeSymbol(span));
}

} // namespace p
