// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.core.subroutine_symbol;

import p.core.variable_symbol;
import p.core.type_symbol;
import p.core.object_type_symbol;
import p.core.symbol_table;
import p.core.block_symbol;
import p.core.writer;
import p.core.reader;
import p.core.emitter;
import p.core.context;
import p.core.execution_context;
import p.core.unit_part_symbol;
import p.core.function_repository;

namespace p {

std::string SubroutineFlagsStr(SubroutineFlags flags)
{
    std::string s;
    if ((flags & SubroutineFlags::external) != SubroutineFlags::none)
    {
        if (!s.empty())
        {
            s.append(1, ' ');
        }
        s.append("external");
    }
    if ((flags & SubroutineFlags::forward) != SubroutineFlags::none)
    {
        if (!s.empty())
        {
            s.append(1, ' ');
        }
        s.append("forward");
    }
    if ((flags & SubroutineFlags::standard) != SubroutineFlags::none)
    {
        if (!s.empty())
        {
            s.append(1, ' ');
        }
        s.append("standard");
    }
    if ((flags & SubroutineFlags::defined) != SubroutineFlags::none)
    {
        if (!s.empty())
        {
            s.append(1, ' ');
        }
        s.append("defined");
    }
    if ((flags & SubroutineFlags::generated) != SubroutineFlags::none)
    {
        if (!s.empty())
        {
            s.append(1, ' ');
        }
        s.append("generated");
    }
    return s;
}

SubroutineSymbol::SubroutineSymbol(SymbolKind kind_, const soul::ast::Span& span_, const std::string& name_) : 
    ContainerSymbol(kind_, span_, name_), flags(SubroutineFlags::none), id(util::uuid::random()), level(-1), vmtIndex(-1), 
    virtual_(Virtual::none), block(nullptr), externalSubroutineId(util::nil_uuid()), nextTempVarIndex(0), 
    frameSize(0), numBasicBlocks(0), code(nullptr)
{
}

SubroutineSymbol::SubroutineSymbol(SymbolKind kind_, const soul::ast::Span& span_, const std::string& name_, const util::uuid& id_) :
    ContainerSymbol(kind_, span_, name_), flags(SubroutineFlags::none), id(id_), level(-1), vmtIndex(-1), virtual_(Virtual::none), block(nullptr),
    externalSubroutineId(util::nil_uuid()), nextTempVarIndex(0), frameSize(0), numBasicBlocks(0), code(nullptr)
{
}

ObjectTypeSymbol* SubroutineSymbol::GetObjectType() const
{
    ContainerSymbol* parent = Parent();
    if (parent->IsObjectTypeSymbol())
    {
        return static_cast<ObjectTypeSymbol*>(parent);
    }
    else
    {
        return nullptr;
    }
}

void SubroutineSymbol::SetVirtual(Virtual virtual__, Node* node)
{
    if (virtual__ == Virtual::virtual_ || virtual__ == Virtual::override_)
    {
        ContainerSymbol* parent = Parent();
        if (parent->IsObjectTypeSymbol())
        {
            ObjectTypeSymbol* objectType = static_cast<ObjectTypeSymbol*>(parent);
            objectType->SetVirtual();
        }
        else
        {
            ThrowError("error: cannot set unit scope subroutine '" + Name() + "' to virtual or override", node->FilePath(), node->Span());
        }
    }
    virtual_ = virtual__;
}

void SubroutineSymbol::SetParameters(std::vector<std::unique_ptr<ParameterSymbol>>& parameters_)
{
    for (auto& parameter : parameters_)
    {
        AddSymbol(parameter.release());
    }
}

void SubroutineSymbol::CheckParameters(const std::vector<std::unique_ptr<ParameterSymbol>>& parameters_, Node* node)
{
    if (parameters.size() != parameters_.size())
    {
        ThrowError("error: conflicting number of parameters for subroutine '" + FullName() + "': " + std::to_string(parameters.size()) + " in declaration, " +
            std::to_string(parameters_.size()) + " in definition", node->FilePath(), node->Span());
    }
    int32_t n = parameters.size();
    for (int32_t i = 0; i < n; ++i)
    {
        ParameterSymbol* declarationParam = parameters[i];
        ParameterSymbol* definitionParam = parameters_[i].get();
        if (declarationParam->Qualifier() != definitionParam->Qualifier())
        {
            ThrowError("error: conflicting parameter " + std::to_string(i) + " qualifier for subroutine '" + FullName() + "': " + 
                ParameterQualifierStr(declarationParam->Qualifier()) + " in declaration, " + 
                ParameterQualifierStr(definitionParam->Qualifier()) + " in definition", node->FilePath(), node->Span());
        }
        if (declarationParam->Type() != definitionParam->Type())
        {
            ThrowError("error: conflicting parameter " + std::to_string(i) + " type for subroutine '" + FullName() + "': " +
                declarationParam->Type()->Name() + " in declaration, " +
                definitionParam->Type()->Name() + " in definition", node->FilePath(), node->Span());
        }
    }
}

void SubroutineSymbol::SetFrameSize(Context* context)
{
    frameSize = parameters.size() + variables.size();
}

bool SubroutineSymbol::Match(const std::vector<TypeSymbol*>& parameterTypes, int& numConversions) const
{
    if (parameters.size() != parameterTypes.size()) return false;
    numConversions = 0;
    int32_t n = parameterTypes.size();
    for (int32_t i = 0; i < n; ++i)
    {
        ParameterSymbol* left = parameters[i];
        TypeSymbol* right = parameterTypes[i];
        if (left->Type() != right)
        {
            FunctionSymbol* conversionFunction = GetConversionFunction(left->Type(), right, nullptr, false);
            if (conversionFunction)
            {
                ++numConversions;
            }
            else if (left->Type()->IsObjectTypeSymbol() && right->IsObjectTypeSymbol())
            {
                ObjectTypeSymbol* leftObjectType = static_cast<ObjectTypeSymbol*>(left->Type());
                ObjectTypeSymbol* rightObjectType = static_cast<ObjectTypeSymbol*>(right);
                if (rightObjectType->IsSameOrHasBaseType(leftObjectType))
                {
                    ++numConversions;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }
    }
    return true;
}

void SubroutineSymbol::AddSymbol(Symbol* symbol)
{
    ContainerSymbol::AddSymbol(symbol);
    if (symbol->IsParameterSymbol())
    {
        ParameterSymbol* parameter = static_cast<ParameterSymbol*>(symbol);
        parameter->SetIndex(parameters.size());
        parameters.push_back(parameter);
    }
    else if (symbol->IsVariableSymbol())
    {
        VariableSymbol* variable = static_cast<VariableSymbol*>(symbol);
        variable->SetIndex(parameters.size() + variables.size());
        variables.push_back(variable);
    }
    else if (symbol->IsBlockSymbol())
    {
        block = static_cast<BlockSymbol*>(symbol);
    }
}

void SubroutineSymbol::Write(SymbolWriter& writer)
{
    ContainerSymbol::Write(writer);
    writer.GetBinaryWriter().Write(static_cast<uint8_t>(flags));
    writer.GetBinaryWriter().Write(id);
    writer.GetBinaryWriter().Write(level);
    writer.GetBinaryWriter().Write(vmtIndex);
    writer.GetBinaryWriter().Write(static_cast<uint8_t>(virtual_));
    writer.GetBinaryWriter().Write(externalSubroutineId);
    writer.GetBinaryWriter().Write(frameSize);
    writer.GetBinaryWriter().Write(numBasicBlocks);
    BasicBlock* bb = FirstBasicBlock();
    while (bb != nullptr)
    {
        bb->Write(writer);
        bb = bb->Next();
    }
}

void SubroutineSymbol::Read(SymbolReader& reader)
{
    ContainerSymbol::Read(reader);
    flags = static_cast<SubroutineFlags>(reader.GetBinaryReader().ReadByte());
    reader.GetBinaryReader().ReadUuid(id);
    level = reader.GetBinaryReader().ReadInt();
    vmtIndex = reader.GetBinaryReader().ReadInt();
    virtual_ = static_cast<Virtual>(reader.GetBinaryReader().ReadByte());
    reader.GetBinaryReader().ReadUuid(externalSubroutineId);
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

void SubroutineSymbol::Execute(ExecutionContext* context)
{
    if (FullName() == "MakeProgramHeadingDiagram")
    {
        int x = 0;
    }
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

void SubroutineSymbol::SetExternalSubroutineId(const util::uuid& externalSubroutineId_)
{
    externalSubroutineId = externalSubroutineId_;
}

VariableSymbol* SubroutineSymbol::MakeTempVar(const soul::ast::Span& span, TypeSymbol* type)
{
    VariableSymbol* tempVar = new VariableSymbol(span, "@tmp" + std::to_string(nextTempVarIndex++));
    tempVar->SetType(type);
    AddSymbol(tempVar);
    return tempVar;
}

BasicBlock* SubroutineSymbol::AddBasicBlock()
{
    BasicBlock* bb = new BasicBlock(this);
    code.AddChild(bb);
    ++numBasicBlocks;
    return bb;
}

void SubroutineSymbol::MakeVmts()
{
    if (block)
    {
        block->MakeVmts();
    }
}

void SubroutineSymbol::CheckDefined()
{
    ContainerSymbol::CheckDefined();
    if (!IsStandard() && !IsGenerated() && !IsDefined())
    {
        ThrowError("implementation of subroutine '" + FullName() + "' is missing", SourceFilePath(), Span());
    }
    if (block)
    {
        block->CheckDefined();
    }
}

void SubroutineSymbol::Print(util::CodeFormatter& formatter, bool full, ExecutionContext* context)
{
    ContainerSymbol::Print(formatter, full, context);
    if (full)
    {
        formatter.WriteLine("flags: " + SubroutineFlagsStr(flags));
        formatter.WriteLine("id: " + util::ToString(id));
        if (level != -1)
        {
            formatter.WriteLine("level: " + std::to_string(level));
        }
        if (vmtIndex != -1)
        {
            formatter.WriteLine("VMT index: " + std::to_string(vmtIndex));
        }
        if (virtual_ != Virtual::none)
        {
            formatter.WriteLine("virtual: " + VirtualStr(virtual_));
        }
        if (block)
        {
            block->Print(formatter, full, context);
        }
        if (externalSubroutineId != util::nil_uuid())
        {
            formatter.WriteLine("external subroutine id: " + util::ToString(externalSubroutineId));
        }
        formatter.WriteLine("frame size: " + std::to_string(frameSize));
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
    }
}

ProcedureSymbol::ProcedureSymbol(const soul::ast::Span& span_, const std::string& name_) : SubroutineSymbol(SymbolKind::procedureSymbol, span_, name_)
{
}

ProcedureSymbol::ProcedureSymbol(SymbolKind kind_, const soul::ast::Span& span_, const std::string& name_) : SubroutineSymbol(kind_, span_, name_)
{
}

FunctionSymbol::FunctionSymbol(const soul::ast::Span& span_, const std::string& name_) : SubroutineSymbol(SymbolKind::functionSymbol, span_, name_), resultType(nullptr)
{
}

FunctionSymbol::FunctionSymbol(SymbolKind kind_, const soul::ast::Span& span_, const std::string& name_) : SubroutineSymbol(kind_, span_, name_), resultType(nullptr)
{
}

void FunctionSymbol::CheckResultType(TypeSymbol* resultType_, Node* node)
{
    if (resultType != resultType_)
    {
        ThrowError("conflicting result type for function '" + FullName() + "': '" + resultType->Name() + "' in declaration, '" + resultType_->Name() + "' in definition",
            node->FilePath(), node->Span());
    }
}

TypeSymbol* FunctionSymbol::ResultType(const std::vector<TypeSymbol*>& argumentTypes, Node* node) const
{
    if (resultType)
    {
        return resultType;
    }
    else if (!argumentTypes.empty())
    {
        return argumentTypes.front();
    }
    else
    {
        return nullptr;
    }
}

void FunctionSymbol::GenerateCode(Emitter* emitter, const soul::ast::Span& span)
{
    ThrowError("error: standard function expected", SourceFilePath(), span);
}

Value* FunctionSymbol::Evaluate(const std::vector<std::unique_ptr<Value>>& argumentValues, Context* context, Node* node) const
{
    ThrowError("error: standard function expected", node->FilePath(), node->Span());
    return nullptr;
}

void FunctionSymbol::Write(SymbolWriter& writer)
{
    SubroutineSymbol::Write(writer);
    if (resultType)
    {
        writer.GetBinaryWriter().Write(resultType->Id());
    }
    else
    {
        writer.GetBinaryWriter().Write(util::nil_uuid());
    }
}

void FunctionSymbol::Read(SymbolReader& reader)
{
    SubroutineSymbol::Read(reader);
    reader.GetBinaryReader().ReadUuid(resultTypeId);
}

void FunctionSymbol::Resolve()
{
    SubroutineSymbol::Resolve();
    SymbolTable* symbolTable = GetSymbolTable();
    if (resultTypeId != util::nil_uuid())
    {
        resultType = symbolTable->GetType(resultTypeId, SourceFilePath(), Span());
    }
}

ConstructorSymbol::ConstructorSymbol(const soul::ast::Span& span_, const std::string& name_) : 
    SubroutineSymbol(SymbolKind::constructorSymbol, span_, name_), constructorCall(nullptr)
{
}

ObjectTypeSymbol* ConstructorSymbol::GetObjectType() const
{
    ContainerSymbol* parent = Parent();
    if (parent->IsConstructorGroupSymbol())
    {
        ConstructorGroupSymbol* constructorGroupSymbol = static_cast<ConstructorGroupSymbol*>(parent);
        parent = constructorGroupSymbol->Parent();
        if (parent->IsObjectTypeSymbol())
        {
            return static_cast<ObjectTypeSymbol*>(parent);
        }
        else
        {
            return nullptr;
        }
    }
    else
    {
        return nullptr;
    }
}

struct BetterConversion
{
    bool operator()(const std::pair<ConstructorSymbol*, int>& left, const std::pair<ConstructorSymbol*, int>& right) const
    {
        return left.second < right.second;
    }
};

ConstructorGroupSymbol::ConstructorGroupSymbol(const soul::ast::Span& span_, const std::string& name_) : ContainerSymbol(SymbolKind::constructorGroupSymbol, span_, name_)
{
}

ConstructorSymbol* ConstructorGroupSymbol::GetConstructor(const std::vector<TypeSymbol*>& parameterTypes) const
{
    std::vector<std::pair<ConstructorSymbol*, int>> constructorConversionCounts;
    for (const auto& constructorSymbol : constructors)
    {
        int numConversions = 0;
        if (constructorSymbol->Match(parameterTypes, numConversions))
        {
            constructorConversionCounts.push_back(std::make_pair(constructorSymbol.get(), numConversions));
        }
    }
    if (constructorConversionCounts.size() == 1)
    {
        return constructorConversionCounts[0].first;
    }
    else if (constructorConversionCounts.size() > 1)
    {
        std::sort(constructorConversionCounts.begin(), constructorConversionCounts.end(), BetterConversion());
        if (BetterConversion()(constructorConversionCounts[0], constructorConversionCounts[1]))
        {
            return constructorConversionCounts[0].first;
        }
    }
    return nullptr;
}

ConstructorSymbol* ConstructorGroupSymbol::GetConstructor(const std::vector<std::unique_ptr<ParameterSymbol>>& parameters) const
{
    std::vector<TypeSymbol*> paramTypes;
    for (const auto& param : parameters)
    {
        paramTypes.push_back(param->Type());
    }
    return GetConstructor(paramTypes);
}

void ConstructorGroupSymbol::AddConstructor(ConstructorSymbol* constructor, Node* node)
{
    std::vector<std::pair<ConstructorSymbol*, int>> constructorConversionCounts;
    for (const auto& constructorSymbol : constructors)
    {
        std::vector<TypeSymbol*> paramTypes;
        for (auto param : constructorSymbol->Parameters())
        {
            paramTypes.push_back(param->Type());
        }
        int numConversions = 0;
        if (constructor->Match(paramTypes, numConversions))
        {
            if (numConversions == 0)
            {
                ThrowError("constructor with same parameter types already declared", node->FilePath(), node->Span());
            }
        }
    }
    constructor->SetParent(this);
    constructors.push_back(std::unique_ptr<ConstructorSymbol>(constructor));
}

void ConstructorGroupSymbol::Write(SymbolWriter& writer)
{
    int32_t n = constructors.size();
    writer.GetBinaryWriter().Write(n);
    for (const auto& constructorSymbol : constructors)
    {
        writer.WriteSymbol(constructorSymbol.get());
    }
}

void ConstructorGroupSymbol::Read(SymbolReader& reader)
{
    int32_t n = reader.GetBinaryReader().ReadInt();
    for (int32_t i = 0; i < n; ++i)
    {
        ConstructorSymbol* constructorSymbol = static_cast<ConstructorSymbol*>(reader.ReadSymbol(this));
        constructorSymbol->SetParent(this);
        constructors.push_back(std::unique_ptr<ConstructorSymbol>(constructorSymbol));
        SymbolTable* symbolTable = GetSymbolTable();
        symbolTable->MapSubroutine(constructorSymbol);
    }
}

void ConstructorGroupSymbol::CheckDefined()
{
    ContainerSymbol::CheckDefined();
    for (const auto& constructor : constructors)
    {
        constructor->CheckDefined();
    }
}

void ConstructorGroupSymbol::Print(util::CodeFormatter& formatter, bool full, ExecutionContext* context)
{
    ContainerSymbol::Print(formatter, full, context);
    if (full)
    {
        for (const auto& constructor : constructors)
        {
            constructor->Print(formatter, full, context);
        }
    }
}

} // namespace p
