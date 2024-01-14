// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.constant;

import p.writer;
import p.reader;
import p.type;
import p.block;
import p.context;

namespace p {

Constant::Constant() : id(), name(), type(nullptr), value()
{
}

Constant::Constant(const std::string& name_) : id(util::uuid::random()), name(name_), type(nullptr), value()
{
}

Constant::~Constant()
{
}

void Constant::SetType(Type* type_)
{
    type = type_;
}

void Constant::SetValue(Value* value_)
{
    value.reset(value_);
}

void Constant::Write(Writer& writer)
{
    writer.GetBinaryWriter().Write(id);
    writer.GetBinaryWriter().Write(name);
    writer.GetBinaryWriter().Write(type->Id());
    writer.GetBinaryWriter().Write(static_cast<uint8_t>(value->Kind()));
    value->Write(writer);
}

void Constant::Read(Reader& reader)
{
    reader.GetBinaryReader().ReadUuid(id);
    name = reader.GetBinaryReader().ReadUtf8String();
    reader.GetBinaryReader().ReadUuid(typeId);
    ValueKind kind = static_cast<ValueKind>(reader.GetBinaryReader().ReadByte());
    value.reset(MakeValue(kind));
    value->Read(reader);
}

void Constant::Resolve(Context* context)
{
    type = context->GetBlock()->GetType(typeId);
}

void Constant::Print(util::CodeFormatter& formatter)
{
    formatter.WriteLine("constant '" + name + "'");
    formatter.IncIndent();
    formatter.WriteLine("type '" + type->Name() + "'");
    formatter.WriteLine("value '" + value->ToValueString() + "'");
    formatter.DecIndent();
}

GlobalConstantMap::GlobalConstantMap()
{
}

void GlobalConstantMap::AddConstant(Constant* constant)
{
    constantMap[constant->Id()] = constant;
}

Constant* GlobalConstantMap::GetConstant(const util::uuid& id) const
{
    auto it = constantMap.find(id);
    if (it != constantMap.end())
    {
        return it->second;
    }
    else
    {
        throw std::runtime_error("constant not found from global constant map");
    }
}

} // namespace p
