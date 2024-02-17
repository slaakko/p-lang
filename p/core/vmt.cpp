// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.core.vmt;

import p.core.type_symbol;
import p.core.subroutine_symbol;
import p.core.writer;
import p.core.reader;
import p.core.context;
import p.core.symbol_table;
import p.core.object_type_symbol;

namespace p {

Vmt::Vmt()
{
}

void Vmt::AddMethod(SubroutineSymbol* method)
{
    if (method->VmtIndex() != -1)
    {
        if (method->VmtIndex() != methods.size())
        {
            throw std::runtime_error("error: method '" + method->FullName() + "' has conflicting VMT index " + std::to_string(method->VmtIndex()) +
                "; would be given VMT index " + std::to_string(methods.size()));
        }
    }
    method->SetVmtIndex(methods.size());
    methods.push_back(method);
}

void Vmt::SetMethod(SubroutineSymbol* method)
{
    methods[method->VmtIndex()] = method;
}

SubroutineSymbol* Vmt::GetMethod(int32_t methodIndex) const
{
    if (methodIndex >= 0 && methodIndex < methods.size())
    {
        return methods[methodIndex];
    }
    else
    {
        return nullptr;
    }
}

SubroutineSymbol* Vmt::GetMethod(const std::string& commonName) const
{
    for (const auto& method : methods)
    {
        if (method->Name() == commonName)
        {
            return method;
        }
    }
    return nullptr;
}

void Vmt::Write(SymbolWriter& writer)
{
    int32_t methodCount = methods.size();
    writer.GetBinaryWriter().Write(methodCount);
    for (const auto& method : methods)
    {
        writer.GetBinaryWriter().Write(method->Id());
    }
}

void Vmt::Read(SymbolReader& reader)
{
    int32_t methodCount = reader.GetBinaryReader().ReadInt();
    for (int32_t i = 0; i < methodCount; ++i)
    {
        util::uuid methodId;
        reader.GetBinaryReader().ReadUuid(methodId);
        methodIds.push_back(methodId);
    }
}

void Vmt::Resolve(SymbolTable* symbolTable)
{
    for (const auto& methodId : methodIds)
    {
        SubroutineSymbol* method = symbolTable->GetSubroutine(methodId);
        methods.push_back(method);
    }
}

void InitVmt(Vmt& vmt, ObjectTypeSymbol* objectType)
{
    if (!objectType->IsVirtual()) return;
    if (objectType->BaseType())
    {
        InitVmt(vmt, objectType->BaseType());
    }
    for (auto method : objectType->Methods())
    {
        Virtual virtual_ = method->GetVirtual();
        if (virtual_ == Virtual::virtual_)
        {
            SubroutineSymbol* prevMethod = vmt.GetMethod(method->Name());
            if (prevMethod)
            {
                throw std::runtime_error("error: object type '" + objectType->Name() + "' overrides method '" + prevMethod->FullName() +
                    "'; method should be declared 'override'");
            }
            else
            {
                vmt.AddMethod(method);
            }
        }
        else if (virtual_ == Virtual::override_)
        {
            SubroutineSymbol* prevMethod = vmt.GetMethod(method->Name());
            if (prevMethod)
            {
                method->SetVmtIndex(prevMethod->VmtIndex());
                vmt.SetMethod(method);
            }
            else
            {
                throw std::runtime_error("error: object type '" + objectType->Name() + "' declares method '" + method->FullName() +
                    "' as 'override' but no suitable method found to override");
            }
        }
        else
        {
            continue;
        }
    }
}

void MakeVmt(Vmt& vmt, ObjectTypeSymbol* objectType)
{
    InitVmt(vmt, objectType);
}

} // namespace p
