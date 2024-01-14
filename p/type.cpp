// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.type;

import p.parsing_context;
import p.evaluator;
import p.writer;
import p.reader;
import p.constant;
import p.block;
import p.binder;
import p.fnrep;
import p.context;

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

Type::Type(TypeKind kind_) : kind(kind_), name(), id(util::uuid::random())
{
}

Type::Type(TypeKind kind_, const std::string& name_) : kind(kind_), name(name_), id(util::uuid::random())
{
}

Type::~Type()
{
}

void Type::Write(Writer& writer) 
{
    writer.GetBinaryWriter().Write(name);
    writer.GetBinaryWriter().Write(id);
}

void Type::Read(Reader& reader)
{
    name = reader.GetBinaryReader().ReadUtf8String();
    reader.GetBinaryReader().ReadUuid(id);
}

void Type::Resolve(Context* context)
{
}

void Type::SetId(const util::uuid& id_)
{
    id = id_;
}

void Type::Print(util::CodeFormatter& formatter, ExecutionContext* context)
{
    formatter.WriteLine("'" + name + "'");
}

void Type::CheckInterface()
{
}

void Type::MakeVmt(Context* context)
{
}

TypePtr::TypePtr(Type* type_) : type(type_)
{
}

OrdinalType::OrdinalType(TypeKind kind_) : Type(kind_)
{
}

OrdinalType::OrdinalType(TypeKind kind_, const std::string& name_) : Type(kind_, name_)
{
}

IntegerType::IntegerType() : OrdinalType(TypeKind::integerType, "integer")
{
    SetId(IntegerTypeId());
}

BooleanType::BooleanType() : OrdinalType(TypeKind::booleanType, "boolean")
{
    SetId(BooleanTypeId());
}

CharType::CharType() : OrdinalType(TypeKind::charType, "char")
{
    SetId(CharTypeId());
}

RealType::RealType() : Type(TypeKind::realType, "real")
{
    SetId(RealTypeId());
}

StringType::StringType() : Type(TypeKind::stringType, "string")
{
    SetId(StringTypeId());
}

PointerType::PointerType() : Type(TypeKind::pointerType, "pointer")
{
    SetId(PointerTypeId());
}

NilType::NilType() : Type(TypeKind::nilType, "nil_type")
{
    SetId(NilTypeId());
}

EnumeratedType::EnumeratedType() : OrdinalType(TypeKind::enumeratedType)
{
}

EnumeratedType::EnumeratedType(const std::string& name_) : OrdinalType(TypeKind::enumeratedType, name_)
{
}

void EnumeratedType::AddEnumerationConstant(Constant* enumConstant)
{
    enumConstants.push_back(enumConstant);
}

void EnumeratedType::Write(Writer& writer)
{
    OrdinalType::Write(writer);
    writer.GetBinaryWriter().Write(static_cast<int32_t>(enumConstants.size()));
    for (const auto& enumConstant : enumConstants)
    {
        writer.GetBinaryWriter().Write(enumConstant->Id());
    }
}

void EnumeratedType::Read(Reader& reader)
{
    OrdinalType::Read(reader);
    int32_t enumConstantCount = reader.GetBinaryReader().ReadInt();
    for (int32_t i = 0; i < enumConstantCount; ++i)
    {
        util::uuid enumCconstantId;
        reader.GetBinaryReader().ReadUuid(enumCconstantId);
        enumConstantIds.push_back(enumCconstantId);
    }
}

void EnumeratedType::Resolve(Context* context)
{
    for (const auto& enumConstantId : enumConstantIds)
    {
        Constant* enumConstant = context->GetBlock()->GetConstant(enumConstantId);
        AddEnumerationConstant(enumConstant);
    }
}

void EnumeratedType::Print(util::CodeFormatter& formatter, ExecutionContext* context)
{
    formatter.Write("enumerated type ");
    Type::Print(formatter, context);
    formatter.WriteLine("(");
    formatter.IncIndent();
    for (const auto& enumConstant : enumConstants)
    {
        enumConstant->Print(formatter);
    }
    formatter.DecIndent();
    formatter.WriteLine(")");
}

SubrangeType::SubrangeType() : OrdinalType(TypeKind::subrangeType), rangeStart(), rangeEnd()
{
}

SubrangeType::SubrangeType(const std::string& name_, Value* rangeStart_, Value* rangeEnd_) : 
    OrdinalType(TypeKind::subrangeType, name_), rangeStart(rangeStart_), rangeEnd(rangeEnd_)
{
}

void SubrangeType::Write(Writer& writer)
{
    OrdinalType::Write(writer);
    writer.GetBinaryWriter().Write(static_cast<uint8_t>(rangeStart->Kind()));
    rangeStart->Write(writer);
    writer.GetBinaryWriter().Write(static_cast<uint8_t>(rangeEnd->Kind()));
    rangeEnd->Write(writer);
}

void SubrangeType::Read(Reader& reader)
{
    OrdinalType::Read(reader);
    ValueKind rangeStartKind = static_cast<ValueKind>(reader.GetBinaryReader().ReadByte());
    rangeStart.reset(MakeValue(rangeStartKind));
    rangeStart->Read(reader);
    ValueKind rangeEndKind = static_cast<ValueKind>(reader.GetBinaryReader().ReadByte());
    rangeEnd.reset(MakeValue(rangeEndKind));
    rangeEnd->Read(reader);
}

void SubrangeType::Print(util::CodeFormatter& formatter, ExecutionContext* context)
{
    formatter.Write("subrange type ");
    OrdinalType::Print(formatter, context);
    formatter.Write(rangeStart->ToValueString());
    formatter.Write("..");
    formatter.Write(rangeEnd->ToValueString());
    formatter.WriteLine();
}

ObjectType::ObjectType() : Type(TypeKind::objectType), flags(ObjectTypeFlags::none), vmtPtrFieldIndex(-1)
{
}

ObjectType::ObjectType(const std::string& name_) : Type(TypeKind::objectType, name_), flags(ObjectTypeFlags::none), vmtPtrFieldIndex(-1)
{
}

void ObjectType::AddFields(ObjectType* to)
{
    if (baseType)
    {
        baseType->AddFields(to);
    }
    int index = 0;
    for (const auto& fieldIndex : fieldIndexMap)
    {
        if (to->fieldIndexMap.find(fieldIndex.first) == to->fieldIndexMap.end())
        {
            to->fieldIndexMap.insert(fieldIndex);
            while (fieldIndex.second >= to->fieldTypes.size())
            {
                to->fieldTypes.push_back(nullptr);
            }
            to->fieldTypes[fieldIndex.second] = GetFieldType(fieldIndex.second);
        }
    }
}

void ObjectType::AddFields(const std::vector<std::string>& fieldNames, Type* type)
{
    for (const auto& fieldName : fieldNames)
    {
        if (GetFieldIndex(fieldName) == -1)
        {
            fieldIndexMap[fieldName] = fieldTypes.size();
            fieldTypes.push_back(type);
        }
        else
        {
            throw std::runtime_error("ObjectType.AddFields: field name '" + fieldName + " not unique");
        }
    }
}

int32_t ObjectType::GetFieldIndex(const std::string& fieldName) const
{
    auto it = fieldIndexMap.find(fieldName);
    if (it != fieldIndexMap.end())
    {
        return it->second;
    }
    else
    {
        return -1;
    }
}

Type* ObjectType::GetFieldType(const std::string& fieldName, bool throw_) const
{
    auto it = fieldIndexMap.find(fieldName);
    if (it != fieldIndexMap.end())
    {
        int32_t fieldIndex = it->second;
        if (fieldIndex >= 0 && fieldIndex < fieldTypes.size())
        {
            return fieldTypes[fieldIndex];
        }
        else
        {
            if (throw_)
            {
                throw std::runtime_error("field '" + fieldName + "' not found");
            }
        }
    }
    else
    {
        if (throw_)
        {
            throw std::runtime_error("field '" + fieldName + "' not found");
        }
    }
    return nullptr;
}

Type* ObjectType::GetFieldType(int32_t fieldIndex) const
{
    if (fieldIndex >= 0 && fieldIndex < fieldTypes.size())
    {
        return fieldTypes[fieldIndex];
    }
    else
    {
        throw std::runtime_error("invalid field index");
    }
}

void ObjectType::FinalizeLayout()
{
    if (!IsVirtual()) return;
    if (baseType && baseType->IsVirtual())
    {
        vmtPtrFieldIndex = baseType->vmtPtrFieldIndex;
    }
    else
    {
        fieldIndexMap["@vmt"] = fieldTypes.size();
        vmtPtrFieldIndex = fieldTypes.size();
        fieldTypes.push_back(nullptr);
    }
}

void ObjectType::GenerateDefaults(ParsingContext* context, soul::lexer::LexerBase<char>& lexer, int64_t pos)
{
    Constructor* defaultCtor = GetDefaultConstructor();
    if (!defaultCtor)
    {
        Subroutine* ctorMethod = AddMethod(context, new ConstructorHeading(), lexer, pos);
        ctorMethod->SetGenerated();
    }
}

Subroutine* ObjectType::AddMethod(ParsingContext* context, SubroutineHeading* methodHeading, soul::lexer::LexerBase<char>& lexer, int64_t pos)
{
    Subroutine* method = nullptr;
    if (methodHeading->GetVirtuality() != Virtuality::none)
    {
        SetVirtual();
    }
    if (methodHeading->IsProcedureHeading())
    {
        ProcedureHeading* procedureHeading = static_cast<ProcedureHeading*>(methodHeading);
        procedureHeading->SetObjectType(this);
        procedureHeading->InsertThisParam();
        Procedure* procedure = new Procedure(ProcedureKind::regularproc, procedureHeading);
        procedure->SetAsDeclaration();
        methods.push_back(std::unique_ptr<Subroutine>(procedure));
        method = procedure;
    }
    else if (methodHeading->IsFunctionHeading())
    {
        FunctionHeading* functionHeading = static_cast<FunctionHeading*>(methodHeading);
        functionHeading->SetObjectType(this);
        functionHeading->InsertThisParam();
        Function* function = new Function(FunctionKind::regularfn, functionHeading);
        function->SetAsDeclaration();
        methods.push_back(std::unique_ptr<Subroutine>(function));
        method = function;
    }
    else if (methodHeading->IsConstructorHeading())
    {
        ConstructorHeading* constructorHeading = static_cast<ConstructorHeading*>(methodHeading);
        constructorHeading->SetObjectType(this);
        constructorHeading->InsertThisParam();
        Constructor* constructor = new Constructor(constructorHeading);
        constructor->SetAsDeclaration();
        methods.push_back(std::unique_ptr<Subroutine>(constructor));
        method = constructor;
    }
    else
    {
        ThrowError("error: unknown heading kind", lexer, pos);
    }
    return method;
}

Constructor* ObjectType::GetDefaultConstructor() const
{
    std::vector<Type*> parameterTypes;
    parameterTypes.push_back(const_cast<ObjectType*>(this));
    return GetConstructor(parameterTypes);
}

Constructor* ObjectType::GetConstructor(const std::vector<Type*>& parameterTypes) const
{
    for (const auto& method : methods)
    {
        if (method->IsConstructor())
        {
            if (method->ParameterCount() == parameterTypes.size())
            {
                bool differ = false;
                int n = parameterTypes.size();
                for (int i = 0; i < n; ++i)
                {
                    if (method->Parameters()[i].GetType() != parameterTypes[i])
                    {
                        if (method->Parameters()[i].GetType()->IsObjectType() && parameterTypes[i]->IsObjectType())
                        {
                            ObjectType* methodParamObjectType = static_cast<ObjectType*>(method->Parameters()[i].GetType());
                            ObjectType* parameterObjectType = static_cast<ObjectType*>(parameterTypes[i]);
                            if (!parameterObjectType->IsSameOrHasBaseType(methodParamObjectType))
                            {
                                differ = true;
                                break;
                            }
                        }
                        else
                        {
                            differ = true;
                            break;
                        }
                    }
                }
                if (!differ)
                {
                    return static_cast<Constructor*>(method.get());
                }
            }
        }
    }
    return nullptr;
}

Constructor* ObjectType::GetConstructor(std::vector<std::unique_ptr<BoundExpressionNode>>& arguments, soul::lexer::LexerBase<char>& lexer, int64_t pos) const
{
    std::vector<Type*> argumentTypes;
    for (const auto& argument : arguments)
    {
        argumentTypes.push_back(argument->GetType());
    }
    Constructor* ctor = GetConstructor(argumentTypes);
    if (ctor)
    {
        return ctor;
    }
    for (const auto& method : methods)
    {
        if (method->IsConstructor())
        {
            if (method->ParameterCount() == arguments.size())
            {
                std::vector<std::unique_ptr<BoundExpressionNode>> newArguments;
                bool differ = false;
                int n = arguments.size();
                for (int i = 0; i < n; ++i)
                {
                    if (method->Parameters()[i].GetType() != arguments[i]->GetType())
                    {
                        if (method->Parameters()[i].GetType()->IsObjectType() && arguments[i]->GetType()->IsObjectType())
                        {
                            ObjectType* methodParamObjectType = static_cast<ObjectType*>(method->Parameters()[i].GetType());
                            ObjectType* argumentObjectType = static_cast<ObjectType*>(arguments[i]->GetType());
                            if (!argumentObjectType->IsSameOrHasBaseType(methodParamObjectType))
                            {
                                differ = true;
                                break;
                            }
                        }
                        Function* conversionFunction = GetConversionFunction(method->Parameters()[i].GetType(), arguments[i]->GetType(), lexer, pos, false);
                        if (!conversionFunction)
                        {
                            differ = true;
                            break;
                        }
                        newArguments.push_back(std::unique_ptr<BoundExpressionNode>(new BoundConversionNode(conversionFunction, arguments[i]->Clone(), pos)));
                    }
                    else
                    {
                        newArguments.push_back(std::unique_ptr<BoundExpressionNode>(arguments[i]->Clone()));
                    }
                }
                if (!differ)
                {
                    arguments = std::move(newArguments);
                    return static_cast<Constructor*>(method.get());
                }
            }
        }
    }
    return nullptr;
}

Subroutine* ObjectType::GetMethod(const std::string& methodName) const
{
    for (const auto& method : methods)
    {
        if (method->FullName() == methodName)
        {
            return method.get();
        }
    }
    return nullptr;
}

void ObjectType::Write(Writer& writer)
{
    Type::Write(writer);
    writer.GetBinaryWriter().Write(static_cast<uint8_t>(flags));
    writer.GetBinaryWriter().Write(baseType != nullptr);
    if (baseType)
    {
        writer.GetBinaryWriter().Write(baseType->Id());
    }
    int32_t fieldCount = fieldIndexMap.size();
    writer.GetBinaryWriter().Write(fieldCount);
    for (const auto& fieldIndex : fieldIndexMap)
    {
        const std::string& fieldName = fieldIndex.first;
        int32_t index = fieldIndex.second;
        writer.GetBinaryWriter().Write(fieldName);
        writer.GetBinaryWriter().Write(index);
    }
    int32_t fieldTypeCount = fieldTypes.size();
    writer.GetBinaryWriter().Write(fieldTypeCount);
    for (const auto& fieldType : fieldTypes)
    {
        if (fieldType)
        {
            writer.GetBinaryWriter().Write(fieldType->Id());
        }
        else
        {
            writer.GetBinaryWriter().Write(util::uuid());
        }
    }
    int32_t methodCount = methods.size();
    writer.GetBinaryWriter().Write(methodCount);
    for (const auto& method : methods)
    {
        writer.GetBinaryWriter().Write(static_cast<uint8_t>(method->Kind()));
        method->Write(writer);
    }
    if (IsVirtual())
    {
        writer.GetBinaryWriter().Write(vmtPtrFieldIndex);
        vmt.Write(writer);
    }
}

void ObjectType::Read(Reader& reader)
{
    Type::Read(reader);
    flags = static_cast<ObjectTypeFlags>(reader.GetBinaryReader().ReadByte());
    bool hasBaseType = reader.GetBinaryReader().ReadBool();
    if (hasBaseType)
    {
        reader.GetBinaryReader().ReadUuid(baseTypeId);
    }
    int32_t fieldCount = reader.GetBinaryReader().ReadInt();
    for (int32_t i = 0; i < fieldCount; ++i)
    {
        std::string fieldName = reader.GetBinaryReader().ReadUtf8String();
        int32_t fieldIndex = reader.GetBinaryReader().ReadInt();
        fieldIndexMap[fieldName] = fieldIndex;
    }
    int32_t fieldTypeCount = reader.GetBinaryReader().ReadInt();
    for (int32_t i = 0; i < fieldTypeCount; ++i)
    { 
        util::uuid fieldTypeId;
        reader.GetBinaryReader().ReadUuid(fieldTypeId);
        fieldTypeIds.push_back(fieldTypeId);
    }
    int32_t methodCount = reader.GetBinaryReader().ReadInt();
    for (int32_t i = 0; i < methodCount; ++i)
    {
        SubroutineKind kind = static_cast<SubroutineKind>(reader.GetBinaryReader().ReadByte());
        std::unique_ptr<Subroutine> method;
        switch (kind)
        {
            case SubroutineKind::procedure: method.reset(new Procedure()); break;
            case SubroutineKind::function: method.reset(new Function()); break;
            case SubroutineKind::constructor: method.reset(new Constructor()); break;
            default:
            {
                throw std::runtime_error("unknown subroutine kind");
            }
        }
        method->Read(reader);
        methods.push_back(std::unique_ptr<Subroutine>(method.release()));
    }
    if (IsVirtual())
    {
        vmtPtrFieldIndex = reader.GetBinaryReader().ReadInt();
        vmt.Read(reader);
    }
}

void ObjectType::Resolve(Context* context)
{
    if (IsResolved()) return;
    SetResolved();
    if (baseTypeId != util::uuid())
    {
        Type* type = context->GetBlock()->GetType(baseTypeId);
        if (type->IsObjectType())
        {
            baseType = static_cast<ObjectType*>(type);
        }
        else
        {
            throw std::runtime_error("object type expected");
        }
    }
    for (const auto& fieldTypeId : fieldTypeIds)
    {
        Type* fieldType = nullptr;
        if (fieldTypeId != util::uuid())
        {
            fieldType = context->GetBlock()->GetType(fieldTypeId);
        }
        fieldTypes.push_back(fieldType);
    }
    for (const auto& method : methods)
    {
        method->Resolve(context);
    }
    if (IsVirtual())
    {
        vmt.Resolve(context);
    }
}

void ObjectType::Print(util::CodeFormatter& formatter, ExecutionContext* context)
{
    formatter.Write("object type ");
    Type::Print(formatter, context);
    formatter.IncIndent();
    if (IsVirtual())
    {
        formatter.WriteLine("virtual");
    }
    if (baseType)
    {
        formatter.WriteLine("base type '" + baseType->Name() + "'");
    }
    formatter.WriteLine("fields:");
    formatter.IncIndent();
    for (const auto& field : fieldIndexMap)
    {
        std::string fieldTypeName = "null";
        Type* fieldType = fieldTypes[field.second];
        if (fieldType)
        {
            fieldTypeName = fieldType->Name();
        }
        formatter.WriteLine("'" + field.first + "' index " + std::to_string(field.second) + " type '" + fieldTypeName + "'");
    }
    formatter.DecIndent();
    formatter.WriteLine("methods:");
    formatter.IncIndent();
    for (const auto& method : methods)
    {
        method->Print(formatter, context);
    }
    formatter.DecIndent();
    formatter.DecIndent();
}

void ObjectType::CheckInterface()
{
    for (const auto& method : methods)
    {
        method->CheckInterface();
    }
}

void ObjectType::MakeVmt(Context* context)
{
    if (!IsVirtual()) return;
    p::MakeVmt(vmt, this, context);
}

bool ObjectType::IsSameOrHasBaseType(ObjectType* objectType) const
{
    ObjectType* thisType = const_cast<ObjectType*>(this);
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

Type* MakeSubrangeType(ParsingContext* context, Node* rangeStart, Node* rangeEnd, soul::lexer::LexerBase<char>& lexer, int64_t pos)
{
    if (context->MakeType())
    {
        Value* start = Evaluate(rangeStart, lexer, context->GetBlock());
        Value* end = Evaluate(rangeEnd, lexer, context->GetBlock());
        Type* type = new SubrangeType(context->TypeName(), start, end);
        context->GetBlock()->AddType(type, lexer, pos);
        return type;
    }
    else
    {
        ThrowError("error: must create", lexer, pos);
    }
    return nullptr;
}

Type* MakeEnumeratedType(ParsingContext* context, const std::vector<std::string>& enumConstantIds, soul::lexer::LexerBase<char>& lexer, int64_t pos)
{
    Block* block = context->GetBlock();
    if (context->MakeType())
    {
        EnumeratedType* enumeratedType = new EnumeratedType(context->TypeName());
        int32_t value = 0;
        for (const auto& enumConstantId : enumConstantIds)
        {
            EnumerationValue* enumValue = new EnumerationValue(enumConstantId, value);
            Constant* enumConstant = new Constant(enumConstantId);
            enumConstant->SetType(enumeratedType);
            enumConstant->SetValue(enumValue);
            block->AddConstant(enumConstant, lexer, pos);
            enumeratedType->AddEnumerationConstant(enumConstant);
            ++value;
        }
        block->AddType(enumeratedType, lexer, pos);
        return enumeratedType;
    }
    else
    {
        ThrowError("error: must create", lexer, pos);
    }
    return nullptr;
}

ObjectType* MakeObjectType(ParsingContext* context, soul::lexer::LexerBase<char>& lexer, int64_t pos)
{
    Block* block = context->GetBlock();
    if (context->MakeType())
    {
        ObjectType* objectType = new ObjectType(context->TypeName());
        block->AddType(objectType, lexer, pos);
        return objectType;
    }
    else
    {
        ThrowError("error: must create", lexer, pos);
    }
    return nullptr;
}

void SetHeritage(ParsingContext* context, ObjectType* objectType, IdentifierNode* baseTypeId, soul::lexer::LexerBase<char>& lexer, int64_t pos)
{
    Block* block = context->GetBlock();
    Type* type = block->GetType(baseTypeId->Str(), lexer, pos);
    if (type->IsObjectType())
    {
        ObjectType* baseType = static_cast<ObjectType*>(type);
        if (baseType->IsVirtual())
        {
            objectType->SetVirtual();
        }
        objectType->SetBaseType(baseType);
        baseType->AddFields(objectType);
    }
}

ArrayType::ArrayType() : Type(TypeKind::arrayType)
{
}

ArrayType::ArrayType(const std::string& name_, Type* elementType_) : Type(TypeKind::arrayType, name_), elementType(elementType_)
{
}

void ArrayType::Write(Writer& writer)
{
    Type::Write(writer);
    writer.GetBinaryWriter().Write(elementType->Id());
}

void ArrayType::Read(Reader& reader)
{
    Type::Read(reader);
    reader.GetBinaryReader().ReadUuid(elementTypeId);
}

void ArrayType::Resolve(Context* context)
{
    elementType = context->GetBlock()->GetType(elementTypeId);
}

void ArrayType::Print(util::CodeFormatter& formatter, ExecutionContext* context)
{
    formatter.Write("array type ");
    Type::Print(formatter, context);
    formatter.IncIndent();
    formatter.WriteLine("element type " + elementType->Name());
    formatter.DecIndent();
    formatter.WriteLine();
}

ArrayType* MakeArrayType(ParsingContext* context, Type* elementType, soul::lexer::LexerBase<char>& lexer, int64_t pos)
{
    Block* block = context->GetBlock();
    ArrayType* arrayType = nullptr;
    if (context->MakeType())
    {
        arrayType = new ArrayType(context->TypeName(), elementType);
    }
    else
    {
        arrayType = new ArrayType("array of " + elementType->Name(), elementType);
    }
    block->AddType(arrayType, lexer, pos);
    return arrayType;
}

int32_t FieldCount(ObjectType* objectType)
{
    int32_t fieldCount = 0;
    ObjectType* baseType = objectType->BaseType();
    if (baseType)
    {
        fieldCount = FieldCount(baseType);
    }
    fieldCount += objectType->FieldCount();
    return fieldCount;
}

GlobalTypeMap::GlobalTypeMap()
{
}

void GlobalTypeMap::AddType(Type* type)
{
    typeMap[type->Id()] = type;
    typeNameMap[type->Name()] = type;
}

Type* GlobalTypeMap::GetType(const util::uuid& id) const
{
    auto it = typeMap.find(id);
    if (it != typeMap.end())
    {
        return it->second;
    }
    else
    {
        throw std::runtime_error("type not found from global type map");
    }
}

Type* GlobalTypeMap::GetType(const std::string& typeName) const
{
    auto it = typeNameMap.find(typeName);
    if (it != typeNameMap.end())
    {
        return it->second;
    }
    else
    {
        throw std::runtime_error("type '" + typeName + "' not found from global type map");
    }
}

Type* MakeType(TypeKind kind)
{
    Type* type = nullptr;
    switch (kind)
    {
        case TypeKind::booleanType: type = new BooleanType(); break;
        case TypeKind::integerType: type = new IntegerType(); break;
        case TypeKind::charType: type = new CharType(); break;
        case TypeKind::enumeratedType: type = new EnumeratedType(); break;
        case TypeKind::subrangeType:type = new SubrangeType(); break;
        case TypeKind::realType:type = new RealType(); break;
        case TypeKind::stringType:type = new StringType(); break;
        case TypeKind::pointerType:type = new PointerType(); break;
        case TypeKind::nilType:type = new NilType(); break;
        case TypeKind::objectType:type = new ObjectType(); break;
        case TypeKind::arrayType:type = new ArrayType(); break;
    }
    if (type)
    {
        return type;
    }
    else
    {
        throw std::runtime_error("could not make type kind " + std::to_string(static_cast<uint8_t>(kind)));
    }
}

} // namespace p
