// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.subroutine;

import p.block;
import p.parsing_context;
import p.context;
import p.code;
import p.type;
import p.writer;
import p.reader;
import p.execute;

namespace p {

std::string SubroutineKindStr(SubroutineKind kind)
{
    switch (kind)
    {
        case SubroutineKind::procedure: return "procedure";
        case SubroutineKind::function: return "function";
        case SubroutineKind::constructor: return "constructor";
    }
    return std::string();
}

Parameter::Parameter() : Variable(), qualifier()
{
}

Parameter::Parameter(const std::string& name_, ParameterQualifier qualifier_, Type* type_) : Variable(name_), qualifier(qualifier_)
{
    SetType(type_);
    SetKind(VariableKind::parameter);
}

void Parameter::Write(Writer& writer)
{
    Variable::Write(writer);
    writer.GetBinaryWriter().Write(static_cast<uint8_t>(qualifier));
}

void Parameter::Read(Reader& reader)
{
    Variable::Read(reader);
    qualifier = static_cast<ParameterQualifier>(reader.GetBinaryReader().ReadByte());
}

void Parameter::Print(util::CodeFormatter& formatter)
{
    switch (qualifier)
    {
    case ParameterQualifier::varParam:
    {
        formatter.Write("var ");
        break;
    }
    case ParameterQualifier::constParam:
    {
        formatter.Write("const ");
        break;
    }
    }
    Variable::Print(formatter);
}

SubroutineHeading::SubroutineHeading(SubroutineHeadingKind kind_) :
    kind(kind_), objectType(nullptr), virtuality(Virtuality::none)
{
}

SubroutineHeading::SubroutineHeading(SubroutineHeadingKind kind_, const std::string& fullName_) :
    kind(kind_), fullName(fullName_), objectType(nullptr), virtuality(Virtuality::none)
{
}

void SubroutineHeading::AddParameter(Parameter& parameter)
{
    if (parameter.Index() == -1)
    {
        parameter.SetIndex(parameters.size());
    }
    parameters.push_back(parameter);
}

void SubroutineHeading::InsertThisParam()
{
    Parameter thisParam("this", ParameterQualifier::valueParam, objectType);
    parameters.insert(parameters.begin(), thisParam);
    SetParamIndeces();
}

void SubroutineHeading::SetParamIndeces()
{
    int32_t index = 0;
    for (auto& parameter : parameters)
    {
        parameter.SetIndex(index++);
    }
}

std::vector<Type*> SubroutineHeading::ParameterTypes() const
{
    std::vector<Type*> parameterTypes;
    for (const auto& param : parameters)
    {
        parameterTypes.push_back(param.GetType());
    }
    return parameterTypes;
}

void SubroutineHeading::Write(Writer& writer)
{
    writer.GetBinaryWriter().Write(fullName);
    writer.GetBinaryWriter().Write(commonName);
    writer.GetBinaryWriter().Write(static_cast<uint8_t>(virtuality));
    writer.GetBinaryWriter().Write(static_cast<int32_t>(parameters.size()));
    for (auto& parameter : parameters)
    {
        parameter.Write(writer);
    }
    if (objectType)
    {
        writer.GetBinaryWriter().Write(objectType->Id());
    }
    else
    {
        writer.GetBinaryWriter().Write(util::uuid());
    }
}

void SubroutineHeading::Read(Reader& reader)
{
    fullName = reader.GetBinaryReader().ReadUtf8String();
    commonName = reader.GetBinaryReader().ReadUtf8String();
    virtuality = static_cast<Virtuality>(reader.GetBinaryReader().ReadByte());
    int32_t parameterCount = reader.GetBinaryReader().ReadInt();
    for (int32_t i = 0; i < parameterCount; ++i)
    {
        Parameter parameter;
        parameter.Read(reader);
        parameters.push_back(std::move(parameter));
    }
    reader.GetBinaryReader().ReadUuid(objectTypeId);
}

void SubroutineHeading::Resolve(Context* context)
{
    for (auto& parameter : parameters)
    {
        parameter.Resolve(context);
    }
    if (objectTypeId != util::uuid())
    {
        Type* type = context->GetBlock()->GetType(objectTypeId);
        if (type->IsObjectType())
        {
            objectType = static_cast<ObjectType*>(type);
        }
        else
        {
            throw std::runtime_error("object type expected");
        }
    }
}

void SubroutineHeading::SetCommonName(const std::string& commonName_)
{
    commonName = commonName_;
}

void SubroutineHeading::Print(util::CodeFormatter& formatter)
{
    if (virtuality == Virtuality::virtual_)
    {
        formatter.WriteLine("virtual");
    }
    else if (virtuality == Virtuality::override_)
    {
        formatter.WriteLine("override");
    }
    if (!commonName.empty())
    {
        formatter.WriteLine("common name '" + commonName + "'");
    }
    for (auto& parameter : parameters)
    {
        parameter.Print(formatter);
    }
    if (objectType)
    {
        formatter.WriteLine("object type '" + objectType->Name() + "'");
    }
}

Subroutine::Subroutine(SubroutineKind kind_, SubroutineHeading* heading_) : 
    kind(kind_), declarationKind(DeclarationKind::definition), forward(false), flags(SubroutineFlags::none), external(false), level(-1), nextTempVarIndex(0), heading(heading_), 
    declaration(nullptr), moduleId(-1), id(-1), implementationId(-1), vmtIndex(-1), frameSize(0), numBasicBlocks(0), code(nullptr)
{
}

Subroutine::~Subroutine()
{
}

void Subroutine::ResolveDeclaration(ParsingContext* context, soul::lexer::LexerBase<char>& lexer, int64_t pos)
{
}

void Subroutine::SetBlock(Block* block_)
{
    block.reset(block_);
    block->SetSubroutine(this);
}

void Subroutine::Write(Writer& writer)
{
    writer.GetBinaryWriter().Write(static_cast<uint8_t>(kind));
    writer.GetBinaryWriter().Write(static_cast<uint8_t>(declarationKind));
    writer.GetBinaryWriter().Write(static_cast<uint8_t>(heading->Kind()));
    heading->Write(writer);
    writer.GetBinaryWriter().Write(static_cast<uint8_t>(flags));
    writer.GetBinaryWriter().Write(forward);
    writer.GetBinaryWriter().Write(external);
    writer.GetBinaryWriter().Write(level);
    bool hasBlock = block != nullptr;
    writer.GetBinaryWriter().Write(hasBlock);
    if (hasBlock)
    {
        block->Write(writer);
    }
    writer.GetBinaryWriter().Write(moduleId);
    writer.GetBinaryWriter().Write(id);
    writer.GetBinaryWriter().Write(implementationId);
    writer.GetBinaryWriter().Write(vmtIndex);
    writer.GetBinaryWriter().Write(frameSize);
    writer.GetBinaryWriter().Write(numBasicBlocks);
    BasicBlock* bb = FirstBasicBlock();
    while (bb != nullptr)
    {
        bb->Write(writer);
        bb = bb->Next();
    }
}

void Subroutine::Read(Reader& reader)
{
    reader.ClearInstructionMap();
    kind = static_cast<SubroutineKind>(reader.GetBinaryReader().ReadByte());
    declarationKind = static_cast<DeclarationKind>(reader.GetBinaryReader().ReadByte());
    SubroutineHeadingKind headingKind = static_cast<SubroutineHeadingKind>(reader.GetBinaryReader().ReadByte());
    switch (headingKind)
    {
    case SubroutineHeadingKind::procedureHeading:
    {
        heading.reset(new ProcedureHeading());
        break;
    }
    case SubroutineHeadingKind::functionHeading:
    {
        heading.reset(new FunctionHeading());
        break;
    }
    case SubroutineHeadingKind::constructorHeading:
    {
        heading.reset(new ConstructorHeading());
        break;
    }
    }
    heading->Read(reader);
    flags = static_cast<SubroutineFlags>(reader.GetBinaryReader().ReadByte());
    forward = reader.GetBinaryReader().ReadBool();
    external = reader.GetBinaryReader().ReadBool();
    level = reader.GetBinaryReader().ReadInt();
    bool hasBlock = reader.GetBinaryReader().ReadBool();
    if (hasBlock)
    {
        block.reset(new Block(reader.GetBlock()));
        AddParameters(block.get());
        block->SetSubroutine(this);
        reader.PushBlock(block.get());
        block->Read(reader);
        reader.PopBlock();
    }
    moduleId = reader.GetBinaryReader().ReadInt();
    id = reader.GetBinaryReader().ReadInt();
    implementationId = reader.GetBinaryReader().ReadInt();
    vmtIndex = reader.GetBinaryReader().ReadInt();
    frameSize = reader.GetBinaryReader().ReadInt();
    numBasicBlocks = reader.GetBinaryReader().ReadInt();
    for (int32_t i = 0; i < numBasicBlocks; ++i)
    {
        BasicBlock* bb = new BasicBlock(this);
        bb->Read(reader);
        code.AddChild(bb);
    }
    BasicBlock* bb = FirstBasicBlock();
    while (bb != nullptr)
    {
        bb->Resolve(reader);
        bb = bb->Next();
    }
}

void Subroutine::ResolveHeading(Context* context)
{
    heading->Resolve(context);
}

void Subroutine::Resolve(Context* context)
{
    ResolveHeading(context);
    if (block)
    {
        block->Resolve(context);
    }
}

void Subroutine::AddParameters(Block* block)
{
    for (auto& parameter : heading->Parameters())
    {
        block->AddParameter(&parameter);
    }
}

void Subroutine::SetParamIndeces()
{
    heading->SetParamIndeces();
}

Variable* Subroutine::MakeTempVar(Block* block, Type* type)
{
    Variable* tempVar = new Variable("@tmp" + std::to_string(nextTempVarIndex++));
    tempVar->SetType(type);
    block->AddVariable(tempVar);
    return tempVar;
}

void Subroutine::SetFrameSize(ParsingContext* context)
{
    Block* block = context->GetBlock();
    frameSize = ParameterCount() + block->NumVariables();
    if ((context->GetFlags() & Flags::initializationPart) != Flags::none)
    {
        ModulePart* interfacePart = context->GetModule()->GetInterfacePart();
        if (interfacePart)
        {
            frameSize += interfacePart->GetBlock()->NumVariables();
        }
        ModulePart* implementationPart = context->GetModule()->GetImplementationPart();
        if (implementationPart)
        {
            frameSize += implementationPart->GetBlock()->NumVariables();
        }
    }
}

BasicBlock* Subroutine::AddBasicBlock()
{
    BasicBlock* bb = new BasicBlock(this);
    code.AddChild(bb);
    ++numBasicBlocks;
    return bb;
}

void Subroutine::Execute(ExecutionContext* context)
{
    context->PushSubroutine(this);
    Frame* frame = new Frame(frameSize, Parameters(), this);
    context->PushFrame(frame);
    context->SetParentFrame(level, frame);
    BasicBlock* bb = FirstBasicBlock();
    if (bb)
    {
        bb->Execute(context);
    }
    context->PopFrame();
    context->SetParentFrame(level, nullptr);
    context->PopSubroutine();
}

void Subroutine::Print(util::CodeFormatter& formatter, ExecutionContext* context)
{
    switch (kind)
    {
        case SubroutineKind::procedure:
        {
            formatter.Write("procedure '");
            break;
        }
        case SubroutineKind::function:
        {
            formatter.Write("function '");
            break;
        }
        case SubroutineKind::constructor:
        {
            formatter.WriteLine("constructor");
            break;
        }
    }
    if (kind != SubroutineKind::constructor)
    {
        formatter.WriteLine(InfoName() + "'");
    }
    if (IsDeclaration())
    {
        formatter.WriteLine("declaration");
    }
    else if (IsDefinition())
    {
        formatter.WriteLine("definition");
    }
    heading->Print(formatter);
    if (forward)
    {
        formatter.WriteLine("forward");
    }
    if (external)
    {
        formatter.WriteLine("external");
    }
    if (moduleId != -1)
    {
        formatter.WriteLine("module id " + std::to_string(moduleId));
    }
    if (id != -1)
    {
        formatter.WriteLine("id " + std::to_string(id));
    }
    if (implementationId != -1)
    {
        formatter.WriteLine("implementation id " + std::to_string(implementationId));
    }
    formatter.WriteLine("frame size: " + std::to_string(frameSize));
    if (block)
    {
        block->Print(formatter, context);
    }
    BasicBlock* bb = FirstBasicBlock();
    if (bb)
    {
        formatter.WriteLine("code");
        formatter.IncIndent();
        while (bb)
        {
            bb->Print(formatter, context);
            bb = bb->Next();
        }
        formatter.DecIndent();
    }
    formatter.WriteLine();
}

std::string Subroutine::ExternalSubroutineName() const
{
    ObjectType* objectType = Heading()->GetObjectType();
    if (objectType)
    {
        return objectType->Name() + "." + CommonName();
    }
    else
    {
        return CommonName();
    }
}

void Subroutine::CheckInterface()
{
    if (IsDeclaration())
    {
        if (!IsExternal())
        {
            if (ImplementationId() == -1)
            {
                throw std::runtime_error(SubroutineKindStr(Kind()) + " '" + InfoName() + "' implementation is missing");
            }
        }
    }
    if (block)
    {
        block->CheckInterface();
    }
}

void Subroutine::MakeVmts(Context* context)
{
    if (block)
    {
        block->MakeVmts(context);
    }
}

ProcedureHeading::ProcedureHeading(const std::string& fullName_) : SubroutineHeading(SubroutineHeadingKind::procedureHeading, fullName_)
{
}

ProcedureHeading::ProcedureHeading() : SubroutineHeading(SubroutineHeadingKind::procedureHeading)
{
}

ProcedureHeading::ProcedureHeading(ParsingContext* context, QualifiedIdNode* qualifiedId, soul::lexer::LexerBase<char>& lexer, int64_t pos) :
    SubroutineHeading(SubroutineHeadingKind::procedureHeading, MakeFullName(qualifiedId))
{
    SetCommonName(qualifiedId->MethodId()->Str());
    if (qualifiedId->ObjectTypeId())
    {
        std::string objectName = qualifiedId->ObjectTypeId()->Str();
        Type* type = context->GetBlock()->GetType(objectName);
        if (type)
        {
            if (type->IsObjectType())
            {
                ObjectType* objectType = static_cast<ObjectType*>(type);
                SetObjectType(objectType);
                InsertThisParam();
            }
            else
            {
                ThrowError("error: object type expression expected", lexer, pos);
            }
        }
        else
        {
            ThrowError("error: object type '" + objectName + "' not found", lexer, pos);
        }
    }
}


Procedure::Procedure() : Subroutine(SubroutineKind::procedure, new ProcedureHeading())
{
}

Procedure::Procedure(ProcedureKind kind_, const std::string& fullName_) : Subroutine(SubroutineKind::procedure, new ProcedureHeading(fullName_)), kind(kind_)
{
}

Procedure::Procedure(ProcedureKind kind_, ProcedureHeading* heading_) : Subroutine(SubroutineKind::procedure, heading_), kind(kind_)
{
}

std::string Procedure::InfoName() const
{
    if (FullName() == CommonName())
    {
        ObjectType* objectType = Heading()->GetObjectType();
        if (objectType)
        {
            return objectType->Name() + "." + FullName();
        }
        else
        {
            return FullName();
        }
    }
    else
    {
        return FullName();
    }
}

void Procedure::Write(Writer& writer)
{
    Subroutine::Write(writer);
    writer.GetBinaryWriter().Write(static_cast<uint8_t>(kind));
}

void Procedure::Read(Reader& reader)
{
    Subroutine::Read(reader);
    kind = static_cast<ProcedureKind>(reader.GetBinaryReader().ReadByte());
}

void Procedure::ResolveDeclaration(ParsingContext* context, soul::lexer::LexerBase<char>& lexer, int64_t pos)
{
    if (IsDeclaration()) return;
    Subroutine* declaration = GetDeclaration();
    if (declaration)
    {
        declaration->SetModuleId(ModuleId());
        declaration->SetImplementationId(Id());
    }
    else
    {
        ObjectType* objectType = Heading()->GetObjectType();
        if (objectType)
        {
            Subroutine* subroutineDeclaration = objectType->GetMethod(CommonName());
            if (subroutineDeclaration)
            {
                SetDeclaration(subroutineDeclaration);
                if (IsExternal())
                {
                    subroutineDeclaration->SetExternal();
                }
                else
                {
                    subroutineDeclaration->SetModuleId(ModuleId());
                    subroutineDeclaration->SetImplementationId(Id());
                }
            }
            else
            {
                ThrowError("error: matching procedure not found", lexer, pos);
            }
        }
        else
        {
            Module* mod = context->GetModule();
            ModulePart* interfacePart = mod->GetInterfacePart();
            Block* block = nullptr;
            if (interfacePart)
            {
                block = interfacePart->GetBlock();
            }
            if (!block)
            {
                ModulePart* implementationPart = mod->GetImplementationPart();
                if (implementationPart)
                {
                    block = implementationPart->GetBlock();
                }
            }
            if (block)
            {
                Procedure* declaration = block->GetProcedure(CommonName());
                if (declaration)
                {
                    SetDeclaration(declaration);
                    if (IsExternal())
                    {
                        declaration->SetExternal();
                    }
                    else
                    {
                        declaration->SetModuleId(ModuleId());
                        declaration->SetImplementationId(Id());
                    }
                }
            }
        }
    }
}

FunctionHeading::FunctionHeading() : SubroutineHeading(SubroutineHeadingKind::functionHeading), resultType(nullptr)
{
}

FunctionHeading::FunctionHeading(const std::string& fullName_) : SubroutineHeading(SubroutineHeadingKind::functionHeading, fullName_), resultType(nullptr)
{
}

FunctionHeading::FunctionHeading(ParsingContext* context, QualifiedIdNode* qualifiedId, soul::lexer::LexerBase<char>& lexer, int64_t pos) :
    SubroutineHeading(SubroutineHeadingKind::functionHeading, MakeFullName(qualifiedId))
{
    SetCommonName(qualifiedId->MethodId()->Str());
    if (qualifiedId->ObjectTypeId())
    {
        std::string objectName = qualifiedId->ObjectTypeId()->Str();
        Type* type = context->GetBlock()->GetType(objectName);
        if (type)
        {
            if (type->IsObjectType())
            {
                ObjectType* objectType = static_cast<ObjectType*>(type);
                SetObjectType(objectType);
                InsertThisParam();
            }
            else
            {
                ThrowError("error: object type expression expected", lexer, pos);
            }
        }
        else
        {
            ThrowError("error: object type '" + objectName + "' not found", lexer, pos);
        }
    }
}

void FunctionHeading::Write(Writer& writer)
{
    SubroutineHeading::Write(writer);
    writer.GetBinaryWriter().Write(resultType->Id());
}

void FunctionHeading::Read(Reader& reader)
{
    SubroutineHeading::Read(reader);
    reader.GetBinaryReader().ReadUuid(resultTypeId);
}

void FunctionHeading::Resolve(Context* context)
{
    SubroutineHeading::Resolve(context);
    resultType = context->GetBlock()->GetType(resultTypeId);
}

Function::Function() : Subroutine(SubroutineKind::function, new FunctionHeading()), fnkind(FunctionKind::none)
{
}

Function::Function(FunctionKind kind_, const std::string& fullName_) : Subroutine(SubroutineKind::function, new FunctionHeading(fullName_)), fnkind(kind_)
{
}

Function::Function(FunctionKind kind_, FunctionHeading* heading_) : Subroutine(SubroutineKind::function, heading_), fnkind(kind_)
{
}

std::string Function::InfoName() const
{
    if (FullName() == CommonName())
    {
        ObjectType* objectType = Heading()->GetObjectType();
        if (objectType)
        {
            return objectType->Name() + "." + FullName();
        }
        else
        {
            return FullName();
        }
    }
    else
    {
        return FullName();
    }
}

Type* Function::ResultType(const std::vector<Type*>& argumentTypes) const
{
    return GetFunctionHeading()->ResultType();
}

Value* Function::Evaluate(const std::vector<std::unique_ptr<Value>>& argumentValues, soul::lexer::LexerBase<char>& lexer, int64_t pos)
{
    ThrowError("error: standard function expected", lexer, pos);
    return nullptr;
}

void Function::GenerateCode(Emitter* emitter, int64_t pos)
{
    ThrowError("error: not implemented", emitter->Lexer(), pos);
}

void Function::Write(Writer& writer)
{
    Subroutine::Write(writer);
    writer.GetBinaryWriter().Write(static_cast<uint8_t>(fnkind));
}

void Function::Read(Reader& reader)
{
    Subroutine::Read(reader);
    fnkind = static_cast<FunctionKind>(reader.GetBinaryReader().ReadByte());
}

void Function::ResolveDeclaration(ParsingContext* context, soul::lexer::LexerBase<char>& lexer, int64_t pos)
{
    if (IsDeclaration()) return;
    Subroutine* declaration = GetDeclaration();
    if (declaration)
    {
        declaration->SetModuleId(ModuleId());
        declaration->SetImplementationId(Id());
    }
    else
    {
        ObjectType* objectType = Heading()->GetObjectType();
        if (objectType)
        {
            Subroutine* subroutineDeclaration = objectType->GetMethod(CommonName());
            if (subroutineDeclaration)
            {
                SetDeclaration(subroutineDeclaration);
                if (IsExternal())
                {
                    subroutineDeclaration->SetExternal();
                }
                else
                {
                    subroutineDeclaration->SetModuleId(ModuleId());
                    subroutineDeclaration->SetImplementationId(Id());
                }
            }
            else
            {
                ThrowError("error: matching function not found", lexer, pos);
            }
        }
        else
        {
            Module* mod = context->GetModule();
            Block* block = nullptr;
            ModulePart* interfacePart = mod->GetInterfacePart();
            if (interfacePart)
            {
                block = interfacePart->GetBlock();
            }
            if (!block)
            {
                ModulePart* implementationPart = mod->GetImplementationPart();
                if (implementationPart)
                {
                    block = implementationPart->GetBlock();
                }
            }
            if (block)
            {
                Function* declaration = block->GetFunction(CommonName());
                if (declaration)
                {
                    SetDeclaration(declaration);
                    if (IsExternal())
                    {
                        declaration->SetExternal();
                    }
                    else
                    {
                        declaration->SetModuleId(ModuleId());
                        declaration->SetImplementationId(Id());
                    }
                }
            }
        }
    }
}

ConstructorHeading::ConstructorHeading() : SubroutineHeading(SubroutineHeadingKind::constructorHeading, "@constructor")
{
}

void ConstructorHeading::SetObjectName(ParsingContext* context, const std::string& objectName_, soul::lexer::LexerBase<char>& lexer, int64_t pos)
{
    objectName = objectName_;
    Type* type = context->GetBlock()->GetType(objectName);
    if (type)
    {
        if (type->IsObjectType())
        {
            ObjectType* objectType = static_cast<ObjectType*>(type);
            SetObjectType(objectType);
            InsertThisParam();
        }
        else
        {
            ThrowError("error: object type expression expected", lexer, pos);
        }
    }
    else
    {
        ThrowError("error: object type '" + objectName + "' not found", lexer, pos);
    }
}

void ConstructorHeading::Write(Writer& writer)
{
    SubroutineHeading::Write(writer);
    writer.GetBinaryWriter().Write(objectName);
}

void ConstructorHeading::Read(Reader& reader)
{
    SubroutineHeading::Read(reader);
    objectName = reader.GetBinaryReader().ReadUtf8String();
}

ConstructorCall::ConstructorCall(ConstructorCallKind kind_) : kind(kind_)
{
}

void ConstructorCall::AddArgument(Node* argument)
{
    arguments.push_back(std::unique_ptr<Node>(argument));
}

Constructor::Constructor() : Subroutine(SubroutineKind::constructor, new ConstructorHeading()), implementation(nullptr)
{
}

Constructor::Constructor(ConstructorHeading* heading_) : Subroutine(SubroutineKind::constructor, heading_), implementation(nullptr)
{
}

std::string Constructor::InfoName() const
{
    ObjectType* objectType = Heading()->GetObjectType();
    if (objectType)
    {
        return objectType->Name() + "." + FullName();
    }
    else
    {
        return FullName();
    }
}

void Constructor::Write(Writer& writer)
{
    Subroutine::Write(writer);
}

void Constructor::Read(Reader& reader)
{
    Subroutine::Read(reader);
}

void Constructor::SetConstructorCall(ConstructorCall* constructorCall_)
{
    constructorCall.reset(constructorCall_);
}

void Constructor::ResolveDeclaration(ParsingContext* context, soul::lexer::LexerBase<char>& lexer, int64_t pos)
{
    if (IsDeclaration()) return;
    ObjectType* objectType = Heading()->GetObjectType();
    if (objectType)
    {
        Constructor* ctorDeclaration = objectType->GetConstructor(Heading()->ParameterTypes());
        if (ctorDeclaration)
        {
            SetDeclaration(ctorDeclaration);
            ctorDeclaration->SetModuleId(ModuleId());
            ctorDeclaration->SetImplementationId(Id());
        }
        else
        {
            ThrowError("error: matching constructor not found", lexer, pos);
        }
    }
    else
    {
        ThrowError("error: object type expected", lexer, pos);
    }
}

Procedure* MakeProcedure(ParsingContext* context, const std::string& fullName, soul::lexer::LexerBase<char>& lexer, int64_t pos)
{
    Block* block = context->GetBlock();
    Procedure* procedure = new Procedure(ProcedureKind::regularproc, fullName);
    block->AddProcedure(procedure, lexer, pos);
    context->GetModulePart()->AddSubroutine(procedure);
    return procedure;
}

Procedure* MakeProcedure(ParsingContext* context, ProcedureHeading* heading, soul::lexer::LexerBase<char>& lexer, int64_t pos)
{
    Block* block = context->GetBlock();
    Procedure* procedure = new Procedure(ProcedureKind::regularproc, heading);
    block->AddProcedure(procedure, lexer, pos);
    context->GetModulePart()->AddSubroutine(procedure);
    return procedure;
}

Function* MakeFunction(ParsingContext* context, const std::string& fullName, soul::lexer::LexerBase<char>& lexer, int64_t pos)
{
    Block* block = context->GetBlock();
    Function* function = new Function(FunctionKind::regularfn, fullName);
    block->AddFunction(function, lexer, pos);
    context->GetModulePart()->AddSubroutine(function);
    return function;
}

Function* MakeFunction(ParsingContext* context, FunctionHeading* heading, soul::lexer::LexerBase<char>& lexer, int64_t pos)
{
    Block* block = context->GetBlock();
    Function* function = new Function(FunctionKind::regularfn, heading);
    block->AddFunction(function, lexer, pos);
    context->GetModulePart()->AddSubroutine(function);
    return function;
}

Constructor* MakeConstructor(ParsingContext* context, soul::lexer::LexerBase<char>& lexer, int64_t pos)
{
    Block* block = context->GetBlock();
    Constructor* constructor = new Constructor();
    block->AddConstructor(constructor, lexer, pos);
    context->GetModulePart()->AddSubroutine(constructor);
    return constructor;
}

Constructor* MakeConstructor(ParsingContext* context, ConstructorHeading* heading, soul::lexer::LexerBase<char>& lexer, int64_t pos)
{
    Block* block = context->GetBlock();
    Constructor* constructor = new Constructor(heading);
    block->AddConstructor(constructor, lexer, pos);
    context->GetModulePart()->AddSubroutine(constructor);
    return constructor;
}

std::string MakeFullName(QualifiedIdNode* qualifiedIdNode)
{
    std::string fullName;
    if (qualifiedIdNode->ObjectTypeId())
    {
        fullName.append(qualifiedIdNode->ObjectTypeId()->Str()).append(1, '.');
    }
    if (qualifiedIdNode->MethodId())
    {
        fullName.append(qualifiedIdNode->MethodId()->Str());
    }
    return fullName;
}

void AddParameters(SubroutineHeading* heading, ParameterQualifier qualifier, const std::vector<std::string>& identifiers, Type* type)
{
    for (const auto& identifier : identifiers)
    {
        Parameter parameter(identifier, qualifier, type);
        heading->AddParameter(parameter);
    }
}

} // namespace p
