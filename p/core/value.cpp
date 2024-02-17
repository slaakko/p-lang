// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.core.value;

import p.core.writer;
import p.core.reader;
import p.core.heap;
import p.core.execution_context;
import p.core.symbol_table;
import p.core.type_symbol;

namespace p {

ValueKind CommonType(ValueKind left, ValueKind right)
{
    if (left == ValueKind::booleanValue && right == ValueKind::booleanValue)
    {
        return ValueKind::booleanValue;
    }
    else if (left == ValueKind::integerValue)
    {
        if (right == ValueKind::integerValue)
        {
            return ValueKind::integerValue;
        }
        else if (right == ValueKind::realValue)
        {
            return ValueKind::realValue;
        }
        else
        {
            return ValueKind::none;
        }
    }
    else if (left == ValueKind::realValue)
    {
        if (right == ValueKind::realValue)
        {
            return ValueKind::realValue;
        }
        else if (right == ValueKind::integerValue)
        {
            return ValueKind::realValue;
        }
        else
        {
            return ValueKind::none;
        }
    }
    else if (left == ValueKind::charValue)
    {
        if (right == ValueKind::charValue)
        {
            return ValueKind::charValue;
        }
        else if (right == ValueKind::stringValue)
        {
            return ValueKind::stringValue;
        }
        else
        {
            return ValueKind::none;
        }
    }
    else if (left == ValueKind::stringValue)
    {
        if (right == ValueKind::stringValue)
        {
            return ValueKind::stringValue;
        }
        else if (right == ValueKind::charValue)
        {
            return ValueKind::stringValue;
        }
        else
        {
            return ValueKind::none;
        }
    }
    else
    {
        return ValueKind::none;
    }
}

Value::Value(ValueKind kind_) : Object(ObjectKind::valueObject), kind(kind_), type(nullptr)
{
}

void Value::Write(SymbolWriter& writer)
{
}

void Value::Read(SymbolReader& reader)
{
}

Value* Value::Succ() const
{
    throw std::runtime_error("error: value.succ: ordinal value expected");
}

Value* Value::Pred() const
{
    throw std::runtime_error("error: value.pred: ordinal value expected");
}

Value* Value::Succ(Node* node) const
{
    ThrowError("error: value.succ: ordinal value expected", node->FilePath(), node->Span());
    return nullptr;
}

Value* Value::Pred(Node* node) const
{
    ThrowError("error: value.pred: ordinal value expected", node->FilePath(), node->Span());
    return nullptr;
}

PtrValue::PtrValue() : Value(ValueKind::ptrValue), value(nullptr)
{
}

PtrValue::PtrValue(Value* value_) : Value(ValueKind::ptrValue), value(value_)
{
}

Object* PtrValue::Clone() const
{
    PtrValue* clone = new PtrValue(value);
    clone->SetType(Type());
    return clone;
}

BooleanValue::BooleanValue() : Value(ValueKind::booleanValue), value(false)
{
}

BooleanValue::BooleanValue(bool value_) : Value(ValueKind::booleanValue), value(value_)
{
}

void BooleanValue::Write(SymbolWriter& writer)
{
    writer.GetBinaryWriter().Write(value);
}

void BooleanValue::Read(SymbolReader& reader)
{
    value = reader.GetBinaryReader().ReadBool();
}

Object* BooleanValue::Clone() const
{
    BooleanValue* clone = new BooleanValue(value);
    clone->SetType(Type());
    return clone;
}

Value* BooleanValue::Succ() const
{
    BooleanValue* booleanValue = new BooleanValue(true);
    booleanValue->SetType(Type());
    return booleanValue;
}

Value* BooleanValue::Pred() const
{
    BooleanValue* booleanValue = new BooleanValue(false);
    booleanValue->SetType(Type());
    return booleanValue;
}

Value* BooleanValue::Succ(Node* node) const
{
    BooleanValue* booleanValue = new BooleanValue(true);
    booleanValue->SetType(Type());
    return booleanValue;
}

Value* BooleanValue::Pred(Node* node) const
{
    BooleanValue* booleanValue = new BooleanValue(false);
    booleanValue->SetType(Type());
    return booleanValue;
}

IntegerValue::IntegerValue() : Value(ValueKind::integerValue), value(0)
{
}

IntegerValue::IntegerValue(int32_t value_) : Value(ValueKind::integerValue), value(value_)
{
}

void IntegerValue::Write(SymbolWriter& writer)
{
    writer.GetBinaryWriter().Write(value);
}

void IntegerValue::Read(SymbolReader& reader)
{
    value = reader.GetBinaryReader().ReadInt();
}

Object* IntegerValue::Clone() const
{
    IntegerValue* clone = new IntegerValue(value);
    clone->SetType(Type());
    return clone;
}

Value* IntegerValue::Succ() const
{
    IntegerValue* integerValue = new IntegerValue(value + 1);
    integerValue->SetType(Type());
    return integerValue;
}

Value* IntegerValue::Pred() const
{
    IntegerValue* integerValue = new IntegerValue(value - 1);
    integerValue->SetType(Type());
    return integerValue;
}

Value* IntegerValue::Succ(Node* node) const
{
    IntegerValue* integerValue = new IntegerValue(value + 1);
    integerValue->SetType(Type());
    return integerValue;
}

Value* IntegerValue::Pred(Node* node) const
{
    IntegerValue* integerValue = new IntegerValue(value - 1);
    integerValue->SetType(Type());
    return integerValue;
}

CharValue::CharValue() : Value(ValueKind::charValue), value('\0')
{
}

CharValue::CharValue(unsigned char value_) : Value(ValueKind::charValue), value(value_)
{
}

void CharValue::Write(SymbolWriter& writer)
{
    writer.GetBinaryWriter().Write(value);
}

void CharValue::Read(SymbolReader& reader)
{
    value = reader.GetBinaryReader().ReadChar();
}

Object* CharValue::Clone() const
{
    CharValue* clone = new CharValue(value);
    clone->SetType(Type());
    return clone;
}

Value* CharValue::Succ() const
{
    if (value < 255)
    {
        CharValue* charValue = new CharValue(value + 1);
        charValue->SetType(Type());
        return charValue;
    }
    else
    {
        CharValue* charValue = new CharValue(255);
        charValue->SetType(Type());
        return charValue;
    }
}

Value* CharValue::Pred() const
{
    if (value > 0)
    {
        CharValue* charValue = new CharValue(value - 1);
        charValue->SetType(Type());
        return charValue;
    }
    else
    {
        CharValue* charValue = new CharValue(0);
        charValue->SetType(Type());
        return charValue;
    }
}

Value* CharValue::Succ(Node* node) const
{
    if (value < 255)
    {
        CharValue* charValue = new CharValue(value + 1);
        charValue->SetType(Type());
        return charValue;
    }
    else
    {
        CharValue* charValue = new CharValue(255);
        charValue->SetType(Type());
        return charValue;
    }
}

Value* CharValue::Pred(Node* node) const
{
    if (value > 0)
    {
        CharValue* charValue = new CharValue(value - 1);
        charValue->SetType(Type());
        return charValue;
    }
    else
    {
        CharValue* charValue = new CharValue(0);
        charValue->SetType(Type());
        return charValue;
    }
}

EnumerationValue::EnumerationValue() : Value(ValueKind::enumerationValue), value()
{
}

EnumerationValue::EnumerationValue(int32_t value_) : Value(ValueKind::enumerationValue), value(value_)
{
}

void EnumerationValue::Write(SymbolWriter& writer)
{
    writer.GetBinaryWriter().Write(value);
}

void EnumerationValue::Read(SymbolReader& reader)
{
    value = reader.GetBinaryReader().ReadInt();
}

Object* EnumerationValue::Clone() const
{
    return new EnumerationValue(value);
}

Value* EnumerationValue::Succ() const
{
    return new EnumerationValue(value + 1);
}

Value* EnumerationValue::Pred() const
{
    return new EnumerationValue(value - 1);
}

Value* EnumerationValue::Succ(Node* node) const
{
    return new EnumerationValue(value + 1);
}

Value* EnumerationValue::Pred(Node* node) const
{
    return new EnumerationValue(value - 1);
}

RealValue::RealValue() : Value(ValueKind::realValue), value(0.0f)
{
}

RealValue::RealValue(float value_) : Value(ValueKind::realValue), value(value_)
{
}

void RealValue::Write(SymbolWriter& writer)
{
    writer.GetBinaryWriter().Write(value);
}

void RealValue::Read(SymbolReader& reader)
{
    value = reader.GetBinaryReader().ReadFloat();
}

Object* RealValue::Clone() const
{
    RealValue* clone = new RealValue(value);
    clone->SetType(Type());
    return clone;
}

StringValue::StringValue() : Value(ValueKind::stringValue), value()
{
}

StringValue::StringValue(const std::string& value_) : Value(ValueKind::stringValue), value(value_)
{
}

void StringValue::Write(SymbolWriter& writer)
{
    writer.GetBinaryWriter().Write(value);
}

void StringValue::Read(SymbolReader& reader)
{
    value = reader.GetBinaryReader().ReadUtf8String();
}

Object* StringValue::Clone() const
{
    StringValue* clone = new StringValue(value);
    clone->SetType(Type());
    return clone;
}

StringObject* StringValue::ToStringObject(ExecutionContext* context)
{
    Heap* heap = context->GetHeap();
    return heap->AllocateString(value, context);
}

ConstantValue::ConstantValue(Value* value_) : Value(ValueKind::constantValue), value(value_)
{
}

Object* ConstantValue::Clone() const
{
    ConstantValue* clone = new ConstantValue(value);
    clone->SetType(Type());
    return clone;
}

ArrayValue::ArrayValue() : Value(ValueKind::arrayValue)
{
}

void ArrayValue::AddElement(Value* element)
{
    elements.push_back(std::unique_ptr<Value>(element));
}

Value* ArrayValue::GetElement(int32_t elementIndex) const
{
    if (elementIndex >= 0 && elementIndex < elements.size())
    {
        return elements[elementIndex].get();
    }
    else
    {
        throw std::runtime_error("error: array value: invalid array index");
    }
}

Value* ArrayValue::GetElement(int32_t elementIndex, Node* node) const
{
    if (elementIndex >= 0 && elementIndex < elements.size())
    {
        return elements[elementIndex].get();
    }
    else
    {
        ThrowError("error: array value: invalid array index", node->FilePath(), node->Span());
    }
    return nullptr;
}

void ArrayValue::Write(SymbolWriter& writer)
{
    writer.GetBinaryWriter().Write(static_cast<int32_t>(elements.size()));
    for (const auto& element : elements)
    {
        writer.GetBinaryWriter().Write(static_cast<uint8_t>(element->Kind()));
        element->Write(writer);
    }
}

void ArrayValue::Read(SymbolReader& reader)
{
    int32_t elementCount = reader.GetBinaryReader().ReadInt();
    for (int32_t i = 0; i < elementCount; ++i)
    {
        ValueKind elementKind = static_cast<ValueKind>(reader.GetBinaryReader().ReadByte());
        Value* value = MakeValue(elementKind);
        value->Read(reader);
        AddElement(value);
    }
}

Object* ArrayValue::Clone() const
{
    ArrayValue* clone = new ArrayValue();
    for (const auto& element : elements)
    {
        clone->AddElement(static_cast<Value*>(element->Clone()));
    }
    return clone;
}

Field::Field() : index(), name(), value()
{
}

Field::Field(int32_t index_, const std::string& name_, Value* value_) : index(index_), name(name_), value(value_)
{
}

void Field::Write(SymbolWriter& writer)
{
    writer.GetBinaryWriter().Write(index);
    writer.GetBinaryWriter().Write(name);
    writer.GetBinaryWriter().Write(static_cast<uint8_t>(value->Kind()));
    value->Write(writer);
}

void Field::Read(SymbolReader& reader)
{
    index = reader.GetBinaryReader().ReadInt();
    name = reader.GetBinaryReader().ReadUtf8String();
    ValueKind kind = static_cast<ValueKind>(reader.GetBinaryReader().ReadByte());
    value.reset(MakeValue(kind));
    value->Read(reader);
}

ObjectValue::ObjectValue() : Value(ValueKind::objectValue)
{
}

void ObjectValue::AddField(Field&& field)
{
    fields.push_back(std::move(field));
}

const Field& ObjectValue::GetField(int32_t index) const
{
    if (index >= 0 && index < fields.size())
    {
        return fields[index];
    }
    else
    {
        throw std::runtime_error("error: object value get field: invalid field index " + std::to_string(index));
    }
}

const Field& ObjectValue::GetField(int32_t index, Node* node) const
{
    if (index >= 0 && index < fields.size())
    {
        return fields[index];
    }
    else
    {
        ThrowError("error: object value get field: invalid field index " + std::to_string(index), node->FilePath(), node->Span());
    }
    static Field field;
    return field;
}

const Field& ObjectValue::GetField(const std::string& fieldName, Node* node) const
{
    for (int32_t index = 0; index < fields.size(); ++index)
    {
        const Field& field = fields[index];
        if (field.Name() == fieldName)
        {
            return field;
        }
    }
    ThrowError("error: object value get field: invalid field name '" + fieldName + "'", node->FilePath(), node->Span());
    static Field field;
    return field;
}

void ObjectValue::Write(SymbolWriter& writer)
{
    writer.GetBinaryWriter().Write(static_cast<int32_t>(fields.size()));
    for (auto& field : fields)
    {
        field.Write(writer);
    }
}

void ObjectValue::Read(SymbolReader& reader)
{
    int32_t fieldCount = reader.GetBinaryReader().ReadInt();
    for (int32_t i = 0; i < fieldCount; ++i)
    {
        Field field;
        field.Read(reader);
        AddField(std::move(field));
    }
}

Object* ObjectValue::Clone() const
{
    ObjectValue* clone = new ObjectValue();
    for (const auto& field : fields)
    {
        clone->AddField(Field(field.Index(), field.Name(), static_cast<Value*>(field.GetValue()->Clone())));
    }
    return clone;
}

FunctionValue::FunctionValue(FunctionSymbol* function_) : Value(ValueKind::functionValue), function(function_)
{
}

FunctionValue::FunctionValue() : Value(ValueKind::functionValue), function(nullptr)
{
}

Object* FunctionValue::Clone() const
{
    return new FunctionValue(function);
}

GenericPointerValue::GenericPointerValue() : Value(ValueKind::genericPointerValue), pointer(nullptr)
{
}

GenericPointerValue::GenericPointerValue(void* pointer_) : Value(ValueKind::genericPointerValue), pointer(pointer_)
{
}

Object* GenericPointerValue::Clone() const
{
    return new GenericPointerValue(pointer);
}

NilValue::NilValue() : Value(ValueKind::nilValue)
{
}

Object* NilValue::Clone() const
{
    NilValue* clone = new NilValue();
    clone->SetType(Type());
    return clone;
}

Value* MakeValue(ValueKind kind)
{
    Value* value = nullptr;
    switch (kind)
    {
        case ValueKind::booleanValue:
        {
            value = new BooleanValue();
            break;
        }
        case ValueKind::integerValue:
        {
            value = new IntegerValue();
            break;
        }
        case ValueKind::charValue:
        {
            value = new CharValue();
            break;
        }
        case ValueKind::enumerationValue:
        {
            value = new EnumerationValue();
            break;
        }
        case ValueKind::realValue:
        {
            value = new RealValue();
            break;
        }
        case ValueKind::stringValue:
        {
            value = new StringValue();
            break;
        }
        case ValueKind::arrayValue:
        {
            value = new ArrayValue();
            break;
        }
        case ValueKind::objectValue:
        {
            value = new ObjectValue();
            break;
        }
        case ValueKind::nilValue:
        {
            value = new NilValue();
            break;
        }
    }
    return value;
}

} // namespace p
