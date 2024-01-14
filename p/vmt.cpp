// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.vmt;

import p.type;
import p.subroutine;
import p.writer;
import p.reader;
import p.context;
import p.mod;

namespace p {

Vmt::Vmt()
{
}

void Vmt::AddMethod(Subroutine* method)
{
    if (method->VmtIndex() != -1)
    {
        if (method->VmtIndex() != methods.size())
        {
            throw std::runtime_error("error: method '" + method->InfoName() + "' has conflicting VMT index " + std::to_string(method->VmtIndex()) +
                "; would be given VMT index " + std::to_string(methods.size()));
        }
    }
    method->SetVmtIndex(methods.size());
    methods.push_back(method);
}

void Vmt::SetMethod(Subroutine* method)
{
    methods[method->VmtIndex()] = method;
}

Subroutine* Vmt::GetMethod(int32_t methodIndex) const
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

Subroutine* Vmt::GetMethod(const std::string& commonName) const
{
    for (const auto& method : methods)
    {
        if (method->CommonName() == commonName)
        {
            return method;
        }
    }
    return nullptr;
}

void Vmt::Write(Writer& writer)
{
    int32_t methodCount = methods.size();
    writer.GetBinaryWriter().Write(methodCount);
    for (const auto& method : methods)
    {
        writer.GetBinaryWriter().Write(method->ModuleId());
        writer.GetBinaryWriter().Write(method->ImplementationId());
    }
}

void Vmt::Read(Reader& reader)
{
    int32_t methodCount = reader.GetBinaryReader().ReadInt();
    for (int32_t i = 0; i < methodCount; ++i)
    {
        int32_t moduleId = reader.GetBinaryReader().ReadInt();
        int32_t implementationId = reader.GetBinaryReader().ReadInt();
        methodIds.push_back(std::make_pair(moduleId, implementationId));
    }
}

void Vmt::Resolve(Context* context)
{
    for (const auto& p : methodIds)
    {
        int32_t moduleId = p.first;
        int32_t implementationId = p.second;
        ModuleMap* moduleMap = context->GetModuleMap();
        Module* mod = moduleMap->GetModule(moduleId);
        Subroutine* method = mod->GetImplementationPart()->GetSubroutine(implementationId);
        methods.push_back(method);
    }
}

void InitVmt(Vmt& vmt, ObjectType* objectType, Context* context)
{
    if (!objectType->IsVirtual()) return;
    ModuleMap* moduleMap = context->GetModuleMap();
    if (objectType->BaseType())
    {
        InitVmt(vmt, objectType->BaseType(), context);
    }
    for (const auto& method : objectType->Methods())
    {
        Virtuality virtuality = method->Heading()->GetVirtuality();
        if (virtuality == Virtuality::virtual_)
        {
            Subroutine* prevMethod = vmt.GetMethod(method->CommonName());
            if (prevMethod)
            {
                throw std::runtime_error("error: object type '" + objectType->Name() + "' overrides method '" + prevMethod->InfoName() +
                    "'; method should be declared 'override'");
            }
            else
            {
                vmt.AddMethod(method.get());
            }
        }
        else if (virtuality == Virtuality::override_)
        {
            Subroutine* prevMethod = vmt.GetMethod(method->CommonName());
            if (prevMethod)
            {
                method->SetVmtIndex(prevMethod->VmtIndex());
                vmt.SetMethod(method.get());
            }
            else
            {
                throw std::runtime_error("error: object type '" + objectType->Name() + "' declares method '" + method->InfoName() +
                    "' as 'override' but no suitable method found to override");
            }
        }
        else
        {
            continue;
        }
        Module* mod = moduleMap->GetModule(method->ModuleId());
        Subroutine* implementation = mod->GetImplementationPart()->GetSubroutine(method->ImplementationId());
        implementation->SetVmtIndex(method->VmtIndex());
    }
}

void MakeVmt(Vmt& vmt, ObjectType* objectType, Context* context)
{
    InitVmt(vmt, objectType, context);
}

} // namespace p
