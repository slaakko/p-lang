// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.mod;

import p.code;
import p.writer;
import p.reader;
import p.parsing_context;
import p.context;
import p.execute;
import p.stdfn;
import p.compiler;
import std.filesystem;

namespace p {

ModuleHeaderCache::ModuleHeaderCache()
{
}

ModuleHeaderCache& ModuleHeaderCache::Instance()
{
    static ModuleHeaderCache instance;
    return instance;
}

void ModuleHeaderCache::SetModuleHeader(Module* moduleHeader)
{
    moduleHeaderMap[moduleHeader->Name()] = std::unique_ptr<Module>(moduleHeader);
}

Module* ModuleHeaderCache::GetModuleHeader(const std::string& moduleName, Context* context) 
{
    auto it = moduleHeaderMap.find(moduleName);
    if (it != moduleHeaderMap.end())
    {
        return it->second.get();
    }
    std::unique_ptr<Module> mod(new Module());
    mod->SetKind(ModuleKind::unit);
    mod->SetName(moduleName);
    mod->SetFilePath();
    mod->LoadHeader(mod->FilePath(), context);
    Module* m = mod.get();
    SetModuleHeader(mod.release());
    return m;
}

ModulePart::ModulePart(ModulePartKind kind_) : kind(kind_), mod(nullptr), systemModule(nullptr)
{
}

ModulePart::~ModulePart()
{
}

void ModulePart::AddImportedModuleName(const std::string& moduleName)
{
    importedModuleNames.insert(moduleName);
}

void ModulePart::SetBlock(Block* block_)
{
    block.reset(block_);
}

void ModulePart::Write(Writer& writer)
{
    writer.GetBinaryWriter().Write(static_cast<uint8_t>(kind));
    writer.GetBinaryWriter().Write(static_cast<int32_t>(importedModuleNames.size()));
    for (const auto& moduleName : importedModuleNames)
    {
        writer.GetBinaryWriter().Write(moduleName);
    }
    writer.GetBinaryWriter().Write(block != nullptr);
    if (block)
    {
        block->Write(writer);
    }
}

void ModulePart::Read(Reader& reader, Context* context)
{
    kind = static_cast<ModulePartKind>(reader.GetBinaryReader().ReadByte());
    int32_t n = reader.GetBinaryReader().ReadInt();
    for (int32_t i = 0; i < n; ++i)
    {
        importedModuleNames.insert(reader.GetBinaryReader().ReadUtf8String());
    }
    LoadModules(context);
    bool blockPresent = reader.GetBinaryReader().ReadBool();
    if (blockPresent)
    {
        Block* parentBlock = nullptr; 
        if (reader.GetBlock())
        {
            parentBlock = reader.GetBlock();
        }
        else if (context->GetBlock())
        {
            parentBlock = context->GetBlock();
        }
        if (parentBlock)
        {
            block.reset(new Block(parentBlock));
        }
        else
        {
            block.reset(new Block());
        }
        reader.PushModulePart(this);
        reader.PushBlock(block.get());
        block->Read(reader);
        reader.PopBlock();
        reader.PopModulePart();
    }
}

void ModulePart::LoadModules(Context* context)
{
    ModuleMap* moduleMap = context->GetModuleMap();
    if (mod->Name() != "System")
    {
        systemModule = moduleMap->GetModule("System", context);
        importedModules.push_back(systemModule);
        Block* rootBlock = systemModule->RootBlock();
        if (rootBlock)
        {
            rootBlock->AddOperatorFunctions();
            context->PushBlock(rootBlock);
        }
    }
    for (const auto& importedModuleName : importedModuleNames)
    {
        if (importedModuleName != "System")
        {
            Module* m = moduleMap->GetModule(importedModuleName, context);
            if (std::find(importedModules.begin(), importedModules.end(), m) == importedModules.end())
            {
                importedModules.push_back(m);
            }
        }
    }
}

void ModulePart::AddSubroutine(Subroutine* subroutine)
{
    subroutine->SetModuleId(mod->Id());
    subroutine->SetId(subroutineMap.size());
    subroutineMap[subroutine->Id()] = subroutine;
}

void ModulePart::AddSubroutineRead(Subroutine* subroutine)
{
    subroutineMap[subroutine->Id()] = subroutine;
}

Subroutine* ModulePart::GetSubroutine(int32_t subroutineId) const
{
    auto it = subroutineMap.find(subroutineId);
    if (it != subroutineMap.end())
    {
        return it->second;
    }
    else
    {
        throw std::runtime_error("subroutine id " + std::to_string(subroutineId) + " not found");
    }
}

void ModulePart::AddObjectType(ObjectType* objectType)
{
    objectTypes.push_back(objectType);
}

void ModulePart::Resolve(Context* context)
{
    for (const auto& mod : importedModules)
    {
        mod->Resolve(context);
    }
    if (block)
    {
        block->Resolve(context);
    }
}

void ModulePart::Print(util::CodeFormatter& formatter, ExecutionContext* context)
{
    if (IsInterfacePart())
    {
        formatter.WriteLine("interface");
    }
    else if (IsImplementationPart())
    {
        formatter.WriteLine("implementation");
    }
    if (!importedModules.empty())
    {
        formatter.Write("uses ");
        bool first = true;
        for (const auto& mod : importedModules)
        {
            if (first)
            {
                first = false;
            }
            else
            {
                formatter.Write(", ");
            }
            formatter.Write(mod->Name());
        }
        formatter.WriteLine();
    }
    if (block)
    {
        block->Print(formatter, context);
    }
}

void ModulePart::CheckInterface()
{
    Block* block = GetBlock();
    if (block)
    {
        block->CheckInterface();
    }
}

void ModulePart::MakeVmts(Context* context)
{
    Block* block = GetBlock();
    if (block)
    {
        block->MakeVmts(context);
    }
}

InterfacePart::InterfacePart() : ModulePart(ModulePartKind::interfacePart)
{
}

ImplementationPart::ImplementationPart() : ModulePart(ModulePartKind::implementationPart)
{
}

Module::Module() : kind(ModuleKind::unit), id(-1), init(nullptr), initRun(false)
{
}

void Module::SetName(const std::string& name_)
{
    name = name_;
}

void Module::SetId()
{
    id = std::hash<std::string>()(name) & 0x7fffffff;
}

void Module::SetFilePath()
{
    std::string plangPRoot = util::GetFullPath(util::PLangRoot());
    switch (kind)
    {
        case ModuleKind::unit:
        {
            filePath = util::Path::Combine(util::Path::Combine(plangPRoot, "unit"), name + ".pcode");
            break;
        }
        case ModuleKind::program:
        {
            filePath = util::Path::Combine(util::Path::Combine(plangPRoot, "prog"), name + ".pcode");
            break;
        }
    }
}

void Module::SetSourceFilePath(const std::string& sourceFilePath_)
{
    sourceFilePath = sourceFilePath_;
}

ModulePart* Module::CreateInterfacePart()
{
    interfacePart.reset(new InterfacePart());
    interfacePart->SetModule(this);
    return interfacePart.get();
}

ModulePart* Module::CreateImplementationPart()
{
    implementationPart.reset(new ImplementationPart());
    implementationPart->SetModule(this);
    return implementationPart.get();
}

bool Module::IsUpToDate(Context* context) 
{
    if (filePath.empty() || sourceFilePath.empty()) return false;
    if (!std::filesystem::exists(filePath) || !std::filesystem::exists(sourceFilePath)) return false;
    if (std::filesystem::last_write_time(sourceFilePath) > std::filesystem::last_write_time(filePath)) return false;
    for (const auto& moduleName : importedModuleNames)
    {
        Module* mod = ModuleHeaderCache::Instance().GetModuleHeader(moduleName, context);
        if (!mod->IsUpToDate(context))
        {
            return false;
        }
    }
    return true;
}

void Module::Init(ParsingContext* context)
{
    if (name == "System")
    {
        rootBlock.reset(new Block());
        rootBlock->AddFundamentalTypes();
        rootBlock->AddOperatorFunctions();
        context->PushBlock(rootBlock.get());
    }
}

void Module::Term(ParsingContext* context)
{
    context->PopBlock(); // pop System root
}

void Module::Save()
{
    if (interfacePart)
    {
        for const auto& importedModule : interfacePart->ImportedModules())
        {
            importedModuleNames.insert(importedModule->Name());
        }
    }
    if (implementationPart)
    {
        for const auto& importedModule : implementationPart->ImportedModules())
        {
            importedModuleNames.insert(importedModule->Name());
        }
    }
    Writer writer(filePath);
    writer.GetBinaryWriter().Write(static_cast<uint8_t>(kind));
    writer.GetBinaryWriter().Write(name);
    writer.GetBinaryWriter().Write(id);
    writer.GetBinaryWriter().Write(sourceFilePath);
    int32_t n = importedModuleNames.size();
    writer.GetBinaryWriter().Write(n);
    for (const auto& importedModuleName : importedModuleNames)
    {
        writer.GetBinaryWriter().Write(importedModuleName);
    }
    writer.GetBinaryWriter().Write(rootBlock != nullptr);
    if (rootBlock)
    {
        rootBlock->Write(writer);
    }
    writer.GetBinaryWriter().Write(interfacePart != nullptr);
    if (interfacePart)
    {
        interfacePart->Write(writer);
    }
    writer.GetBinaryWriter().Write(implementationPart != nullptr);
    if (implementationPart)
    {
        implementationPart->Write(writer);
    }
    writer.GetBinaryWriter().Write(initBlock != nullptr);
    if (initBlock)
    {
        initBlock->Write(writer);
    }
}

void Module::LoadHeader(const std::string& fromFilePath, Context* context)
{
    filePath = fromFilePath;
    Reader reader(filePath);
    reader.SetContext(context);
    kind = static_cast<ModuleKind>(reader.GetBinaryReader().ReadByte());
    name = reader.GetBinaryReader().ReadUtf8String();
    id = reader.GetBinaryReader().ReadInt();
    sourceFilePath = reader.GetBinaryReader().ReadUtf8String();
    int32_t n = reader.GetBinaryReader().ReadInt();
    for (int32_t i = 0; i < n; ++i)
    {
        std::string importedModuleName = reader.GetBinaryReader().ReadUtf8String();
        importedModuleNames.insert(importedModuleName);
    }
}

void Module::Load(const std::string& fromFilePath, Context* context)
{
    filePath = fromFilePath;
    Reader reader(filePath);
    reader.SetContext(context);
    kind = static_cast<ModuleKind>(reader.GetBinaryReader().ReadByte());
    name = reader.GetBinaryReader().ReadUtf8String();
    id = reader.GetBinaryReader().ReadInt();
    sourceFilePath = reader.GetBinaryReader().ReadUtf8String();
    int32_t n = reader.GetBinaryReader().ReadInt();
    for (int32_t i = 0; i < n; ++i)
    {
        std::string importedModuleName = reader.GetBinaryReader().ReadUtf8String();
        importedModuleNames.insert(importedModuleName);
    }
    bool hasRootBlock = reader.GetBinaryReader().ReadBool();
    if (hasRootBlock)
    {
        if (context->GetBlock())
        {
            rootBlock.reset(new Block(context->GetBlock()));
        }
        else
        {
            rootBlock.reset(new Block());
        }
        rootBlock->Read(reader);
        reader.PushBlock(rootBlock.get());
        context->PushBlock(rootBlock.get());
        SetStandardFunctionReturnTypes(rootBlock.get());
    }
    bool interfacePresent = reader.GetBinaryReader().ReadBool();
    if (interfacePresent)
    {
        interfacePart.reset(new InterfacePart());
        interfacePart->SetModule(this);
        interfacePart->Read(reader, context);
    }
    bool implementationPresent = reader.GetBinaryReader().ReadBool();
    if (implementationPresent)
    {
        implementationPart.reset(new ImplementationPart());
        implementationPart->SetModule(this);
        implementationPart->Read(reader, context);
    }
    if (hasRootBlock)
    {
        reader.PopBlock();
    }
    bool initBlockPresent = reader.GetBinaryReader().ReadBool();
    if (initBlockPresent)
    {
        initBlock.reset(new Block(context->GetBlock()));
        reader.PushBlock(context->GetBlock());
        initBlock->Read(reader);
        reader.PopBlock();
        init = initBlock->GetProcedure(Name() + ".@init");
    }
}

void Module::RunInits(ExecutionContext* context)
{
    ModulePart* interfacePart = GetInterfacePart();
    if (interfacePart)
    {
        for (const auto& mod : interfacePart->ImportedModules())
        {
            mod->RunInits(context);
        }
    }
    ModulePart* implementationPart = GetImplementationPart();
    if (implementationPart)
    {
        for (const auto& mod : implementationPart->ImportedModules())
        {
            mod->RunInits(context);
        }
    }
    if (initRun) return;
    initRun = true;
    if (init)
    {
        init->Execute(context);
    }
}

void Module::CheckInterface()
{
    if (interfacePart)
    {
        interfacePart->CheckInterface();
    }
    if (implementationPart)
    {
        implementationPart->CheckInterface();
    }
}

void Module::MakeVmts(Context* context)
{
    if (interfacePart)
    {
        interfacePart->MakeVmts(context);
    }
    if (implementationPart)
    {
        implementationPart->MakeVmts(context);
    }
}

void Module::Resolve(Context* context)
{
    if (interfacePart)
    {
        interfacePart->Resolve(context);
    }
    if (implementationPart)
    {
        implementationPart->Resolve(context);
    }
}

void Module::Print(const std::string& textFilePath, ExecutionContext* context)
{
    std::ofstream file(textFilePath);
    util::CodeFormatter formatter(file);
    formatter.SetIndentSize(2);
    formatter.WriteLine("module " + name + ", id=" + std::to_string(id));
    if (interfacePart)
    {
        interfacePart->Print(formatter, context);
    }
    if (implementationPart)
    {
        implementationPart->Print(formatter, context);
    }
    if (init)
    {
        init->Print(formatter, context);
    }
}

void Module::SetInit(ParsingContext* context, soul::lexer::LexerBase<char>& lexer, int64_t pos)
{
    init = MakeProcedure(context, Name() + ".@init", lexer, pos);
}

void Module::MakeInitBlock(Block* parent)
{
    initBlock.reset(new Block(parent));
}

ModuleMap::ModuleMap()
{
}

void ModuleMap::AddModule(Module* mod)
{
    moduleIdMap[mod->Id()] = mod;
}

Module* ModuleMap::GetModule(const std::string& moduleName, Context* context)
{
    auto it = moduleMap.find(moduleName);
    if (it != moduleMap.end())
    {
        return it->second;
    }
    else
    {
        std::unique_ptr<Module> mod(new Module());
        Module* thisMod = nullptr;
        bool compile = true;
        while (compile)
        {
            compile = false;
            thisMod = mod.get();
            mod->SetKind(ModuleKind::unit);
            mod->SetName(moduleName);
            mod->SetFilePath();
            moduleMap[moduleName] = mod.get();
            mod->LoadHeader(mod->FilePath(), context);
            if (GetCompileFlag(CompileFlags::update | CompileFlags::rebuild))
            {
                if ((GetCompileFlag(CompileFlags::rebuild) || context->UpdateUnits() || !mod->IsUpToDate(context)) && !context->UnitUpdated(mod->Name()))
                {
                    Compile(mod->SourceFilePath(), context);
                    context->AddUpdatedUnit(moduleName);
                    ModuleHeaderCache::Instance().SetModuleHeader(mod.release());
                    mod.reset(new Module());
                    context->Reset();
                    context->SetUpdateUnits();
                    compile = true;
                }
            }
        }
        mod->Load(mod->FilePath(), context);
        context->AddUpdatedUnit(moduleName);
        Block* blk = mod->RootBlock();
        if (!blk)
        {
            blk = mod->GetInterfacePart()->GetBlock();
        }
        context->PushBlock(blk);
        modules.push_back(std::move(mod));
        moduleIdMap[thisMod->Id()] = thisMod;
        thisMod->Resolve(context);
        return thisMod;
    }
}

Module* ModuleMap::GetModule(int32_t moduleId) const
{
    auto it = moduleIdMap.find(moduleId);
    if (it != moduleIdMap.end())
    {
        return it->second;
    }
    else
    {
        throw std::runtime_error("module id " + std::to_string(moduleId) + " not loaded");
    }
}

} // namespace p
