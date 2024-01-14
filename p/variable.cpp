// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.variable;

import p.writer;
import p.reader;
import p.type;
import p.block;
import p.context;
import p.execute;

namespace p {

std::string VariableKindStr(VariableKind kind)
{
    switch (kind)
    {
        case VariableKind::local:
        {
            return "local";
        }
        case VariableKind::parameter:
        {
            return "parameter";
        }
        case VariableKind::global:
        {
            return "global";
        }
    }
    return std::string();
}

Variable::Variable() : kind(VariableKind::local), name(), index(-1), type(nullptr), block(nullptr)
{
}

Variable::Variable(const std::string& name_) : kind(VariableKind::local), name(name_), index(-1), type(nullptr), block(nullptr)
{
}

Variable::~Variable()
{
}

void Variable::SetType(Type* type_)
{
    type = type_;
}

void Variable::Write(Writer& writer)
{
    writer.GetBinaryWriter().Write(static_cast<uint8_t>(kind));
    writer.GetBinaryWriter().Write(name);
    writer.GetBinaryWriter().Write(index);
    writer.GetBinaryWriter().Write(type->Id());
}

void Variable::Read(Reader& reader)
{
    kind = static_cast<VariableKind>(reader.GetBinaryReader().ReadByte());
    name = reader.GetBinaryReader().ReadUtf8String();
    index = reader.GetBinaryReader().ReadInt();
    reader.GetBinaryReader().ReadUuid(typeId);
}

void Variable::Resolve(Context* context)
{
    type = context->GetBlock()->GetType(typeId);
}

void Variable::Print(util::CodeFormatter& formatter)
{
    formatter.Write(VariableKindStr(kind) + " ");
    formatter.Write("'" + name + "'");
    if (index != -1)
    {
        formatter.Write(" index " + std::to_string(index));
    }
    if (type)
    {
        formatter.Write(" type '" + type->Name() + "'");
    }
    formatter.WriteLine();
}

GlobalVariableMap::GlobalVariableMap()
{
}

void GlobalVariableMap::AddVariable(Variable* variable)
{
    variable->SetKind(VariableKind::global);
    if (variable->Index() == -1)
    {
        variable->SetIndex(variableMap.size());
    }
    variableMap[variable->Index()] = variable;
}

Variable* GlobalVariableMap::GetVariable(int32_t index) const
{
    auto it = variableMap.find(index);
    if (it != variableMap.end())
    {
        return it->second;
    }
    else
    {
        throw std::runtime_error("variable " + std::to_string(index) + " not found from global variable map");
    }
}

void GlobalVariableMap::AllocateFrame()
{
    globalFrame.reset(new Frame(variableMap.size(), std::vector<Parameter>(), nullptr));
}

Object* GlobalVariableMap::GetObject(int32_t index) const
{
    return globalFrame->GetObject(index);
}

void GlobalVariableMap::SetObject(int32_t index, Object* object, ExecutionContext* context)
{
    globalFrame->SetObject(index, object, context);
}

} // namespace p
