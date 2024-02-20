// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.core.object_type_symbol;

import p.ast;
import p.core.symbol_table;
import p.core.writer;
import p.core.reader;
import p.core.variable_symbol;
import p.core.subroutine_symbol;
import p.core.block_symbol;
import p.core.context;
import p.core.function_repository;

namespace p {

FieldSymbol::FieldSymbol(const soul::ast::Span& span_, const std::string& name_) : Symbol(SymbolKind::fieldSymbol, span_, name_), type(nullptr), index(-1)
{
}

FieldSymbol::FieldSymbol(const soul::ast::Span& span_, const std::string& name_, TypeSymbol* type_) : Symbol(SymbolKind::fieldSymbol, span_, name_), type(type_), index(-1)
{
}

void FieldSymbol::Write(SymbolWriter& writer)
{
    if (type)
    {
        writer.GetBinaryWriter().Write(type->Id());
    }
    else
    {
        writer.GetBinaryWriter().Write(util::uuid());
    }
    writer.GetBinaryWriter().Write(index);
}

void FieldSymbol::Read(SymbolReader& reader)
{
    reader.GetBinaryReader().ReadUuid(typeId);
    index = reader.GetBinaryReader().ReadInt();
}

void FieldSymbol::Resolve()
{
    Symbol::Resolve();
    SymbolTable* symbolTable = GetSymbolTable();
    if (typeId != util::nil_uuid())
    {
        type = symbolTable->GetType(typeId, SourceFilePath(), Span());
    }
}

void FieldSymbol::Print(util::CodeFormatter& formatter, bool full, ExecutionContext* context)
{
    Symbol::Print(formatter, full, context);
    if (full)
    {
        formatter.Write("type: ");
        if (type)
        {
            type->Print(formatter, false, context);
        }
        else
        {
            formatter.WriteLine("null");
        }
        formatter.WriteLine("index: " + std::to_string(index));
    }
}

TypeParamSymbol::TypeParamSymbol(const soul::ast::Span& span_, const std::string& name_) : TypeSymbol(SymbolKind::typeParamSymbol, span_, name_)
{
}

ObjectTypeSymbol::ObjectTypeSymbol(const soul::ast::Span& span_, const std::string& name_) : 
    TypeSymbol(SymbolKind::objectTypeSymbol, span_, name_), flags(ObjectTypeFlags::none), baseType(nullptr), vmtPtrFieldIndex(-1), typeParam(nullptr)
{
}

ObjectTypeSymbol::ObjectTypeSymbol(SymbolKind kind_, const soul::ast::Span& span_, const std::string& name_) : 
    TypeSymbol(kind_, span_, name_), flags(ObjectTypeFlags::none), baseType(nullptr), vmtPtrFieldIndex(-1), typeParam(nullptr)
{
}

void ObjectTypeSymbol::AddSymbol(Symbol* symbol)
{
    TypeSymbol::AddSymbol(symbol);
    if (symbol->IsFieldSymbol())
    {
        FieldSymbol* field = static_cast<FieldSymbol*>(symbol);
        field->SetIndex(fields.size());
        fields.push_back(field);
    }
    else if (symbol->IsSubroutineSymbol())
    {
        SubroutineSymbol* method = static_cast<SubroutineSymbol*>(symbol);
        methods.push_back(method);
    }
    else if (symbol->IsTypeParamSymbol())
    {
        typeParam = static_cast<TypeParamSymbol*>(symbol);
    }
}

Symbol* ObjectTypeSymbol::GetSymbol(const std::string& symbolName, Node* node, bool searchBase, bool mustExist) const
{
    Symbol* symbol = TypeSymbol::GetSymbol(symbolName, node, searchBase, false);
    if (symbol)
    {
        return symbol;
    }
    else if (searchBase && baseType)
    {
        return baseType->GetSymbol(symbolName, node, searchBase, mustExist);
    }
    else
    {
        if (mustExist)
        {
            ThrowError("error: symbol '" + symbolName + "' not found", node->FilePath(), node->Span());
        }
    }
    return nullptr;
}

void ObjectTypeSymbol::SetBaseType(ObjectTypeSymbol* baseType_)
{
    baseType = baseType_;
    if (baseType->IsVirtual())
    {
        SetVirtual();
    }
    baseType->AddFields(this);
}

void ObjectTypeSymbol::AddFields(ObjectTypeSymbol* objectTypeSymbol)
{
    if (baseType)
    {
        baseType->AddFields(objectTypeSymbol);
    }
    for (auto field : fields)
    {
        bool found = false;
        for (auto prevField : objectTypeSymbol->Fields())
        {
            if (field->Name() == prevField->Name())
            {
                found = true;
                break;
            }
        }
        if (!found)
        {
            objectTypeSymbol->AddSymbol(new FieldSymbol(field->Span(), field->Name(), field->Type()));
        }
    }
}

void ObjectTypeSymbol::Write(SymbolWriter& writer)
{
    TypeSymbol::Write(writer);
    writer.GetBinaryWriter().Write(static_cast<uint8_t>(flags));
    if (baseType)
    {
        writer.GetBinaryWriter().Write(baseType->Id());
    }
    else
    {
        writer.GetBinaryWriter().Write(util::uuid());
    }
    if (IsVirtual())
    {
        writer.GetBinaryWriter().Write(vmtPtrFieldIndex);
        vmt.Write(writer);
    }
    if (IsGeneric())
    {
        writer.WriteNode(objectTypeNode.get());
        subroutineNodes.Write(writer);
    }
}

void ObjectTypeSymbol::Read(SymbolReader& reader)
{
    TypeSymbol::Read(reader);
    flags = static_cast<ObjectTypeFlags>(reader.GetBinaryReader().ReadByte());
    reader.GetBinaryReader().ReadUuid(baseTypeId);
    if (IsVirtual())
    {
        vmtPtrFieldIndex = reader.GetBinaryReader().ReadInt();
        vmt.Read(reader);
    }
    if (IsGeneric())
    {
        objectTypeNode.reset(reader.ReadNode());
        subroutineNodes.Read(reader);
    }
}

void ObjectTypeSymbol::Resolve()
{
    TypeSymbol::Resolve();
    SymbolTable* symbolTable = GetSymbolTable();
    if (baseTypeId != util::nil_uuid())
    {
        TypeSymbol* type = symbolTable->GetType(baseTypeId, SourceFilePath(), Span());
        if (type->IsObjectTypeOrSpecializationSymbol())
        {
            baseType = static_cast<ObjectTypeSymbol*>(type);
        }
        else
        {
            ThrowError("error: object_type_symbol.resolve: object type expected for base type", SourceFilePath(), Span());
        }
    }
    if (IsVirtual())
    {
        vmt.Resolve(symbolTable);
    }
    if (!IsGeneric())
    {
        Context context;
        TypeSymbol* booleanType = symbolTable->GetType(GetBooleanTypeId(), &context);
        AddBinaryOperatorFunction(Span(), Operator::equal, this, booleanType);
        AddBinaryOperatorFunction(Span(), Operator::notEqual, this, booleanType);
    }
}

void ObjectTypeSymbol::MakeVmts()
{
    if (!IsVirtual()) return;
    p::MakeVmt(vmt, this);
}

int32_t ObjectTypeSymbol::GetFieldIndex(const std::string& fieldName) const
{
    FieldSymbol* field = ContainerSymbol::GetField(fieldName);
    return field->Index();
}

int32_t ObjectTypeSymbol::GetFieldIndexNoThrow(const std::string& fieldName) const
{
    FieldSymbol* field = ContainerSymbol::GetFieldNoThrow(fieldName);
    if (field)
    {
        return field->Index();
    }
    else
    {
        return -1;
    }
}

FieldSymbol* ObjectTypeSymbol::GetField(int32_t fieldIndex) const
{
    if (fieldIndex >= 0 && fieldIndex < fields.size())
    {
        return fields[fieldIndex];
    }
    else
    {
        ThrowError("error: invalid field index " + std::to_string(fieldIndex), SourceFilePath(), Span());
    }
    return nullptr;
}

ConstructorSymbol* ObjectTypeSymbol::GetDefaultConstructor() const
{
    std::vector<TypeSymbol*> parameterTypes;
    parameterTypes.push_back(const_cast<ObjectTypeSymbol*>(this));
    return GetConstructor(parameterTypes);
}

void ObjectTypeSymbol::GenerateDefaults(Node* node, Context* context)
{
    ConstructorSymbol* defaultCtor = GetDefaultConstructor();
    if (!defaultCtor)
    {
        std::vector<std::unique_ptr<ParameterSymbol>> parameters;
        ParameterSymbol* thisParameter = new ParameterSymbol(node->Span(), "this");
        thisParameter->SetType(this);
        parameters.push_back(std::unique_ptr<ParameterSymbol>(thisParameter));
        defaultCtor = GetOrInsertConstructor(parameters, node, false);
        defaultCtor->SetFrameSize(context);
        defaultCtor->SetGenerated();
        BlockSymbol* currentBlock = CurrentBlock();
        defaultCtor->SetLevel(currentBlock->Level() + 1);
    }
}

bool ObjectTypeSymbol::IsSameOrHasBaseType(ObjectTypeSymbol* objectType) const
{
    ObjectTypeSymbol* thisType = const_cast<ObjectTypeSymbol*>(this);
    if (thisType == objectType)
    {
        return true;
    }
    else if (baseType)
    {
        if (baseType->IsSameOrHasBaseType(objectType))
        {
            return true;
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

void ObjectTypeSymbol::Print(util::CodeFormatter& formatter, bool full, ExecutionContext* context)
{
    TypeSymbol::Print(formatter, full, context);
    if (full)
    {
        if (baseType)
        {
            formatter.Write("base type: ");
            baseType->Print(formatter, false, context);
        }
        if (vmtPtrFieldIndex != -1)
        {
            formatter.WriteLine("VMT pointer field index: " + std::to_string(vmtPtrFieldIndex));
        }
    }
}

void ObjectTypeSymbol::SetObjectTypeNode(Node* objectTypeNode_)
{
    objectTypeNode.reset(objectTypeNode_);
}

void ObjectTypeSymbol::AddSubroutineNode(Node* subroutineNode)
{
    subroutineNodes.Add(subroutineNode);
}

} // namespace p
