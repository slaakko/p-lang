// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.object;

import p.type;
import p.value;
import p.execute;
import p.heap;

namespace p {

bool HasNullContent(uint8_t* addr)
{
    for (int i = 0; i < 32; ++i)
    {
        if (addr[i] != 0) return false;
    }
    return true;
}

Object::Object(ObjectKind kind_) : kind(kind_), owner(nullptr), ownerIndex(-1)
{
}

Object::~Object()
{
}

bool Object::ToBoolean() const
{
    throw std::runtime_error("cannot convert '" + KindStr() + "' to Boolean");
}

int32_t Object::ToInteger() const
{
    throw std::runtime_error("cannot convert '" + KindStr() + "' to integer");
}

char Object::ToChar() const
{
    throw std::runtime_error("cannot convert '" + KindStr() + "' to char");
}

float Object::ToReal() const
{
    throw std::runtime_error("cannot convert '" + KindStr() + "' to real");
}

std::string Object::ToString() const
{
    throw std::runtime_error("cannot convert '" + KindStr() + "' to string");
}

StringObject* Object::ToStringObject(ExecutionContext* context) 
{
    throw std::runtime_error("cannot convert '" + KindStr() + "' to string object");
}

PtrObject::PtrObject(Object* object_) : Object(ObjectKind::ptrObject), object(object_)
{
}

Object* PtrObject::Clone() const
{
    return new PtrObject(object);
}

Object* PtrObject::GetObject() 
{ 
    if (object)
    {
        return object->GetObject();
    }
    else
    {
        return nullptr;
    }
}

void PtrObject::SetObject(Object* object_) 
{
    object = object_; 
}

HeapObject::HeapObject(ObjectType* type_, int32_t fieldCount_, int32_t size_) : 
    Object(ObjectKind::heapObject), type(type_), fieldCount(fieldCount_), size(size_)
{
}

Object* HeapObject::Clone() const
{
    return new HeapObjectPtr(const_cast<HeapObject*>(this));
}

void HeapObject::Init(ExecutionContext* context)
{
    fieldCount = type->FieldCount();
    for (int32_t i = 0; i < fieldCount; ++i)
    {
        Type* fieldType = type->GetFieldType(i);
        if (!fieldType)
        {
            continue;
        }
        switch (fieldType->Kind())
        {
            case TypeKind::booleanType:
            {
                BooleanValue value(false);
                SetField(i, &value, context);
                break;
            }
            case TypeKind::integerType:
            case TypeKind::enumeratedType:
            case TypeKind::subrangeType:
            {
                IntegerValue value(0);
                SetField(i, &value, context);
                break;
            }
            case TypeKind::charType:
            {
                CharValue value('\0');
                SetField(i, &value, context);
                break;
            }
            case TypeKind::realType:
            {
                RealValue value(0.0f);
                SetField(i, &value, context);
                break;
            }
            case TypeKind::stringType:
            {
                SetField(i, NilObject::Ptr(), context);
                break;
            }
            case TypeKind::objectType:
            {
                SetField(i, NilObject::Ptr(), context);
                break;
            }
            case TypeKind::pointerType:
            {
                GenericPointerValue value(nullptr);
                SetField(i, &value, context);
                break;
            }
            case TypeKind::arrayType:
            {
                ArrayObjectPtr arrayPtr(nullptr);
                SetField(i, &arrayPtr, context);
                break;
            }
        }
    }
    if (type->IsVirtual())
    {
        int32_t vmtPtrFieldIndex = type->VmtPtrFieldIndex();
        Vmt* vmtPtr = type->GetVmtPtr();
        GenericPointerValue pointerValue(vmtPtr);
        SetField(vmtPtrFieldIndex, &pointerValue, context);
    }
}

Object* HeapObject::GetField(int32_t fieldIndex) const
{
    int32_t offset = sizeof(HeapObject) + fieldIndex * valueSize;
    uint8_t* addr = Ptr() + offset;
    if (HasNullContent(addr))
    {
        throw std::runtime_error("uninitialized field access");
    }
    return static_cast<Object*>(static_cast<void*>(addr));
}

void HeapObject::SetField(int32_t fieldIndex, Object* object, ExecutionContext* context)
{
    int32_t offset = sizeof(HeapObject) + fieldIndex * valueSize;
    switch (object->Kind())
    {
        case ObjectKind::valueObject:
        {
            Value* value = static_cast<Value*>(object);
            switch (value->Kind())
            {
                case ValueKind::booleanValue:
                {
                    BooleanValue* booleanValue = static_cast<BooleanValue*>(value);
                    new (static_cast<void*>(Ptr() + offset))BooleanValue(*booleanValue);
                    break;
                }
                case ValueKind::integerValue:
                {
                    IntegerValue* integerValue = static_cast<IntegerValue*>(value);
                    new (static_cast<void*>(Ptr() + offset))IntegerValue(*integerValue);
                    break;
                }
                case ValueKind::charValue:
                {
                    CharValue* charValue = static_cast<CharValue*>(value);
                    new (static_cast<void*>(Ptr() + offset))CharValue(*charValue);
                    break;
                }
                case ValueKind::realValue:
                {
                    RealValue* realValue = static_cast<RealValue*>(value);
                    new (static_cast<void*>(Ptr() + offset))RealValue(*realValue);
                    break;
                }
                case ValueKind::stringValue:
                {
                    StringValue* stringValue = static_cast<StringValue*>(value);
                    Heap* heap = context->GetHeap();
                    StringObjectPtr stringObjectPtr(heap->AllocateString(stringValue->GetStringValue(), context));
                    new (static_cast<void*>(Ptr() + offset))StringObjectPtr(stringObjectPtr);
                    break;
                }
                case ValueKind::genericPointerValue:
                {
                    GenericPointerValue* pointerValue = static_cast<GenericPointerValue*>(value);
                    new (static_cast<void*>(Ptr() + offset))GenericPointerValue(*pointerValue);
                    break;
                }
            }
            break;
        }
        case ObjectKind::heapObject:
        {
            HeapObjectPtr heapObjectPtr(static_cast<HeapObject*>(object));
            new (static_cast<void*>(Ptr() + offset))HeapObjectPtr(heapObjectPtr);
            break;
        }
        case ObjectKind::heapObjectPtr:
        {
            HeapObjectPtr* heapObjectPtr = static_cast<HeapObjectPtr*>(object);
            new (static_cast<void*>(Ptr() + offset))HeapObjectPtr(*heapObjectPtr);
            break;
        }
        case ObjectKind::stringObject:
        {
            StringObjectPtr stringObjectPtr(static_cast<StringObject*>(object));
            new (static_cast<void*>(Ptr() + offset))StringObjectPtr(stringObjectPtr);
            break;
        }
        case ObjectKind::stringObjectPtr:
        {
            StringObjectPtr* stringObjectPtr = static_cast<StringObjectPtr*>(object);
            new (static_cast<void*>(Ptr() + offset))StringObjectPtr(*stringObjectPtr);
            break;
        }
        case ObjectKind::arrayObject:
        {
            ArrayObjectPtr arrayObjectPtr(static_cast<ArrayObject*>(object));
            new (Ptr() + offset)ArrayObjectPtr(arrayObjectPtr);
            break;
        }
        case ObjectKind::arrayObjectPtr:
        {
            ArrayObjectPtr* arrayObjectPtr = static_cast<ArrayObjectPtr*>(object);
            new (Ptr() + offset)ArrayObjectPtr(*arrayObjectPtr);
            break;
        }
        case ObjectKind::nilObject:
        {
            HeapObjectPtr heapObjectPtr(static_cast<HeapObject*>(object));
            new (static_cast<void*>(Ptr() + offset))HeapObjectPtr(heapObjectPtr);
            break;
        }
    }
}

HeapObjectPtr::HeapObjectPtr(HeapObject* heapObject_) : Object(ObjectKind::heapObjectPtr), heapObject(heapObject_)
{
}

Object* HeapObjectPtr::Clone() const
{
    return new HeapObjectPtr(heapObject);
}

Object* HeapObjectPtr::GetObject() 
{ 
    if (heapObject)
    {
        return heapObject->GetObject();
    }
    else
    {
        return nullptr;
    }
}

void HeapObjectPtr::SetObject(Object* value_) 
{ 
    if (heapObject)
    {
        heapObject->SetObject(value_);
    }
    else
    {
        throw std::runtime_error("null pointer access");
    }
}

ArrayObject::ArrayObject(ArrayType* arrayType_, int32_t length_, int32_t size_) : Object(ObjectKind::arrayObject), arrayType(arrayType_), length(length_), size(size_)
{
}

Object* ArrayObject::Clone() const
{
    return new ArrayObjectPtr(const_cast<ArrayObject*>(this));
}

void ArrayObject::Init(ExecutionContext* context)
{
    for (int32_t i = 0; i < length; ++i)
    {
        Type* elementType = arrayType->ElementType();
        switch (elementType->Kind())
        {
            case TypeKind::booleanType:
            {
                BooleanValue value(false);
                SetElement(i, &value, context);
                break;
            }
            case TypeKind::integerType:
            case TypeKind::enumeratedType:
            case TypeKind::subrangeType:
            {
                IntegerValue value(0);
                SetElement(i, &value, context);
                break;
            }
            case TypeKind::charType:
            {
                CharValue value('\0');
                SetElement(i, &value, context);
                break;
            }
            case TypeKind::realType:
            {
                RealValue value(0.0f);
                SetElement(i, &value, context);
                break;
            }
            case TypeKind::stringType:
            {
                SetElement(i, NilObject::Ptr(), context);
                break;
            }
            case TypeKind::objectType:
            {
                SetElement(i, NilObject::Ptr(), context);
                break;
            }
        }
    }
}

Object* ArrayObject::GetElement(int32_t elementIndex) const
{
    int32_t offset = sizeof(ArrayObject) + elementIndex * valueSize;
    uint8_t* addr = Ptr() + offset;
    if (HasNullContent(addr))
    {
        throw std::runtime_error("uninitialized array element access");
    }
    return static_cast<Object*>(static_cast<void*>(addr));
}

void ArrayObject::SetElement(int32_t elementIndex, Object* element, ExecutionContext* context)
{
    int32_t offset = sizeof(ArrayObject) + elementIndex * valueSize;
    switch (element->Kind())
    {
        case ObjectKind::valueObject:
        {
            Value* value = static_cast<Value*>(element);
            switch (value->Kind())
            {
                case ValueKind::booleanValue:
                {
                    BooleanValue* booleanValue = static_cast<BooleanValue*>(value);
                    new (static_cast<void*>(Ptr() + offset))BooleanValue(*booleanValue);
                    break;
                }
                case ValueKind::integerValue:
                {
                    IntegerValue* integerValue = static_cast<IntegerValue*>(value);
                    new (static_cast<void*>(Ptr() + offset))IntegerValue(*integerValue);
                    break;
                }
                case ValueKind::charValue:
                {
                    CharValue* charValue = static_cast<CharValue*>(value);
                    new (static_cast<void*>(Ptr() + offset))CharValue(*charValue);
                    break;
                }
                case ValueKind::realValue:
                {
                    RealValue* realValue = static_cast<RealValue*>(value);
                    new (static_cast<void*>(Ptr() + offset))RealValue(*realValue);
                    break;
                }
                case ValueKind::stringValue:
                {
                    StringValue* stringValue = static_cast<StringValue*>(value);
                    Heap* heap = context->GetHeap();
                    StringObjectPtr stringObjectPtr(heap->AllocateString(stringValue->GetStringValue(), context));
                    new (static_cast<void*>(Ptr() + offset))StringObjectPtr(stringObjectPtr);
                    break;
                }
                case ValueKind::genericPointerValue:
                {
                    GenericPointerValue* pointerValue = static_cast<GenericPointerValue*>(value);
                    new (static_cast<void*>(Ptr() + offset))GenericPointerValue(*pointerValue);
                    break;
                }
                default:
                {
                    int x = 0;
                    break;
                }
            }
            break;
        }
        case ObjectKind::heapObject:
        {
            HeapObjectPtr heapObjectPtr(static_cast<HeapObject*>(element));
            new (static_cast<void*>(Ptr() + offset))HeapObjectPtr(heapObjectPtr);
            break;
        }
        case ObjectKind::heapObjectPtr:
        {
            HeapObjectPtr* heapObjectPtr = static_cast<HeapObjectPtr*>(element);
            new (static_cast<void*>(Ptr() + offset))HeapObjectPtr(*heapObjectPtr);
            break;
        }
        case ObjectKind::nilObject:
        {
            HeapObjectPtr heapObjectPtr(static_cast<HeapObject*>(element));
            new (static_cast<void*>(Ptr() + offset))HeapObjectPtr(heapObjectPtr);
            break;
        }
        case ObjectKind::arrayObject:
        {
            ArrayObjectPtr arrayObjectPtr(static_cast<ArrayObject*>(element));
            new (static_cast<void*>(Ptr() + offset))ArrayObjectPtr(arrayObjectPtr);
            break;
        }
        case ObjectKind::arrayObjectPtr:
        {
            ArrayObjectPtr arrayObjectPtr(static_cast<ArrayObject*>(element));
            new (static_cast<void*>(Ptr() + offset))ArrayObjectPtr(arrayObjectPtr);
            break;
        }
        case ObjectKind::stringObject:
        {
            StringObjectPtr stringObjectPtr(static_cast<StringObject*>(element));
            new (static_cast<void*>(Ptr() + offset))StringObjectPtr(stringObjectPtr);
            break;
        }
        case ObjectKind::stringObjectPtr:
        {
            StringObjectPtr* stringObjectPtr = static_cast<StringObjectPtr*>(element);
            new (static_cast<void*>(Ptr() + offset))StringObjectPtr(*stringObjectPtr);
            break;
        }
        default:
        {
            int x = 0;
            break;
        }
    }
}

ArrayObjectPtr::ArrayObjectPtr(ArrayObject* arrayObject_) : Object(ObjectKind::arrayObjectPtr), arrayObject(arrayObject_)
{
}

Object* ArrayObjectPtr::Clone() const
{
    return new ArrayObjectPtr(arrayObject);
}

Object* ArrayObjectPtr::GetObject()
{
    if (arrayObject)
    {
        return arrayObject->GetObject();
    }
    else
    {
        return nullptr;
    }
}

void ArrayObjectPtr::SetObject(Object* value_)
{
    if (arrayObject)
    {
        arrayObject->SetObject(value_);
    }
    else
    {
        throw std::runtime_error("null pointer access");
    }
}

StringObject::StringObject(const std::string& value_, int32_t size_) : Object(ObjectKind::stringObject), length(value_.length()), value(nullptr), size(size_)
{
    uint8_t* p = Ptr() + sizeof(StringObject);
    char* q = static_cast<char*>(static_cast<void*>(p));
    std::strncpy(q, value_.c_str(), value_.length());
    value = q;
}

Object* StringObject::Clone() const
{
    return new StringObjectPtr(const_cast<StringObject*>(this));
}

StringObject* StringObject::ToStringObject(ExecutionContext* context) 
{
    return const_cast<StringObject*>(this);
}

StringObjectPtr::StringObjectPtr(StringObject* stringObject_) : Object(ObjectKind::stringObjectPtr), stringObject(stringObject_)
{
}

Object* StringObjectPtr::Clone() const
{
    return new StringObjectPtr(stringObject);
}

Object* StringObjectPtr::GetObject() 
{
    if (stringObject)
    {
        return stringObject->GetObject();
    }
    else
    {
        return nullptr;
    }
}

void StringObjectPtr::SetObject(Object* value_) 
{
    if (stringObject)
    {
        stringObject->SetObject(value_);
    }
    else
    {
        throw std::runtime_error("null pointer access");
    }
}

StringObject* StringObjectPtr::ToStringObject(ExecutionContext* context) 
{
    return GetObject()->ToStringObject(context);
}

NilObject::NilObject() : Object(ObjectKind::nilObject)
{
}

Object* NilObject::Clone() const
{
    return const_cast<NilObject*>(this);
}

Object* NilObject::Ptr()
{
    static NilObject instance;
    return &instance;
}

} // namespace p
