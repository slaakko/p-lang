// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.core.instruction;

import p.core.object;
import p.core.value;
import p.core.execution_context;
import p.core.subroutine_symbol;
import p.core.writer;
import p.core.reader;
import p.core.standard_subroutine;
import p.core.type_symbol;
import p.core.heap;
import p.core.rt;
import p.core.constant_symbol;
import p.core.variable_symbol;
import p.core.symbol_table;
import p.core.object_type_symbol;
import p.core.array_type_symbol;
import p.core.vmt;

namespace p {

std::string InstructionKindStr(InstructionKind instructionKind)
{
    switch (instructionKind)
    {
        case InstructionKind::nop: return "nop";
        case InstructionKind::load_local: return "load_local";
        case InstructionKind::store_local: return "store_local";
        case InstructionKind::load_parent: return "load_parent";
        case InstructionKind::store_parent: return "store_parent";
        case InstructionKind::load_global: return "load_global";
        case InstructionKind::store_global: return "store_global";
        case InstructionKind::load_constant: return "load_constant";
        case InstructionKind::load_result_var: return "load_result_var";
        case InstructionKind::push_value: return "push_value";
        case InstructionKind::pop_value: return "pop_value";
        case InstructionKind::load_field: return "load_field";
        case InstructionKind::store_field: return "store_field";
        case InstructionKind::load_element: return "load_element";
        case InstructionKind::store_element: return "store_element";
        case InstructionKind::array_length: return "array_length";
        case InstructionKind::string_length: return "string_length";
        case InstructionKind::receive: return "receive";
        case InstructionKind::jump: return "jump";
        case InstructionKind::branch: return "branch";
        case InstructionKind::case_inst: return "case";
        case InstructionKind::call_subroutine: return "call_subroutine";
        case InstructionKind::call_virtual: return "call_virtual";
        case InstructionKind::call_external: return "call_external";
        case InstructionKind::new_object: return "new_object";
        case InstructionKind::new_array: return "new_array";
        case InstructionKind::call_ctor: return "call_ctor";
        case InstructionKind::equal_bool: return "equal_bool";
        case InstructionKind::not_equal_bool: return "not_equal_bool";
        case InstructionKind::and_bool: return "and_bool";
        case InstructionKind::or_bool: return "or_bool";
        case InstructionKind::xor_bool: return "xor_bool";
        case InstructionKind::not_bool: return "not_bool";
        case InstructionKind::equal_int: return "equal_int";
        case InstructionKind::not_equal_int: return "not_equal_int";
        case InstructionKind::less_int: return "less_int";
        case InstructionKind::greater_int: return "greater_int";
        case InstructionKind::less_or_equal_int: return "less_or_equal_int";
        case InstructionKind::greater_or_equal_int: return "greater_or_equal_int";
        case InstructionKind::plus_int: return "plus_int";
        case InstructionKind::minus_int: return "minus_int";
        case InstructionKind::mul_int: return "mul_int";
        case InstructionKind::fractional_divide_int: "fractional_divide_int";
        case InstructionKind::div_int: return "div_int";
        case InstructionKind::mod_int: return "mod_int";
        case InstructionKind::and_int: return "and_int";
        case InstructionKind::or_int: return "or_int";
        case InstructionKind::xor_int: return "xor_int";
        case InstructionKind::shl_int: return "shl_int";
        case InstructionKind::shr_int: return "shr_int";
        case InstructionKind::not_int: return "not_int";
        case InstructionKind::unary_plus_int: return "unary_plus_int";
        case InstructionKind::unary_minus_int: return "unary_minus_int";
        case InstructionKind::equal_real: return "equal_real";
        case InstructionKind::not_equal_real: return "not_equal_real";
        case InstructionKind::less_real: return "less_real";
        case InstructionKind::greater_real: return "greater_real";
        case InstructionKind::less_or_equal_real: return "less_or_equal_real";
        case InstructionKind::greater_or_equal_real: return "greater_or_equal_real";
        case InstructionKind::plus_real: return "plus_real";
        case InstructionKind::minus_real: return "minus_real";
        case InstructionKind::mul_real: return "mul_real";
        case InstructionKind::fractional_divide_real: return "fractional_divide_real";
        case InstructionKind::unary_plus_real: return "unary_plus_real";
        case InstructionKind::unary_minus_real: return "unary_minus_real";
        case InstructionKind::equal_char: return "equal_char";
        case InstructionKind::not_equal_char: return "not_equal_char";
        case InstructionKind::equal_string: return "equal_string";
        case InstructionKind::not_equal_string: return "not_equal_string";
        case InstructionKind::less_string: return "less_string";
        case InstructionKind::greater_string: return "greater_string";
        case InstructionKind::less_or_equal_string: return "less_or_equal_string";
        case InstructionKind::greater_or_equal_string: return "greater_or_equal_string";
        case InstructionKind::plus_string: return "plus_string";
        case InstructionKind::int_to_real: return "int_to_real";
        case InstructionKind::real_to_int: return "real_to_int";
        case InstructionKind::char_to_string: return "char_to_string";
        case InstructionKind::identity: return "identity";
        case InstructionKind::equal_nil: return "equal_nil";
        case InstructionKind::equal_object: return "equal_object";
        case InstructionKind::not_equal_object: return "not_equal_object";
    }
    return std::string();
}

Instruction::Instruction(InstructionKind kind_) : kind(kind_), index(0)
{
}

void Instruction::Write(SymbolWriter& writer)
{
    writer.GetBinaryWriter().Write(index);
}

void Instruction::Read(SymbolReader& reader)
{
    index = reader.GetBinaryReader().ReadInt();
}

void Instruction::Resolve(SymbolReader& reader)
{
}

std::string Instruction::ToString(ExecutionContext* context) const
{
    return std::to_string(index) + " : " + InstructionKindStr(kind);
}

NoOperationInstruction::NoOperationInstruction() : Instruction(InstructionKind::nop)
{
}

Instruction* NoOperationInstruction::Execute(ExecutionContext* context)
{
    return Next();
}

LoadLocalInstruction::LoadLocalInstruction() : Instruction(InstructionKind::load_local), localIndex(-1)
{
}

void LoadLocalInstruction::SetLocalIndex(int32_t localIndex_)
{
    localIndex = localIndex_;
}

void LoadLocalInstruction::Write(SymbolWriter& writer)
{
    Instruction::Write(writer);
    writer.GetBinaryWriter().Write(localIndex);
}

void LoadLocalInstruction::Read(SymbolReader& reader)
{
    Instruction::Read(reader);
    localIndex = reader.GetBinaryReader().ReadInt();
}

std::string LoadLocalInstruction::ToString(ExecutionContext* context) const
{
    return Instruction::ToString(context) + "(" + std::to_string(localIndex) + ")";
}

Instruction* LoadLocalInstruction::Execute(ExecutionContext* context)
{
    Object* object = context->CurrentFrame()->GetObject(localIndex);
    Object* obj = nullptr;
    if (object)
    {
        obj = object->GetObject();
        obj->SetOwner(object->Owner());
        obj->SetOwnerIndex(object->OwnerIndex());
    }
    if (!obj)
    {
        obj = NilObject::Ptr();
    }
    context->GetStack()->Push(new PtrObject(obj));
    return Next();
}

StoreLocalInstruction::StoreLocalInstruction() : Instruction(InstructionKind::store_local), localIndex(-1)
{
}

void StoreLocalInstruction::SetLocalIndex(int32_t localIndex_)
{
    localIndex = localIndex_;
}

void StoreLocalInstruction::Write(SymbolWriter& writer)
{
    Instruction::Write(writer);
    writer.GetBinaryWriter().Write(localIndex);
}

void StoreLocalInstruction::Read(SymbolReader& reader)
{
    Instruction::Read(reader);
    localIndex = reader.GetBinaryReader().ReadInt();
}

std::string StoreLocalInstruction::ToString(ExecutionContext* context) const
{
    return Instruction::ToString(context) + "(" + std::to_string(localIndex) + ")";
}

Instruction* StoreLocalInstruction::Execute(ExecutionContext* context)
{
    std::unique_ptr<Object> object = context->GetStack()->Pop();
    Object* obj = object->GetObject();
    if (!obj)
    {
        obj = NilObject::Ptr();
    }
    context->CurrentFrame()->SetObject(localIndex, obj, context);
    return Next();
}

LoadParentInstruction::LoadParentInstruction() : Instruction(InstructionKind::load_parent), parentLevel(-1), variableIndex(-1)
{
}

void LoadParentInstruction::SetParentLevel(int32_t parentLevel_)
{
    parentLevel = parentLevel_;
}

void LoadParentInstruction::SetVariableIndex(int32_t variableIndex_)
{
    variableIndex = variableIndex_;
}

void LoadParentInstruction::Write(SymbolWriter& writer)
{
    Instruction::Write(writer);
    writer.GetBinaryWriter().Write(parentLevel);
    writer.GetBinaryWriter().Write(variableIndex);
}

void LoadParentInstruction::Read(SymbolReader& reader)
{
    Instruction::Read(reader);
    parentLevel = reader.GetBinaryReader().ReadInt();
    variableIndex = reader.GetBinaryReader().ReadInt();
}

std::string LoadParentInstruction::ToString(ExecutionContext* context) const
{
    return Instruction::ToString(context) + "(" + std::to_string(parentLevel) + ", " + std::to_string(variableIndex) + ")";
}

Instruction* LoadParentInstruction::Execute(ExecutionContext* context)
{
    Frame* frame = context->ParentFrame(parentLevel);
    Object* object = frame->GetObject(variableIndex);
    if (!object)
    {
        throw std::runtime_error("error: uninitialized local variable access in function '" + context->CurrentSubroutine()->FullName() + "' load_parent instruction " +
            std::to_string(InstIndex()));
    }
    context->GetStack()->Push(new PtrObject(object->GetObject()));
    return Next();
}

StoreParentInstruction::StoreParentInstruction() : Instruction(InstructionKind::store_parent), parentLevel(-1), variableIndex(-1)
{
}

void StoreParentInstruction::SetParentLevel(int32_t parentLevel_)
{
    parentLevel = parentLevel_;
}

void StoreParentInstruction::SetVariableIndex(int32_t variableIndex_)
{
    variableIndex = variableIndex_;
}

void StoreParentInstruction::Write(SymbolWriter& writer)
{
    Instruction::Write(writer);
    writer.GetBinaryWriter().Write(parentLevel);
    writer.GetBinaryWriter().Write(variableIndex);
}

void StoreParentInstruction::Read(SymbolReader& reader)
{
    Instruction::Read(reader);
    parentLevel = reader.GetBinaryReader().ReadInt();
    variableIndex = reader.GetBinaryReader().ReadInt();
}

std::string StoreParentInstruction::ToString(ExecutionContext* context) const
{
    return Instruction::ToString(context) + "(" + std::to_string(parentLevel) + ", " + std::to_string(variableIndex) + ")";
}

Instruction* StoreParentInstruction::Execute(ExecutionContext* context)
{
    std::unique_ptr<Object> object = context->GetStack()->Pop();
    Frame* frame = context->ParentFrame(parentLevel);
    frame->SetObject(variableIndex, object->GetObject(), context);
    return Next();
}

LoadGlobalInstruction::LoadGlobalInstruction() : Instruction(InstructionKind::load_global), globalId(util::nil_uuid())
{
}

void LoadGlobalInstruction::SetGlobalId(const util::uuid& globalId_)
{
    globalId = globalId_;
}

void LoadGlobalInstruction::Write(SymbolWriter& writer)
{
    Instruction::Write(writer);
    writer.GetBinaryWriter().Write(globalId);
}

void LoadGlobalInstruction::Read(SymbolReader& reader)
{
    Instruction::Read(reader);
    reader.GetBinaryReader().ReadUuid(globalId);
}

std::string LoadGlobalInstruction::ToString(ExecutionContext* context) const
{
    return Instruction::ToString(context) + "(" + util::ToString(globalId) + ")";
}

Instruction* LoadGlobalInstruction::Execute(ExecutionContext* context)
{
    int32_t index = context->GetGlobalVariableMap()->GetObjectIndex(globalId);
    if (index == -1)
    {
        throw std::runtime_error("error: LoadGlobalInstruction: object index not found");
    }
    Object* object = context->GetGlobalVariableMap()->GetObject(index);
    context->GetStack()->Push(new PtrObject(object->GetObject()));
    return Next();
}

StoreGlobalInstruction::StoreGlobalInstruction() : Instruction(InstructionKind::store_global), globalId(util::nil_uuid())
{
}

void StoreGlobalInstruction::SetGlobalId(const util::uuid& globalId_)
{
    globalId = globalId_;
}

void StoreGlobalInstruction::Write(SymbolWriter& writer)
{
    Instruction::Write(writer);
    writer.GetBinaryWriter().Write(globalId);
}

void StoreGlobalInstruction::Read(SymbolReader& reader)
{
    Instruction::Read(reader);
    reader.GetBinaryReader().ReadUuid(globalId);
}

std::string StoreGlobalInstruction::ToString(ExecutionContext* context) const
{
    return Instruction::ToString(context) + "(" + util::ToString(globalId) + ")";
}

Instruction* StoreGlobalInstruction::Execute(ExecutionContext* context)
{
    std::unique_ptr<Object> object = context->GetStack()->Pop();
    int32_t index = context->GetGlobalVariableMap()->GetObjectIndex(globalId);
    if (index == -1)
    {
        throw std::runtime_error("error: StoreGlobalInstruction: object index not found");
    }
    context->GetGlobalVariableMap()->SetObject(index, object->GetObject(), context);
    return Next();
}

LoadConstantInstruction::LoadConstantInstruction() : Instruction(InstructionKind::load_constant)
{
}

void LoadConstantInstruction::SetConstantId(const util::uuid& constantId_)
{
    constantId = constantId_;
}

void LoadConstantInstruction::Write(SymbolWriter& writer)
{
    Instruction::Write(writer);
    writer.GetBinaryWriter().Write(constantId);
}

void LoadConstantInstruction::Read(SymbolReader& reader)
{
    Instruction::Read(reader);
    reader.GetBinaryReader().ReadUuid(constantId);
}

std::string LoadConstantInstruction::ToString(ExecutionContext* context) const
{
    return Instruction::ToString(context) + "(" + util::ToString(constantId) + ")";
}

Instruction* LoadConstantInstruction::Execute(ExecutionContext* context)
{
    ConstantSymbol* constant = context->GetSymbolTable()->GetConstant(constantId);
    context->GetStack()->Push(new PtrObject(constant->GetValue()));
    return Next();
}

LoadResultVarInstruction::LoadResultVarInstruction() : Instruction(InstructionKind::load_result_var), resultVarIndex(-1)
{
}

void LoadResultVarInstruction::SetResultVarIndex(int32_t resultVarIndex_)
{
    resultVarIndex = resultVarIndex_;
}

void LoadResultVarInstruction::Write(SymbolWriter& writer)
{
    Instruction::Write(writer);
    writer.GetBinaryWriter().Write(resultVarIndex);
}

void LoadResultVarInstruction::Read(SymbolReader& reader)
{
    Instruction::Read(reader);
    resultVarIndex = reader.GetBinaryReader().ReadInt();
}

std::string LoadResultVarInstruction::ToString(ExecutionContext* context) const
{
    return Instruction::ToString(context) + "(" + std::to_string(resultVarIndex) + ")";
}

Instruction* LoadResultVarInstruction::Execute(ExecutionContext* context)
{
    Object* object = context->CurrentFrame()->GetObject(resultVarIndex);
    if (!object)
    {
        throw std::runtime_error("error: uninitialized local variable access in function '" + context->CurrentSubroutine()->FullName() + "' load_result_var instruction " +
            std::to_string(InstIndex()));
    }
    context->GetStack()->Push(object->GetObject()->Clone());
    return Next();
}

PushValueInstruction::PushValueInstruction() : Instruction(InstructionKind::push_value), value(nullptr)
{
}

void PushValueInstruction::SetValue(Value* value_)
{
    value.reset(value_);
}

void PushValueInstruction::Write(SymbolWriter& writer)
{
    Instruction::Write(writer);
    Value* val = value->GetValue();
    writer.GetBinaryWriter().Write(static_cast<uint8_t>(val->Kind()));
    val->Write(writer);
}

void PushValueInstruction::Read(SymbolReader& reader)
{
    Instruction::Read(reader);
    ValueKind kind = static_cast<ValueKind>(reader.GetBinaryReader().ReadByte());
    value.reset(MakeValue(kind));
    value->Read(reader);
}

std::string PushValueInstruction::ToString(ExecutionContext* context) const
{
    return Instruction::ToString(context) + "(" + value->ToValueString() + ")";
}

Instruction* PushValueInstruction::Execute(ExecutionContext* context)
{
    context->GetStack()->Push(new PtrObject(value->GetValue()));
    return Next();
}

PopValueInstruction::PopValueInstruction() : Instruction(InstructionKind::pop_value)
{
}

Instruction* PopValueInstruction::Execute(ExecutionContext* context)
{
    std::unique_ptr<Object> value = context->GetStack()->Pop();
    return Next();
}

LoadFieldInstruction::LoadFieldInstruction() : Instruction(InstructionKind::load_field), fieldIndex(-1)
{
}

void LoadFieldInstruction::SetFieldIndex(int32_t fieldIndex_)
{
    fieldIndex = fieldIndex_;
}

void LoadFieldInstruction::Write(SymbolWriter& writer)
{
    Instruction::Write(writer);
    writer.GetBinaryWriter().Write(fieldIndex);
}

void LoadFieldInstruction::Read(SymbolReader& reader)
{
    Instruction::Read(reader);
    fieldIndex = reader.GetBinaryReader().ReadInt();
}

std::string LoadFieldInstruction::ToString(ExecutionContext* context) const
{
    return Instruction::ToString(context) + "(" + std::to_string(fieldIndex) + ")";
}

Instruction* LoadFieldInstruction::Execute(ExecutionContext* context)
{
    std::unique_ptr<Object> object = context->GetStack()->Pop();
    Object* obj = object->GetObject();
    if (obj->IsHeapObject())
    {
        HeapObject* heapObject = static_cast<HeapObject*>(obj);
        Object* fieldValue = heapObject->GetField(fieldIndex);
        context->GetStack()->Push(new PtrObject(fieldValue->GetObject()));
    }
    else if (obj->IsValueObject())
    {
        Value* value = static_cast<Value*>(obj);
        if (value->IsObjectValue())
        {
            ObjectValue* objectValue = static_cast<ObjectValue*>(value);
            const Field& field = objectValue->GetField(fieldIndex);
            Value* fieldValue = field.GetValue();
            context->GetStack()->Push(new PtrObject(fieldValue));
        }
        else
        {
            throw std::runtime_error("error: object value expected in function '" + context->CurrentSubroutine()->FullName() + "' load_field instruction " +
                std::to_string(InstIndex()));
        }
    }
    else if (obj->IsNilObject())
    {
        throw std::runtime_error("error: nil reference in function '" + context->CurrentSubroutine()->FullName() + "' load_field instruction " +
            std::to_string(InstIndex()));
    }
    else
    {
        throw std::runtime_error("error: heap object or object value expected in function '" + context->CurrentSubroutine()->FullName() + "' load_field instruction " +
            std::to_string(InstIndex()));
    }
    return Next();
}

StoreFieldInstruction::StoreFieldInstruction() : Instruction(InstructionKind::store_field), fieldIndex(-1)
{
}

void StoreFieldInstruction::SetFieldIndex(int32_t fieldIndex_)
{
    fieldIndex = fieldIndex_;
}

void StoreFieldInstruction::Write(SymbolWriter& writer)
{
    Instruction::Write(writer);
    writer.GetBinaryWriter().Write(fieldIndex);
}

void StoreFieldInstruction::Read(SymbolReader& reader)
{
    Instruction::Read(reader);
    fieldIndex = reader.GetBinaryReader().ReadInt();
}

std::string StoreFieldInstruction::ToString(ExecutionContext* context) const
{
    return Instruction::ToString(context) + "(" + std::to_string(fieldIndex) + ")";
}

Instruction* StoreFieldInstruction::Execute(ExecutionContext* context)
{
    std::unique_ptr<Object> object = context->GetStack()->Pop();
    Object* obj = object->GetObject();
    if (obj->IsHeapObject())
    {
        HeapObject* heapObject = static_cast<HeapObject*>(obj);
        std::unique_ptr<Object> value = context->GetStack()->Pop();
        heapObject->SetField(fieldIndex, value->GetObject(), context);
    }
    else if (obj->IsValueObject())
    {
        Value* value = static_cast<Value*>(obj);
        if (value->IsObjectValue())
        {
            throw std::runtime_error("error: cannot store to constant object in function '" + context->CurrentSubroutine()->FullName() + "' store_field instruction " +
                std::to_string(InstIndex()));
        }
    }
    else if (obj->IsNilObject())
    {
        throw std::runtime_error("error: nil reference in function '" + context->CurrentSubroutine()->FullName() + "' store_field instruction " +
            std::to_string(InstIndex()));
    }
    else
    {
        throw std::runtime_error("error: heap object expected in function '" + context->CurrentSubroutine()->FullName() + "' store_field instruction " +
            std::to_string(InstIndex()));
    }
    return Next();
}

LoadElementInstruction::LoadElementInstruction() : Instruction(InstructionKind::load_element)
{
}

Instruction* LoadElementInstruction::Execute(ExecutionContext* context)
{
    std::unique_ptr<Object> object = context->GetStack()->Pop();
    Object* obj = object->GetObject();
    if (obj->IsArrayObject())
    {
        ArrayObject* arrayObject = static_cast<ArrayObject*>(obj);
        std::unique_ptr<Object> index = context->GetStack()->Pop();
        Object* indexObj = index->GetObject();
        if (indexObj->IsValueObject())
        {
            Value* indexValue = static_cast<Value*>(indexObj);
            if (indexValue->IsIntegerValue())
            {
                int32_t elementIndex = indexValue->ToInteger();
                Object* elementValue = arrayObject->GetElement(elementIndex);
                context->GetStack()->Push(new PtrObject(elementValue->GetObject()));
            }
            else
            {
                throw std::runtime_error("error: integer index object expected in function '" + context->CurrentSubroutine()->FullName() + "' load_element instruction " +
                    std::to_string(InstIndex()));
            }
        }
        else
        {
            throw std::runtime_error("error: value index object expected in function '" + context->CurrentSubroutine()->FullName() + "' load_element instruction " +
                std::to_string(InstIndex()));
        }
    }
    else if (obj->IsValueObject())
    {
        Value* value = static_cast<Value*>(obj);
        if (value->IsArrayValue())
        {
            ArrayValue* arrayValue = static_cast<ArrayValue*>(value);
            std::unique_ptr<Object> index = context->GetStack()->Pop();
            Object* indexObj = index->GetObject();
            if (indexObj->IsValueObject())
            {
                Value* indexValue = static_cast<Value*>(indexObj);
                if (indexValue->IsIntegerValue())
                {
                    int32_t elementIndex = indexValue->ToInteger();
                    Value* elementValue = arrayValue->GetElement(elementIndex);
                    context->GetStack()->Push(new PtrObject(elementValue));
                }
                else
                {
                    throw std::runtime_error("error: integer index object expected in function '" + context->CurrentSubroutine()->FullName() + "' load_element instruction " +
                        std::to_string(InstIndex()));
                }
            }
            else
            {
                throw std::runtime_error("error: value index object expected in function '" + context->CurrentSubroutine()->FullName() + "' load_element instruction " +
                    std::to_string(InstIndex()));
            }
        }
        else
        {
            throw std::runtime_error("error: array value expected in function '" + context->CurrentSubroutine()->FullName() + "' load_element instruction " +
                std::to_string(InstIndex()));
        }
    }
    else
    {
        throw std::runtime_error("error: array object expected in function '" + context->CurrentSubroutine()->FullName() + "' load_element instruction " +
            std::to_string(InstIndex()));
    }
    return Next();
}

StoreElementInstruction::StoreElementInstruction() : Instruction(InstructionKind::store_element)
{
}

Instruction* StoreElementInstruction::Execute(ExecutionContext* context)
{
    std::unique_ptr<Object> object = context->GetStack()->Pop();
    Object* obj = object->GetObject();
    if (obj->IsArrayObject())
    {
        ArrayObject* arrayObject = static_cast<ArrayObject*>(obj);
        std::unique_ptr<Object> index = context->GetStack()->Pop();
        Object* indexObj = index->GetObject();
        if (indexObj->IsValueObject())
        {
            Value* indexValue = static_cast<Value*>(indexObj);
            if (indexValue->IsIntegerValue())
            {
                int32_t elementIndex = indexValue->ToInteger();
                std::unique_ptr<Object> value = context->GetStack()->Pop();
                arrayObject->SetElement(elementIndex, value->GetObject(), context);
            }
            else
            {
                throw std::runtime_error("error: integer index object expected in function '" + context->CurrentSubroutine()->FullName() + "' store_element instruction " +
                    std::to_string(InstIndex()));
            }
        }
        else
        {
            throw std::runtime_error("error: value index object expected in function '" + context->CurrentSubroutine()->FullName() + "' store_element instruction " +
                std::to_string(InstIndex()));
        }
    }
    else if (obj->IsValueObject())
    {
        Value* value = static_cast<Value*>(obj);
        if (value->IsArrayValue())
        {
            ArrayValue* arrayValue = static_cast<ArrayValue*>(value);
            throw std::runtime_error("error: cannot store to constant array in function '" + context->CurrentSubroutine()->FullName() + "' store_element instruction " +
                std::to_string(InstIndex()));
        }
        else
        {
            throw std::runtime_error("error: array value expected in function '" + context->CurrentSubroutine()->FullName() + "' store_element instruction " +
                std::to_string(InstIndex()));
        }
    }
    else
    {
        throw std::runtime_error("error: array object expected in function '" + context->CurrentSubroutine()->FullName() + "' store_element instruction " +
            std::to_string(InstIndex()));
    }
    return Next();
}

ArrayLengthInstruction::ArrayLengthInstruction() : Instruction(InstructionKind::array_length)
{
}

Instruction* ArrayLengthInstruction::Execute(ExecutionContext* context)
{
    std::unique_ptr<Object> object = context->GetStack()->Pop();
    Object* obj = object->GetObject();
    if (obj->IsArrayObject())
    {
        ArrayObject* arrayObject = static_cast<ArrayObject*>(obj);
        int32_t length = arrayObject->Length();
        context->GetStack()->Push(new IntegerValue(length));
    }
    else if (obj->IsValueObject())
    {
        Value* value = static_cast<Value*>(obj);
        if (value->IsArrayValue())
        {
            ArrayValue* arrayValue = static_cast<ArrayValue*>(value);
            int32_t length = arrayValue->Length();
            context->GetStack()->Push(new IntegerValue(length));
        }
        else
        {
            throw std::runtime_error("error: array value expected in function '" + context->CurrentSubroutine()->FullName() + "' array_length instruction " +
                std::to_string(InstIndex()));
        }
    }
    else
    {
        throw std::runtime_error("error: array object expected in function '" + context->CurrentSubroutine()->FullName() + "' array_length instruction " +
            std::to_string(InstIndex()));
    }
    return Next();
}

StringLengthInstruction::StringLengthInstruction() : Instruction(InstructionKind::string_length)
{
}

Instruction* StringLengthInstruction::Execute(ExecutionContext* context)
{
    std::unique_ptr<Object> object = context->GetStack()->Pop();
    Object* obj = object->GetObject();
    if (obj->IsStringObject())
    {
        StringObject* stringObject = static_cast<StringObject*>(obj);
        int32_t length = stringObject->Length();
        context->GetStack()->Push(new IntegerValue(length));
    }
    else
    {
        throw std::runtime_error("error: string object expected in function '" + context->CurrentSubroutine()->FullName() + "' string_length instruction " +
            std::to_string(InstIndex()));
    }
    return Next();
}

ReceiveInstruction::ReceiveInstruction() : Instruction(InstructionKind::receive), count(-1)
{
}

void ReceiveInstruction::SetCount(int32_t count_)
{
    count = count_;
}

void ReceiveInstruction::Write(SymbolWriter& writer)
{
    Instruction::Write(writer);
    writer.GetBinaryWriter().Write(count);
}

void ReceiveInstruction::Read(SymbolReader& reader)
{
    Instruction::Read(reader);
    count = reader.GetBinaryReader().ReadInt();
}

std::string ReceiveInstruction::ToString(ExecutionContext* context) const
{
    return Instruction::ToString(context) + "(" + std::to_string(count) + ")";
}

Instruction* ReceiveInstruction::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    Frame* frame = context->CurrentFrame();
    for (int32_t i = 0; i < count; ++i)
    {
        std::unique_ptr<Object> object = stack->Pop();
        Object* obj = object->GetObject();
        int32_t index = count - i - 1;
        frame->SetObject(index, obj, context);
    }
    return Next();
}

JumpInstruction::JumpInstruction() : Instruction(InstructionKind::jump), target(nullptr), targetIndex(-1)
{
}

void JumpInstruction::SetTarget(Instruction* target_)
{
    target = target_;
}

void JumpInstruction::Write(SymbolWriter& writer)
{
    Instruction::Write(writer);
    writer.GetBinaryWriter().Write(target->InstIndex());
}

void JumpInstruction::Read(SymbolReader& reader)
{
    Instruction::Read(reader);
    targetIndex = reader.GetBinaryReader().ReadInt();
}

void JumpInstruction::Resolve(SymbolReader& reader)
{
    target = reader.GetInstruction(targetIndex);
}

std::string JumpInstruction::ToString(ExecutionContext* context) const
{
    return Instruction::ToString(context) + "(" + std::to_string(targetIndex) + ")";
}

Instruction* JumpInstruction::Execute(ExecutionContext* context)
{
    return target;
}

BranchInstruction::BranchInstruction() : Instruction(InstructionKind::branch), trueNextIndex(-1), falseNextIndex(-1), trueNext(nullptr), falseNext(nullptr)
{
}

void BranchInstruction::SetTrueNext(Instruction* trueNext_)
{
    trueNext = trueNext_;
}

void BranchInstruction::SetFalseNext(Instruction* falseNext_)
{
    falseNext = falseNext_;
}

void BranchInstruction::Write(SymbolWriter& writer)
{
    Instruction::Write(writer);
    writer.GetBinaryWriter().Write(trueNext->InstIndex());
    writer.GetBinaryWriter().Write(falseNext->InstIndex());
}

void BranchInstruction::Read(SymbolReader& reader)
{
    Instruction::Read(reader);
    trueNextIndex = reader.GetBinaryReader().ReadInt();
    falseNextIndex = reader.GetBinaryReader().ReadInt();
}

void BranchInstruction::Resolve(SymbolReader& reader)
{
    trueNext = reader.GetInstruction(trueNextIndex);
    falseNext = reader.GetInstruction(falseNextIndex);
}

std::string BranchInstruction::ToString(ExecutionContext* context) const
{
    return Instruction::ToString(context) + "(" + std::to_string(trueNextIndex) + ", " + std::to_string(falseNextIndex) + ")";
}

Instruction* BranchInstruction::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> object = stack->Pop();
    Object* obj = object->GetObject();
    if (obj->IsValueObject())
    {
        Value* value = static_cast<Value*>(obj);
        if (value->IsBooleanValue())
        {
            BooleanValue* booleanValue = static_cast<BooleanValue*>(value);
            if (booleanValue->GetBooleanValue())
            {
                return trueNext;
            }
            else
            {
                return falseNext;
            }
        }
        else
        {
            throw std::runtime_error("error: branch: Boolean value expected");
        }
    }
    else
    {
        throw std::runtime_error("error: branch: value object expected");
    }
}

Range::Range() : first(-1), last(-1)
{
}

Range::Range(int32_t first_, int32_t last_) : first(first_), last(last_)
{
}

void Range::Write(SymbolWriter& writer)
{
    writer.GetBinaryWriter().Write(first);
    writer.GetBinaryWriter().Write(last);
}

void Range::Read(SymbolReader& reader)
{
    first = reader.GetBinaryReader().ReadInt();
    last = reader.GetBinaryReader().ReadInt();
}

RangeList::RangeList() : next(nullptr), nextIndex(-1)
{
}

bool RangeList::Match(int32_t value) const
{
    for (const auto& range : ranges)
    {
        if (range.Match(value)) return true;
    }
    return false;
}

void RangeList::AddRange(const Range& range)
{
    ranges.push_back(range);
}

void RangeList::SetNext(Instruction* next_)
{
    next = next_;
}

void RangeList::Write(SymbolWriter& writer)
{
    int32_t rangeCount = ranges.size();
    writer.GetBinaryWriter().Write(rangeCount);
    for (auto& range : ranges)
    {
        range.Write(writer);
    }
    writer.GetBinaryWriter().Write(next->InstIndex());
}

void RangeList::Read(SymbolReader& reader)
{
    int32_t rangeCount = reader.GetBinaryReader().ReadInt();
    for (int32_t i = 0; i < rangeCount; ++i)
    {
        Range range;
        range.Read(reader);
        AddRange(range);
    }
    nextIndex = reader.GetBinaryReader().ReadInt();
}

void RangeList::Resolve(SymbolReader& reader)
{
    next = reader.GetInstruction(nextIndex);
}

CaseInstruction::CaseInstruction() : Instruction(InstructionKind::case_inst), elseInst(nullptr), elseInstIndex(-1)
{
}

void CaseInstruction::SetElseInst(Instruction* elseInst_)
{
    elseInst = elseInst_;
}

RangeList& CaseInstruction::AddRangeList()
{
    rangeLists.push_back(RangeList());
    return rangeLists.back();
}

void CaseInstruction::Write(SymbolWriter& writer)
{
    Instruction::Write(writer);
    int32_t rangeListCount = rangeLists.size();
    writer.GetBinaryWriter().Write(rangeListCount);
    for (auto& rangeList : rangeLists)
    {
        rangeList.Write(writer);
    }
    writer.GetBinaryWriter().Write(elseInst->InstIndex());
}

void CaseInstruction::Read(SymbolReader& reader)
{
    Instruction::Read(reader);
    int32_t rangeListCount = reader.GetBinaryReader().ReadInt();
    for (int32_t i = 0; i < rangeListCount; ++i)
    {
        RangeList& rangeList = AddRangeList();
        rangeList.Read(reader);
    }
    elseInstIndex = reader.GetBinaryReader().ReadInt();
}

void CaseInstruction::Resolve(SymbolReader& reader)
{
    for (auto& rangeList : rangeLists)
    {
        rangeList.Resolve(reader);
    }
    elseInst = reader.GetInstruction(elseInstIndex);
}

std::string CaseInstruction::ToString(ExecutionContext* context) const
{
    std::string nexts;
    bool first = true;
    for (const auto& rangeList : rangeLists)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            nexts.append(", ");
        }
        nexts.append(std::to_string(rangeList.Next()->InstIndex()));
    }
    return Instruction::ToString(context) + "(" + nexts + ")";
}

Instruction* CaseInstruction::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> object = stack->Pop();
    Object* obj = object->GetObject();
    int32_t value = -1;
    if (obj->IsValueObject())
    {
        Value* objectValue = static_cast<Value*>(obj);
        value = objectValue->ToInteger();
    }
    else
    {
        throw std::runtime_error("value expected");
    }
    for (const auto& rangeList : rangeLists)
    {
        if (rangeList.Match(value))
        {
            return rangeList.Next();
        }
    }
    return elseInst;
}

CallSubroutineInstruction::CallSubroutineInstruction() : Instruction(InstructionKind::call_subroutine), subroutine(nullptr), argumentCount(-1), subroutineId(util::nil_uuid())
{
}

void CallSubroutineInstruction::SetSubroutine(SubroutineSymbol* subroutine_)
{
    subroutine = subroutine_;
}

void CallSubroutineInstruction::SetArgumentCount(int32_t argumentCount_)
{
    argumentCount = argumentCount_;
}

void CallSubroutineInstruction::Write(SymbolWriter& writer)
{
    Instruction::Write(writer);
    subroutineId = subroutine->Id();
    if (subroutineId == util::nil_uuid())
    {
        throw std::runtime_error("ID not set in subroutine call '" + subroutine->FullName() + "'");
    }
    writer.GetBinaryWriter().Write(subroutineId);
    writer.GetBinaryWriter().Write(argumentCount);
}

void CallSubroutineInstruction::Read(SymbolReader& reader)
{
    Instruction::Read(reader);
    reader.GetBinaryReader().ReadUuid(subroutineId);
    argumentCount = reader.GetBinaryReader().ReadInt();
}

std::string CallSubroutineInstruction::ToString(ExecutionContext* context) const
{
    SymbolTable* symbolTable = context->GetSymbolTable();
    SubroutineSymbol* subroutine = symbolTable->GetSubroutine(subroutineId);
    return Instruction::ToString(context) + "(" + subroutine->FullName() + ", " + util::ToString(subroutineId) + ", " + std::to_string(argumentCount) + ")";
}

Instruction* CallSubroutineInstruction::Execute(ExecutionContext* context)
{
    SymbolTable* symbolTable = context->GetSymbolTable();
    SubroutineSymbol* subroutine = symbolTable->GetSubroutine(subroutineId);
    context->PushArgumentCount(argumentCount);
    subroutine->Execute(context);
    context->PopArgumentCount();
    return Next();
}

CallVirtualInstruction::CallVirtualInstruction() : Instruction(InstructionKind::call_virtual), methodId(util::nil_uuid()), vmtIndex(-1)
{
}

void CallVirtualInstruction::SetMethod(SubroutineSymbol* method_)
{
    method = method_;
}

void CallVirtualInstruction::Write(SymbolWriter& writer)
{
    Instruction::Write(writer);
    if (method->VmtIndex() == -1)
    {
        throw std::runtime_error("method '" + method->FullName() + " VMT index not set");
    }
    methodId = method->Id();
    if (methodId == util::nil_uuid())
    {
        throw std::runtime_error("method '" + method->FullName() + " id not set");
    }
    writer.GetBinaryWriter().Write(method->Id());
    writer.GetBinaryWriter().Write(method->VmtIndex());
}

void CallVirtualInstruction::Read(SymbolReader& reader)
{
    Instruction::Read(reader);
    reader.GetBinaryReader().ReadUuid(methodId);
    vmtIndex = reader.GetBinaryReader().ReadInt();
}

std::string CallVirtualInstruction::ToString(ExecutionContext* context) const
{
    SymbolTable* symbolTable = context->GetSymbolTable();
    SubroutineSymbol* method = symbolTable->GetSubroutine(methodId);
    return Instruction::ToString(context) + "(" + method->FullName() + ", " + std::to_string(vmtIndex) + ")";
}

Instruction* CallVirtualInstruction::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> object = stack->Pop();
    Object* obj = object->GetObject();
    if (!obj || obj->IsNilObject())
    {
        throw std::runtime_error("error: nil reference in function '" + context->CurrentSubroutine()->FullName() + "' call_virtual instruction " +
            std::to_string(InstIndex()));
    }
    if (obj->IsHeapObject())
    {
        HeapObject* heapObject = static_cast<HeapObject*>(obj);
        ObjectTypeSymbol* objectType = heapObject->GetType();
        int32_t vmtPtrFieldIndex = objectType->VmtPtrFieldIndex();
        Object* vmtPtrObject = heapObject->GetField(vmtPtrFieldIndex);
        if (vmtPtrObject->IsValueObject())
        {
            Value* vmtPtrValue = static_cast<Value*>(vmtPtrObject);
            if (vmtPtrValue->IsGenericPointerValue())
            {
                GenericPointerValue* vmtPtrPointerValue = static_cast<GenericPointerValue*>(vmtPtrValue);
                void* vmtPtrPointer = vmtPtrPointerValue->Pointer();
                Vmt* vmt = static_cast<Vmt*>(vmtPtrPointer);
                if (vmt)
                {
                    SubroutineSymbol* method = vmt->GetMethod(vmtIndex);
                    if (method)
                    {
                        method->Execute(context);
                    }
                    else
                    {
                        throw std::runtime_error("error: virtual method is null in function '" + context->CurrentSubroutine()->FullName() + "' call_virtual instruction " +
                            std::to_string(InstIndex()));
                    }
                }
                else
                {
                    throw std::runtime_error("error: VMT pointer is null in function '" + context->CurrentSubroutine()->FullName() + "' call_virtual instruction " +
                        std::to_string(InstIndex()));
                }
            }
            else
            {
                throw std::runtime_error("error: generic pointer value expected in function '" + context->CurrentSubroutine()->FullName() + "' call_virtual instruction " +
                    std::to_string(InstIndex()));
            }
        }
        else
        {
            throw std::runtime_error("error: value object expected in function '" + context->CurrentSubroutine()->FullName() + "' call_virtual instruction " +
                std::to_string(InstIndex()));
        }
    }
    else
    {
        throw std::runtime_error("error: heap object expected in function '" + context->CurrentSubroutine()->FullName() + "' call_virtual instruction " +
            std::to_string(InstIndex()));
    }
    return Next();
}

CallExternalInstruction::CallExternalInstruction() : Instruction(InstructionKind::call_external), id(util::nil_uuid()), subroutine(nullptr)
{
}

void CallExternalInstruction::SetSubroutine(SubroutineSymbol* subroutine_)
{
    subroutine = subroutine_;
}

void CallExternalInstruction::Write(SymbolWriter& writer)
{
    Instruction::Write(writer);
    ExternalSubroutineSymbol* externalSubroutine = GetExternalSubroutine(subroutine->GetExternalSubroutineId());
    if (externalSubroutine->Id() == util::nil_uuid())
    {
        throw std::runtime_error("subroutine ID not set in external call '" + subroutine->FullName() + "'");
    }
    id = externalSubroutine->Id();
    writer.GetBinaryWriter().Write(id);
}

void CallExternalInstruction::Read(SymbolReader& reader)
{
    Instruction::Read(reader);
    reader.GetBinaryReader().ReadUuid(id);
}

std::string CallExternalInstruction::ToString(ExecutionContext* context) const
{
    ExternalSubroutineSymbol* subroutine = GetExternalSubroutine(id);
    return Instruction::ToString(context) + "(" + subroutine->Name() + ", " + util::ToString(id) + ")";
}

Instruction* CallExternalInstruction::Execute(ExecutionContext* context)
{
    ExternalSubroutineSymbol* subroutine = GetExternalSubroutine(id);
    subroutine->Execute(context);
    return Next();
}

CallConstructorInstruction::CallConstructorInstruction() : Instruction(InstructionKind::call_ctor), constructor(nullptr)
{
}

void CallConstructorInstruction::SetConstructor(ConstructorSymbol* constructor_)
{
    constructor = constructor_;
}

void CallConstructorInstruction::Write(SymbolWriter& writer)
{
    Instruction::Write(writer);
    constructorId = constructor->Id();
    writer.GetBinaryWriter().Write(constructorId);
}

void CallConstructorInstruction::Read(SymbolReader& reader)
{
    Instruction::Read(reader);
    reader.GetBinaryReader().ReadUuid(constructorId);
}

std::string CallConstructorInstruction::ToString(ExecutionContext* context) const
{
    SymbolTable* symbolTable = context->GetSymbolTable();
    SubroutineSymbol* method = symbolTable->GetSubroutine(constructorId);
    return Instruction::ToString(context) + "(" + method->FullName() + ", " + util::ToString(constructorId) + ")";
}

Instruction* CallConstructorInstruction::Execute(ExecutionContext* context)
{
    SymbolTable* symbolTable = context->GetSymbolTable();
    SubroutineSymbol* method = symbolTable->GetSubroutine(constructorId);
    method->Execute(context);
    return Next();
}

NewObjectInstruction::NewObjectInstruction() : Instruction(InstructionKind::new_object), objectTypeId()
{
}

void NewObjectInstruction::SetObjectTypeId(const util::uuid& objectTypeId_)
{
    objectTypeId = objectTypeId_;
}

void NewObjectInstruction::Write(SymbolWriter& writer)
{
    Instruction::Write(writer);
    writer.GetBinaryWriter().Write(objectTypeId);
}

void NewObjectInstruction::Read(SymbolReader& reader)
{
    Instruction::Read(reader);
    reader.GetBinaryReader().ReadUuid(objectTypeId);
}

std::string NewObjectInstruction::ToString(ExecutionContext* context) const
{
    Heap* heap = context->GetHeap();
    Stack* stack = context->GetStack();
    SymbolTable* symbolTable = context->GetSymbolTable();
    TypeSymbol* type = symbolTable->GetType(objectTypeId, context);
    return Instruction::ToString(context) + "(" + type->Name() + ")";
}

Instruction* NewObjectInstruction::Execute(ExecutionContext* context)
{
    Heap* heap = context->GetHeap();
    Stack* stack = context->GetStack();
    SymbolTable* symbolTable = context->GetSymbolTable();
    TypeSymbol* type = symbolTable->GetType(objectTypeId, context);
    if (type->IsObjectTypeOrSpecializationSymbol())
    {
        ObjectTypeSymbol* objectType = static_cast<ObjectTypeSymbol*>(type);
        HeapObject* object = heap->Allocate(objectType, context);
        stack->Push(new HeapObjectPtr(object));
        stack->Dup();
    }
    else
    {
        throw std::runtime_error("error: object type expected in function '" + context->CurrentSubroutine()->FullName() + "' new_object instruction " +
            std::to_string(InstIndex()));
    }
    return Next();
}

NewArrayInstruction::NewArrayInstruction() : Instruction(InstructionKind::new_array), arrayTypeId()
{
}

void NewArrayInstruction::SetArrayTypeId(const util::uuid& arrayTypeId_)
{
    arrayTypeId = arrayTypeId_;
}

void NewArrayInstruction::Write(SymbolWriter& writer)
{
    Instruction::Write(writer);
    writer.GetBinaryWriter().Write(arrayTypeId);
}

void NewArrayInstruction::Read(SymbolReader& reader)
{
    Instruction::Read(reader);
    reader.GetBinaryReader().ReadUuid(arrayTypeId);
}

std::string NewArrayInstruction::ToString(ExecutionContext* context) const
{
    Heap* heap = context->GetHeap();
    Stack* stack = context->GetStack();
    SymbolTable* symbolTable = context->GetSymbolTable();
    TypeSymbol* type = symbolTable->GetType(arrayTypeId, context);
    return Instruction::ToString(context) + "(" + type->Name() + ")";
}

Instruction* NewArrayInstruction::Execute(ExecutionContext* context)
{
    Heap* heap = context->GetHeap();
    Stack* stack = context->GetStack();
    SymbolTable* symbolTable = context->GetSymbolTable();
    std::unique_ptr<Object> arrayLength = stack->Pop();
    Object* lengthObj = arrayLength->GetObject();
    int32_t length = -1;
    if (lengthObj->IsValueObject())
    {
        Value* lengthValue = static_cast<Value*>(lengthObj);
        if (lengthValue->IsIntegerValue())
        {
            length = lengthValue->ToInteger();
        }
        else
        {
            throw std::runtime_error("error: integer array length expected in function '" + context->CurrentSubroutine()->FullName() + "' new_array instruction " +
                std::to_string(InstIndex()));
        }
    }
    else
    {
        throw std::runtime_error("error: integer array length expected in function '" + context->CurrentSubroutine()->FullName() + "' new_array instruction " +
            std::to_string(InstIndex()));
    }
    TypeSymbol* type = symbolTable->GetType(arrayTypeId, context);
    if (type->IsArrayTypeSymbol())
    {
        ArrayTypeSymbol* arrayType = static_cast<ArrayTypeSymbol*>(type);
        ArrayObject* object = heap->AllocateArray(arrayType, length, context);
        stack->Push(new ArrayObjectPtr(object));
    }
    else
    {
        throw std::runtime_error("error: array type expected in function '" + context->CurrentSubroutine()->FullName() + "' new_array instruction " +
            std::to_string(InstIndex()));
    }
    return Next();
}

EqualBoolInstruction::EqualBoolInstruction() : Instruction(InstructionKind::equal_bool)
{
}

Instruction* EqualBoolInstruction::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> right = stack->Pop();
    std::unique_ptr<Object> left = stack->Pop();
    int32_t leftValue = left->GetObject()->ToBoolean();
    int32_t rightValue = right->GetObject()->ToBoolean();
    stack->Push(new BooleanValue(leftValue == rightValue));
    return Next();
}

NotEqualBoolInstruction::NotEqualBoolInstruction() : Instruction(InstructionKind::not_equal_bool)
{
}

Instruction* NotEqualBoolInstruction::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> right = stack->Pop();
    std::unique_ptr<Object> left = stack->Pop();
    int32_t leftValue = left->GetObject()->ToBoolean();
    int32_t rightValue = right->GetObject()->ToBoolean();
    stack->Push(new BooleanValue(leftValue != rightValue));
    return Next();
}

AndBoolInstruction::AndBoolInstruction() : Instruction(InstructionKind::and_bool)
{
}

Instruction* AndBoolInstruction::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> right = stack->Pop();
    std::unique_ptr<Object> left = stack->Pop();
    int32_t leftValue = left->GetObject()->ToBoolean();
    int32_t rightValue = right->GetObject()->ToBoolean();
    stack->Push(new BooleanValue(leftValue && rightValue));
    return Next();
}

OrBoolInstruction::OrBoolInstruction() : Instruction(InstructionKind::or_bool)
{
}

Instruction* OrBoolInstruction::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> right = stack->Pop();
    std::unique_ptr<Object> left = stack->Pop();
    int32_t leftValue = left->GetObject()->ToBoolean();
    int32_t rightValue = right->GetObject()->ToBoolean();
    stack->Push(new BooleanValue(leftValue || rightValue));
    return Next();
}

XorBoolInstruction::XorBoolInstruction() : Instruction(InstructionKind::xor_bool)
{
}

Instruction* XorBoolInstruction::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> right = stack->Pop();
    std::unique_ptr<Object> left = stack->Pop();
    int32_t leftValue = left->GetObject()->ToBoolean();
    int32_t rightValue = right->GetObject()->ToBoolean();
    stack->Push(new BooleanValue(leftValue ^ rightValue));
    return Next();
}

NotBoolInstruction::NotBoolInstruction() : Instruction(InstructionKind::not_bool)
{
}

Instruction* NotBoolInstruction::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> operand = stack->Pop();
    int32_t value = operand->GetObject()->ToBoolean();
    stack->Push(new BooleanValue(!value));
    return Next();
}

EqualIntInstruction::EqualIntInstruction() : Instruction(InstructionKind::equal_int)
{
}

Instruction* EqualIntInstruction::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> right = stack->Pop();
    std::unique_ptr<Object> left = stack->Pop();
    int32_t leftValue = left->GetObject()->ToInteger();
    int32_t rightValue = right->GetObject()->ToInteger();
    stack->Push(new BooleanValue(leftValue == rightValue));
    return Next();
}

NotEqualIntInstruction::NotEqualIntInstruction() : Instruction(InstructionKind::not_equal_int)
{
}

Instruction* NotEqualIntInstruction::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> right = stack->Pop();
    std::unique_ptr<Object> left = stack->Pop();
    int32_t leftValue = left->GetObject()->ToInteger();
    int32_t rightValue = right->GetObject()->ToInteger();
    stack->Push(new BooleanValue(leftValue != rightValue));
    return Next();
}

LessIntInstruction::LessIntInstruction() : Instruction(InstructionKind::less_int)
{
}

Instruction* LessIntInstruction::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> right = stack->Pop();
    std::unique_ptr<Object> left = stack->Pop();
    int32_t leftValue = left->GetObject()->ToInteger();
    int32_t rightValue = right->GetObject()->ToInteger();
    stack->Push(new BooleanValue(leftValue < rightValue));
    return Next();
}

GreaterIntInstruction::GreaterIntInstruction() : Instruction(InstructionKind::greater_int)
{
}

Instruction* GreaterIntInstruction::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> right = stack->Pop();
    std::unique_ptr<Object> left = stack->Pop();
    int32_t leftValue = left->GetObject()->ToInteger();
    int32_t rightValue = right->GetObject()->ToInteger();
    stack->Push(new BooleanValue(leftValue > rightValue));
    return Next();
}

LessOrEqualIntInstruction::LessOrEqualIntInstruction() : Instruction(InstructionKind::less_or_equal_int)
{
}

Instruction* LessOrEqualIntInstruction::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> right = stack->Pop();
    std::unique_ptr<Object> left = stack->Pop();
    int32_t leftValue = left->GetObject()->ToInteger();
    int32_t rightValue = right->GetObject()->ToInteger();
    stack->Push(new BooleanValue(leftValue <= rightValue));
    return Next();
}

GreaterOrEqualIntInstruction::GreaterOrEqualIntInstruction() : Instruction(InstructionKind::greater_or_equal_int)
{
}

Instruction* GreaterOrEqualIntInstruction::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> right = stack->Pop();
    std::unique_ptr<Object> left = stack->Pop();
    int32_t leftValue = left->GetObject()->ToInteger();
    int32_t rightValue = right->GetObject()->ToInteger();
    stack->Push(new BooleanValue(leftValue >= rightValue));
    return Next();
}

PlusIntInstruction::PlusIntInstruction() : Instruction(InstructionKind::plus_int)
{
}

Instruction* PlusIntInstruction::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> right = stack->Pop();
    std::unique_ptr<Object> left = stack->Pop();
    int32_t leftValue = left->GetObject()->ToInteger();
    int32_t rightValue = right->GetObject()->ToInteger();
    stack->Push(new IntegerValue(leftValue + rightValue));
    return Next();
}

MinusIntInstruction::MinusIntInstruction() : Instruction(InstructionKind::minus_int)
{
}

Instruction* MinusIntInstruction::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> right = stack->Pop();
    std::unique_ptr<Object> left = stack->Pop();
    int32_t leftValue = left->GetObject()->ToInteger();
    int32_t rightValue = right->GetObject()->ToInteger();
    stack->Push(new IntegerValue(leftValue - rightValue));
    return Next();
}

MultiplyIntInstruction::MultiplyIntInstruction() : Instruction(InstructionKind::mul_int)
{
}

Instruction* MultiplyIntInstruction::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> right = stack->Pop();
    std::unique_ptr<Object> left = stack->Pop();
    int32_t leftValue = left->GetObject()->ToInteger();
    int32_t rightValue = right->GetObject()->ToInteger();
    stack->Push(new IntegerValue(leftValue * rightValue));
    return Next();
}

FractionalDivideIntInstruction::FractionalDivideIntInstruction() : Instruction(InstructionKind::fractional_divide_int)
{
}

Instruction* FractionalDivideIntInstruction::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> right = stack->Pop();
    std::unique_ptr<Object> left = stack->Pop();
    int32_t leftValue = left->GetObject()->ToInteger();
    int32_t rightValue = right->GetObject()->ToInteger();
    stack->Push(new RealValue((1.0f * leftValue) / rightValue));
    return Next();
}

DivIntInstruction::DivIntInstruction() : Instruction(InstructionKind::div_int)
{
}

Instruction* DivIntInstruction::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> right = stack->Pop();
    std::unique_ptr<Object> left = stack->Pop();
    int32_t leftValue = left->GetObject()->ToInteger();
    int32_t rightValue = right->GetObject()->ToInteger();
    stack->Push(new IntegerValue(leftValue / rightValue));
    return Next();
}

ModIntInstruction::ModIntInstruction() : Instruction(InstructionKind::mod_int)
{
}

Instruction* ModIntInstruction::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> right = stack->Pop();
    std::unique_ptr<Object> left = stack->Pop();
    int32_t leftValue = left->GetObject()->ToInteger();
    int32_t rightValue = right->GetObject()->ToInteger();
    stack->Push(new IntegerValue(leftValue % rightValue));
    return Next();
}

AndIntInstruction::AndIntInstruction() : Instruction(InstructionKind::and_int)
{
}

Instruction* AndIntInstruction::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> right = stack->Pop();
    std::unique_ptr<Object> left = stack->Pop();
    int32_t leftValue = left->GetObject()->ToInteger();
    int32_t rightValue = right->GetObject()->ToInteger();
    stack->Push(new IntegerValue(leftValue & rightValue));
    return Next();
}

OrIntInstruction::OrIntInstruction() : Instruction(InstructionKind::or_int)
{
}

Instruction* OrIntInstruction::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> right = stack->Pop();
    std::unique_ptr<Object> left = stack->Pop();
    int32_t leftValue = left->GetObject()->ToInteger();
    int32_t rightValue = right->GetObject()->ToInteger();
    stack->Push(new IntegerValue(leftValue | rightValue));
    return Next();
}

XorIntInstruction::XorIntInstruction() : Instruction(InstructionKind::xor_int)
{
}

Instruction* XorIntInstruction::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> right = stack->Pop();
    std::unique_ptr<Object> left = stack->Pop();
    int32_t leftValue = left->GetObject()->ToInteger();
    int32_t rightValue = right->GetObject()->ToInteger();
    stack->Push(new IntegerValue(leftValue ^ rightValue));
    return Next();
}

ShlIntInstruction::ShlIntInstruction() : Instruction(InstructionKind::shl_int)
{
}

Instruction* ShlIntInstruction::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> right = stack->Pop();
    std::unique_ptr<Object> left = stack->Pop();
    int32_t leftValue = left->GetObject()->ToInteger();
    int32_t rightValue = right->GetObject()->ToInteger();
    stack->Push(new IntegerValue(leftValue << rightValue));
    return Next();
}

ShrIntInstruction::ShrIntInstruction() : Instruction(InstructionKind::shr_int)
{
}

Instruction* ShrIntInstruction::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> right = stack->Pop();
    std::unique_ptr<Object> left = stack->Pop();
    int32_t leftValue = left->GetObject()->ToInteger();
    int32_t rightValue = right->GetObject()->ToInteger();
    stack->Push(new IntegerValue(leftValue >> rightValue));
    return Next();
}

NotIntInstruction::NotIntInstruction() : Instruction(InstructionKind::not_int)
{
}

Instruction* NotIntInstruction::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> operand = stack->Pop();
    int32_t value = operand->GetObject()->ToInteger();
    stack->Push(new IntegerValue(~value));
    return Next();
}

UnaryPlusIntInstruction::UnaryPlusIntInstruction() : Instruction(InstructionKind::unary_plus_int)
{
}

Instruction* UnaryPlusIntInstruction::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> operand = stack->Pop();
    int32_t value = operand->GetObject()->ToInteger();
    stack->Push(new IntegerValue(+value));
    return Next();
}

UnaryMinusIntInstruction::UnaryMinusIntInstruction() : Instruction(InstructionKind::unary_minus_int)
{
}

Instruction* UnaryMinusIntInstruction::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> operand = stack->Pop();
    int32_t value = operand->GetObject()->ToInteger();
    stack->Push(new IntegerValue(-value));
    return Next();
}

EqualRealInstruction::EqualRealInstruction() : Instruction(InstructionKind::equal_real)
{
}

Instruction* EqualRealInstruction::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> right = stack->Pop();
    std::unique_ptr<Object> left = stack->Pop();
    float  leftValue = left->GetObject()->ToReal();
    float rightValue = right->GetObject()->ToReal();
    stack->Push(new BooleanValue(leftValue == rightValue));
    return Next();
}

NotEqualRealInstruction::NotEqualRealInstruction() : Instruction(InstructionKind::not_equal_real)
{
}

Instruction* NotEqualRealInstruction::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> right = stack->Pop();
    std::unique_ptr<Object> left = stack->Pop();
    float  leftValue = left->GetObject()->ToReal();
    float rightValue = right->GetObject()->ToReal();
    stack->Push(new BooleanValue(leftValue != rightValue));
    return Next();
}

LessRealInstruction::LessRealInstruction() : Instruction(InstructionKind::less_real)
{
}

Instruction* LessRealInstruction::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> right = stack->Pop();
    std::unique_ptr<Object> left = stack->Pop();
    float  leftValue = left->GetObject()->ToReal();
    float rightValue = right->GetObject()->ToReal();
    stack->Push(new BooleanValue(leftValue < rightValue));
    return Next();
}

GreaterRealInstruction::GreaterRealInstruction() : Instruction(InstructionKind::greater_real)
{
}

Instruction* GreaterRealInstruction::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> right = stack->Pop();
    std::unique_ptr<Object> left = stack->Pop();
    float  leftValue = left->GetObject()->ToReal();
    float rightValue = right->GetObject()->ToReal();
    stack->Push(new BooleanValue(leftValue > rightValue));
    return Next();
}

LessOrEqualRealInstruction::LessOrEqualRealInstruction() : Instruction(InstructionKind::less_or_equal_real)
{
}

Instruction* LessOrEqualRealInstruction::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> right = stack->Pop();
    std::unique_ptr<Object> left = stack->Pop();
    float  leftValue = left->GetObject()->ToReal();
    float rightValue = right->GetObject()->ToReal();
    stack->Push(new BooleanValue(leftValue <= rightValue));
    return Next();
}

GreaterOrEqualRealInstruction::GreaterOrEqualRealInstruction() : Instruction(InstructionKind::greater_or_equal_real)
{
}

Instruction* GreaterOrEqualRealInstruction::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> right = stack->Pop();
    std::unique_ptr<Object> left = stack->Pop();
    float  leftValue = left->GetObject()->ToReal();
    float rightValue = right->GetObject()->ToReal();
    stack->Push(new BooleanValue(leftValue >= rightValue));
    return Next();
}

PlusRealInstruction::PlusRealInstruction() : Instruction(InstructionKind::plus_real)
{
}

Instruction* PlusRealInstruction::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> right = stack->Pop();
    std::unique_ptr<Object> left = stack->Pop();
    float leftValue = left->GetObject()->ToReal();
    float rightValue = right->GetObject()->ToReal();
    stack->Push(new RealValue(leftValue + rightValue));
    return Next();
}

MinusRealInstruction::MinusRealInstruction() : Instruction(InstructionKind::minus_real)
{
}

Instruction* MinusRealInstruction::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> right = stack->Pop();
    std::unique_ptr<Object> left = stack->Pop();
    float leftValue = left->GetObject()->ToReal();
    float rightValue = right->GetObject()->ToReal();
    stack->Push(new RealValue(leftValue - rightValue));
    return Next();
}

MultiplyRealInstruction::MultiplyRealInstruction() : Instruction(InstructionKind::mul_real)
{
}

Instruction* MultiplyRealInstruction::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> right = stack->Pop();
    std::unique_ptr<Object> left = stack->Pop();
    float leftValue = left->GetObject()->ToReal();
    float rightValue = right->GetObject()->ToReal();
    stack->Push(new RealValue(leftValue * rightValue));
    return Next();
}

FractionalDivideRealInstruction::FractionalDivideRealInstruction() : Instruction(InstructionKind::fractional_divide_real)
{
}

Instruction* FractionalDivideRealInstruction::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> right = stack->Pop();
    std::unique_ptr<Object> left = stack->Pop();
    float leftValue = left->GetObject()->ToReal();
    float rightValue = right->GetObject()->ToReal();
    stack->Push(new RealValue(leftValue / rightValue));
    return Next();
}

UnaryPlusRealInstruction::UnaryPlusRealInstruction() : Instruction(InstructionKind::unary_plus_real)
{
}

Instruction* UnaryPlusRealInstruction::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> operand = stack->Pop();
    float value = operand->GetObject()->ToReal();
    stack->Push(new RealValue(+value));
    return Next();
}

UnaryMinusRealInstruction::UnaryMinusRealInstruction() : Instruction(InstructionKind::unary_minus_real)
{
}

Instruction* UnaryMinusRealInstruction::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> operand = stack->Pop();
    float value = operand->GetObject()->ToReal();
    stack->Push(new RealValue(-value));
    return Next();
}

EqualCharInstruction::EqualCharInstruction() : Instruction(InstructionKind::equal_char)
{
}

Instruction* EqualCharInstruction::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> right = stack->Pop();
    std::unique_ptr<Object> left = stack->Pop();
    char leftValue = left->GetObject()->ToChar();
    char rightValue = right->GetObject()->ToChar();
    stack->Push(new BooleanValue(leftValue == rightValue));
    return Next();
}

NotEqualCharInstruction::NotEqualCharInstruction() : Instruction(InstructionKind::not_equal_char)
{
}

Instruction* NotEqualCharInstruction::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> right = stack->Pop();
    std::unique_ptr<Object> left = stack->Pop();
    char leftValue = left->GetObject()->ToChar();
    char rightValue = right->GetObject()->ToChar();
    stack->Push(new BooleanValue(leftValue != rightValue));
    return Next();
}

EqualStringInstruction::EqualStringInstruction() : Instruction(InstructionKind::equal_string)
{
}

Instruction* EqualStringInstruction::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> right = stack->Pop();
    Object* rightObj = right->GetObject();
    std::unique_ptr<Object> left = stack->Pop();
    Object* leftObj = left->GetObject();
    StringObject* leftObject = leftObj->ToStringObject(context);
    std::string leftValue = leftObject->Value();
    StringObject* rightObject = rightObj->ToStringObject(context);
    std::string rightValue = rightObject->Value();
    stack->Push(new BooleanValue(leftValue == rightValue));
    return Next();
}

NotEqualStringInstruction::NotEqualStringInstruction() : Instruction(InstructionKind::not_equal_string)
{
}

Instruction* NotEqualStringInstruction::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> right = stack->Pop();
    std::unique_ptr<Object> left = stack->Pop();
    Object* rightObj = right->GetObject();
    Object* leftObj = left->GetObject();
    StringObject* leftObject = leftObj->ToStringObject(context);
    std::string leftValue = leftObject->Value();
    StringObject* rightObject = rightObj->ToStringObject(context);
    std::string rightValue = rightObject->Value();
    stack->Push(new BooleanValue(leftValue != rightValue));
    return Next();
}

LessStringInstruction::LessStringInstruction() : Instruction(InstructionKind::less_string)
{
}

Instruction* LessStringInstruction::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> right = stack->Pop();
    std::unique_ptr<Object> left = stack->Pop();
    Object* rightObj = right->GetObject();
    Object* leftObj = left->GetObject();
    StringObject* leftObject = leftObj->ToStringObject(context);
    std::string leftValue = leftObject->Value();
    StringObject* rightObject = rightObj->ToStringObject(context);
    std::string rightValue = rightObject->Value();
    stack->Push(new BooleanValue(leftValue < rightValue));
    return Next();
}

GreaterStringInstruction::GreaterStringInstruction() : Instruction(InstructionKind::greater_string)
{
}

Instruction* GreaterStringInstruction::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> right = stack->Pop();
    std::unique_ptr<Object> left = stack->Pop();
    Object* rightObj = right->GetObject();
    Object* leftObj = left->GetObject();
    StringObject* leftObject = leftObj->ToStringObject(context);
    std::string leftValue = leftObject->Value();
    StringObject* rightObject = rightObj->ToStringObject(context);
    std::string rightValue = rightObject->Value();
    stack->Push(new BooleanValue(leftValue > rightValue));
    return Next();
}

LessOrEqualStringInstruction::LessOrEqualStringInstruction() : Instruction(InstructionKind::less_or_equal_string)
{
}

Instruction* LessOrEqualStringInstruction::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> right = stack->Pop();
    std::unique_ptr<Object> left = stack->Pop();
    Object* rightObj = right->GetObject();
    Object* leftObj = left->GetObject();
    StringObject* leftObject = leftObj->ToStringObject(context);
    std::string leftValue = leftObject->Value();
    StringObject* rightObject = rightObj->ToStringObject(context);
    std::string rightValue = rightObject->Value();
    stack->Push(new BooleanValue(leftValue <= rightValue));
    return Next();
}

GreaterOrEqualStringInstruction::GreaterOrEqualStringInstruction() : Instruction(InstructionKind::greater_or_equal_string)
{
}

Instruction* GreaterOrEqualStringInstruction::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> right = stack->Pop();
    std::unique_ptr<Object> left = stack->Pop();
    Object* rightObj = right->GetObject();
    Object* leftObj = left->GetObject();
    StringObject* leftObject = leftObj->ToStringObject(context);
    std::string leftValue = leftObject->Value();
    StringObject* rightObject = rightObj->ToStringObject(context);
    std::string rightValue = rightObject->Value();
    stack->Push(new BooleanValue(leftValue >= rightValue));
    return Next();
}

PlusStringInstruction::PlusStringInstruction() : Instruction(InstructionKind::plus_string)
{
}

Instruction* PlusStringInstruction::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> right = stack->Pop();
    Object* rightObj = right->GetObject();
    StringObject* rightObject = rightObj->ToStringObject(context);
    std::unique_ptr<Object> left = stack->Pop();
    Object* leftObj = left->GetObject();
    StringObject* leftObject = leftObj->ToStringObject(context);
    std::string value = leftObject->Value();
    value.append(rightObject->Value());
    Heap* heap = context->GetHeap();
    StringObject* result = heap->AllocateString(value, context);
    stack->Push(new StringObjectPtr(result));
    return Next();
}

IntToRealInstruction::IntToRealInstruction() : Instruction(InstructionKind::int_to_real)
{
}

Instruction* IntToRealInstruction::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> operand = stack->Pop();
    float value = operand->GetObject()->ToReal();
    stack->Push(new RealValue(value));
    return Next();
}

RealToIntInstruction::RealToIntInstruction() : Instruction(InstructionKind::real_to_int)
{
}

Instruction* RealToIntInstruction::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> operand = stack->Pop();
    int32_t value = operand->GetObject()->ToInteger();
    stack->Push(new IntegerValue(value));
    return Next();
}

CharToStringInstruction::CharToStringInstruction() : Instruction(InstructionKind::char_to_string)
{
}

Instruction* CharToStringInstruction::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> operand = stack->Pop();
    std::string value = operand->GetObject()->ToString();
    Heap* heap = context->GetHeap();
    StringObject* stringObject = heap->AllocateString(value, context);
    stack->Push(new StringObjectPtr(stringObject));
    return Next();
}

IdentityInstruction::IdentityInstruction() : Instruction(InstructionKind::identity)
{
}

Instruction* IdentityInstruction::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> operand = stack->Pop();
    stack->Push(operand->Clone());
    return Next();
}

EqualNilInstruction::EqualNilInstruction() : Instruction(InstructionKind::equal_nil)
{
}

Instruction* EqualNilInstruction::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> right = stack->Pop();
    std::unique_ptr<Object> left = stack->Pop();
    if (left->GetObject()->IsNilObject())
    {
        stack->Push(new BooleanValue(true));
    }
    else
    {
        stack->Push(new BooleanValue(false));
    }
    return Next();
}

Instruction* MakeInstruction(InstructionKind kind)
{
    switch (kind)
    {
        case InstructionKind::nop: return new NoOperationInstruction();
        case InstructionKind::load_local: return new LoadLocalInstruction();
        case InstructionKind::store_local: return new StoreLocalInstruction();
        case InstructionKind::load_parent: return new LoadParentInstruction();
        case InstructionKind::store_parent: return new StoreParentInstruction();
        case InstructionKind::load_global: return new LoadGlobalInstruction();
        case InstructionKind::store_global: return new StoreGlobalInstruction();
        case InstructionKind::load_constant: return new LoadConstantInstruction();
        case InstructionKind::load_result_var: return new LoadResultVarInstruction();
        case InstructionKind::push_value: return new PushValueInstruction();
        case InstructionKind::pop_value: return new PopValueInstruction();
        case InstructionKind::load_field: return new LoadFieldInstruction();
        case InstructionKind::store_field: return new StoreFieldInstruction();
        case InstructionKind::load_element: return new LoadElementInstruction();
        case InstructionKind::store_element: return new StoreElementInstruction();
        case InstructionKind::array_length: return new ArrayLengthInstruction();
        case InstructionKind::string_length: return new StringLengthInstruction();
        case InstructionKind::receive: return new ReceiveInstruction();
        case InstructionKind::jump: return new JumpInstruction();
        case InstructionKind::branch: return new BranchInstruction();
        case InstructionKind::case_inst: return new CaseInstruction();
        case InstructionKind::call_subroutine: return new CallSubroutineInstruction();
        case InstructionKind::call_virtual: return new CallVirtualInstruction();
        case InstructionKind::call_external: return new CallExternalInstruction();
        case InstructionKind::new_object: return new NewObjectInstruction();
        case InstructionKind::new_array: return new NewArrayInstruction();
        case InstructionKind::call_ctor: return new CallConstructorInstruction();
        case InstructionKind::equal_bool: return new EqualBoolInstruction();
        case InstructionKind::not_equal_bool: return new NotEqualBoolInstruction();
        case InstructionKind::and_bool: return new AndBoolInstruction();
        case InstructionKind::or_bool: return new OrBoolInstruction();
        case InstructionKind::xor_bool: return new XorBoolInstruction();
        case InstructionKind::not_bool: return new NotBoolInstruction();
        case InstructionKind::equal_int: return new EqualIntInstruction();
        case InstructionKind::not_equal_int: return new NotEqualIntInstruction();
        case InstructionKind::less_int: return new LessIntInstruction();
        case InstructionKind::greater_int: return new GreaterIntInstruction();
        case InstructionKind::less_or_equal_int: return new LessOrEqualIntInstruction();
        case InstructionKind::greater_or_equal_int: return new GreaterOrEqualIntInstruction();
        case InstructionKind::plus_int: return new PlusIntInstruction();
        case InstructionKind::minus_int: return new MinusIntInstruction();
        case InstructionKind::mul_int: return new MultiplyIntInstruction();
        case InstructionKind::fractional_divide_int: return new FractionalDivideIntInstruction();
        case InstructionKind::div_int: return new DivIntInstruction();
        case InstructionKind::mod_int: return new ModIntInstruction();
        case InstructionKind::and_int: return new AndIntInstruction();
        case InstructionKind::or_int: return new OrIntInstruction();
        case InstructionKind::xor_int: return new XorIntInstruction();
        case InstructionKind::shl_int: return new ShlIntInstruction();
        case InstructionKind::shr_int: return new ShrIntInstruction();
        case InstructionKind::not_int: return new NotIntInstruction();
        case InstructionKind::unary_plus_int: return new UnaryPlusIntInstruction();
        case InstructionKind::unary_minus_int: return new UnaryMinusIntInstruction();
        case InstructionKind::equal_real: return new EqualRealInstruction();
        case InstructionKind::not_equal_real: return new NotEqualRealInstruction();
        case InstructionKind::less_real: return new LessRealInstruction();
        case InstructionKind::greater_real: return new GreaterRealInstruction();
        case InstructionKind::less_or_equal_real: return new LessOrEqualRealInstruction();
        case InstructionKind::greater_or_equal_real: return new GreaterOrEqualRealInstruction();
        case InstructionKind::plus_real: return new PlusRealInstruction();
        case InstructionKind::minus_real: return new MinusRealInstruction();
        case InstructionKind::mul_real: return new MultiplyRealInstruction();
        case InstructionKind::fractional_divide_real: return new FractionalDivideRealInstruction();
        case InstructionKind::unary_plus_real: return new UnaryPlusRealInstruction();
        case InstructionKind::unary_minus_real: return new UnaryMinusRealInstruction();
        case InstructionKind::equal_char: return new EqualCharInstruction();
        case InstructionKind::not_equal_char: return new NotEqualCharInstruction();
        case InstructionKind::equal_string: return new EqualStringInstruction();
        case InstructionKind::not_equal_string: return new NotEqualStringInstruction();
        case InstructionKind::less_string: return new LessStringInstruction();
        case InstructionKind::greater_string: return new GreaterStringInstruction();
        case InstructionKind::less_or_equal_string: return new LessOrEqualStringInstruction();
        case InstructionKind::greater_or_equal_string: return new GreaterOrEqualStringInstruction();
        case InstructionKind::plus_string: return new PlusStringInstruction();
        case InstructionKind::int_to_real: return new IntToRealInstruction();
        case InstructionKind::real_to_int: return new RealToIntInstruction();
        case InstructionKind::char_to_string: return new CharToStringInstruction();
        case InstructionKind::identity: return new IdentityInstruction();
        case InstructionKind::equal_nil: return new EqualNilInstruction();
        case InstructionKind::equal_object: return new EqualObjectInstruction();
        case InstructionKind::not_equal_object: return new NotEqualObjectInstruction();
    }
    throw std::runtime_error("invalid instruction kind " + std::to_string(static_cast<int32_t>(kind)));
}

EqualObjectInstruction::EqualObjectInstruction() : Instruction(InstructionKind::equal_object)
{
}

Instruction* EqualObjectInstruction::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> right = stack->Pop();
    std::unique_ptr<Object> left = stack->Pop();
    Object* leftObj = left->GetObject();
    Object* RightObj = right->GetObject();
    stack->Push(new BooleanValue(leftObj == RightObj));
    return Next();
}

NotEqualObjectInstruction::NotEqualObjectInstruction() : Instruction(InstructionKind::not_equal_object)
{
}

Instruction* NotEqualObjectInstruction::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> right = stack->Pop();
    std::unique_ptr<Object> left = stack->Pop();
    Object* leftObj = left->GetObject();
    Object* RightObj = right->GetObject();
    stack->Push(new BooleanValue(leftObj != RightObj));
    return Next();
}

} // namespace p
