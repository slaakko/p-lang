// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.value;

import p.writer;
import p.reader;
import p.heap;
import p.execute;

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


Value::Value(ValueKind kind_) : Object(ObjectKind::valueObject), kind(kind_)
{
}

void Value::Write(Writer& writer)
{
}

void Value::Read(Reader& reader)
{
}

Value* Value::Succ() const
{
    throw std::runtime_error("Succ: ordinal value expected");
}

Value* Value::Pred() const
{
    throw std::runtime_error("Pred: ordinal value expected");
}

PtrValue::PtrValue() : Value(ValueKind::ptrValue), value(nullptr)
{
}

PtrValue::PtrValue(Value* value_) : Value(ValueKind::ptrValue), value(value_)
{
}

Object* PtrValue::Clone() const
{
    return new PtrValue(value);
}

BooleanValue::BooleanValue() : Value(ValueKind::booleanValue), value(false)
{
}

BooleanValue::BooleanValue(bool value_) : Value(ValueKind::booleanValue), value(value_)
{
}

void BooleanValue::Write(Writer& writer)
{
    writer.GetBinaryWriter().Write(value);
}

void BooleanValue::Read(Reader& reader)
{
    value = reader.GetBinaryReader().ReadBool();
}

Object* BooleanValue::Clone() const
{
    return new BooleanValue(value);
}

Value* BooleanValue::Succ() const
{
    return new BooleanValue(true);
}

Value* BooleanValue::Pred() const
{
    return new BooleanValue(false);
}

IntegerValue::IntegerValue() : Value(ValueKind::integerValue), value(0)
{
}

IntegerValue::IntegerValue(int32_t value_) : Value(ValueKind::integerValue), value(value_)
{
}

void IntegerValue::Write(Writer& writer)
{
    writer.GetBinaryWriter().Write(value);
}

void IntegerValue::Read(Reader& reader)
{
    value = reader.GetBinaryReader().ReadInt();
}

Object* IntegerValue::Clone() const
{
    return new IntegerValue(value);
}

Value* IntegerValue::Succ() const
{
    return new IntegerValue(value + 1);
}

Value* IntegerValue::Pred() const
{
    return new IntegerValue(value - 1);
}

CharValue::CharValue() : Value(ValueKind::charValue), value('\0')
{
}

CharValue::CharValue(unsigned char value_) : Value(ValueKind::charValue), value(value_)
{
}

void CharValue::Write(Writer& writer)
{
    writer.GetBinaryWriter().Write(value);
}

void CharValue::Read(Reader& reader)
{
    value = reader.GetBinaryReader().ReadChar();
}

Object* CharValue::Clone() const
{
    return new CharValue(value);
}

Value* CharValue::Succ() const
{
    if (value < 255)
    {
        return new CharValue(value + 1);
    }
    else
    {
        return new CharValue(255);
    }
}

Value* CharValue::Pred() const
{
    if (value > 0)
    {
        return new CharValue(value - 1);
    }
    else
    {
        return new CharValue(0);
    }
}

EnumerationValue::EnumerationValue() : Value(ValueKind::enumerationValue), name(), value()
{
}

EnumerationValue::EnumerationValue(const std::string& name_, int32_t value_) : Value(ValueKind::enumerationValue), name(name_), value(value_)
{
}

void EnumerationValue::Write(Writer& writer)
{
    writer.GetBinaryWriter().Write(name);
    writer.GetBinaryWriter().Write(value);
}

void EnumerationValue::Read(Reader& reader)
{
    name = reader.GetBinaryReader().ReadUtf8String();
    value = reader.GetBinaryReader().ReadInt();
}

Object* EnumerationValue::Clone() const
{
    return new EnumerationValue(name, value);
}

Value* EnumerationValue::Succ() const
{
    return new EnumerationValue(std::string(), value + 1);
}

Value* EnumerationValue::Pred() const
{
    return new EnumerationValue(std::string(), value - 1);
}

RealValue::RealValue() : Value(ValueKind::realValue), value(0.0f)
{
}

RealValue::RealValue(float value_) : Value(ValueKind::realValue), value(value_)
{
}

void RealValue::Write(Writer& writer)
{
    writer.GetBinaryWriter().Write(value);
}

void RealValue::Read(Reader& reader)
{
    value = reader.GetBinaryReader().ReadFloat();
}

Object* RealValue::Clone() const
{
    return new RealValue(value);
}

StringValue::StringValue() : Value(ValueKind::stringValue), value()
{
}

StringValue::StringValue(const std::string& value_) : Value(ValueKind::stringValue), value(value_)
{
}

void StringValue::Write(Writer& writer)
{
    writer.GetBinaryWriter().Write(value);
}

void StringValue::Read(Reader& reader)
{
    value = reader.GetBinaryReader().ReadUtf8String();
}

Object* StringValue::Clone() const
{
    return new StringValue(value);
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
    return new ConstantValue(value);
}

ArrayValue::ArrayValue() : Value(ValueKind::arrayValue)
{
}

void ArrayValue::AddElement(Value* element)
{
    elements.push_back(std::unique_ptr<Value>(element));
}

void ArrayValue::Write(Writer& writer)
{
    writer.GetBinaryWriter().Write(static_cast<int32_t>(elements.size()));
    for (const auto& element : elements)
    {
        writer.GetBinaryWriter().Write(static_cast<uint8_t>(element->Kind()));
        element->Write(writer);
    }
}

void ArrayValue::Read(Reader& reader)
{
    int32_t elementCount = reader.GetBinaryReader().ReadInt();
    for (int32_t i = 0; i < elementCount; ++i)
    {
        ValueKind elementKind = static_cast<ValueKind>(reader.GetBinaryReader().ReadByte());
        Value* value = MakeValue(elementKind);
        value->Read(reader);
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

void Field::Write(Writer& writer)
{
    writer.GetBinaryWriter().Write(index);
    writer.GetBinaryWriter().Write(name);
    writer.GetBinaryWriter().Write(static_cast<uint8_t>(value->Kind()));
    value->Write(writer);
}

void Field::Read(Reader& reader)
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
        throw std::runtime_error("invalid field index");
    }
}

void ObjectValue::Write(Writer& writer)
{
    writer.GetBinaryWriter().Write(static_cast<int32_t>(fields.size()));
    for (auto& field : fields)
    {
        field.Write(writer);
    }
}

void ObjectValue::Read(Reader& reader)
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

FunctionValue::FunctionValue(Function* function_) : Value(ValueKind::functionValue), function(function_)
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
    return new NilValue();
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
