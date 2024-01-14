// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.heap;

import p.type;
import p.execute;
import p.value;

namespace p {

HeapObject* CopyObject(HeapObject* object, uint8_t* mem, uint8_t*& current, int32_t size)
{
    int32_t allocationSize = object->Size();
    int32_t currentSize = current - mem;
    if (currentSize + allocationSize > size)
    {
        throw std::runtime_error("copy_object: out of memory");
    }
    std::memcpy(static_cast<void*>(current), object, allocationSize);
    HeapObject* newObject = static_cast<HeapObject*>(static_cast<void*>(current));
    current += allocationSize;
    newObject->SetType(object->GetType());
    newObject->SetFieldCount(object->FieldCount());
    newObject->SetSize(object->Size());
    return newObject;
}

ArrayObject* CopyArray(ArrayObject* object, uint8_t* mem, uint8_t*& current, int32_t size)
{
    int32_t allocationSize = object->Size();
    int32_t currentSize = current - mem;
    if (currentSize + allocationSize > size)
    {
        throw std::runtime_error("copy_array: out of memory");
    }
    std::memcpy(static_cast<void*>(current), object, allocationSize);
    ArrayObject* newArrayObject = static_cast<ArrayObject*>(static_cast<void*>(current));
    current += allocationSize;
    newArrayObject->SetArrayType(object->GetArrayType());
    newArrayObject->SetLength(object->Length());
    newArrayObject->SetSize(object->Size());
    return newArrayObject;
}

StringObject* CopyString(StringObject* object, uint8_t* mem, uint8_t*& current, int32_t size)
{
    int32_t allocationSize = object->Size();
    int32_t currentSize = current - mem;
    if (currentSize + allocationSize > size)
    {
        throw std::runtime_error("copy_string: out of memory");
    }
    std::memcpy(static_cast<void*>(current), object, allocationSize);
    StringObject* newObject = static_cast<StringObject*>(static_cast<void*>(current));
    current += allocationSize;
    newObject->SetLength(object->Length());
    uint8_t* p = newObject->Ptr() + sizeof(StringObject);
    char* q = static_cast<char*>(static_cast<void*>(p));
    newObject->SetValue(q);
    newObject->SetSize(object->Size());
    return newObject;
}

void CollectObject(HeapObject* object, HeapObject* parent, int32_t index,
    std::map<Object*, Object*>& objectMap, uint8_t* mem, uint8_t*& current, int32_t size, ExecutionContext* context);
    
void CollectArray(ArrayObject* arrayObject, HeapObject* parent, int32_t index, std::map<Object*, Object*>& objectMap, uint8_t* mem, uint8_t*& current, int32_t size,
    ExecutionContext* context);

void CollectString(StringObject* stringObject, HeapObject* parent, int32_t index, std::map<Object*, Object*>& objectMap, uint8_t* mem, uint8_t*& current, int32_t size,
    ExecutionContext* context);

void CollectArrayElementString(StringObject* object, ArrayObject* parent, int32_t index, std::map<Object*, Object*>& objectMap, uint8_t* mem, uint8_t*& current, int32_t size,
    ExecutionContext* context);

void CollectArrayElementObject(HeapObject* object, ArrayObject* parent, int32_t index, std::map<Object*, Object*>& objectMap, uint8_t* mem, uint8_t*& current, int32_t size,
    ExecutionContext* context)
{
    auto it = objectMap.find(object);
    if (it != objectMap.end())
    {
        Object* newObject = it->second;
        parent->SetElement(index, newObject, context);
    }
    else
    {
        HeapObject* newObject = CopyObject(object, mem, current, size);
        objectMap[object] = newObject;
        parent->SetElement(index, newObject, context);
        ObjectType* objectType = newObject->GetType();
        int32_t fieldCount = objectType->FieldCount();
        for (int32_t fieldIndex = 0; fieldIndex < fieldCount; ++fieldIndex)
        {
            Type* fieldType = objectType->GetFieldType(fieldIndex);
            if (fieldType)
            {
                switch (fieldType->Kind())
                {
                    case TypeKind::objectType:
                    {
                        Object* child = newObject->GetField(fieldIndex);
                        child = child->GetObject();
                        if (child->Kind() == ObjectKind::heapObject)
                        {
                            CollectObject(static_cast<HeapObject*>(child), newObject, fieldIndex, objectMap, mem, current, size, context);
                        }
                        else if (child->Kind() != ObjectKind::nilObject)
                        {
                            throw std::runtime_error("collect_array_element_object: invalid object kind");
                        }
                        break;
                    }
                    case TypeKind::arrayType:
                    {
                        Object* child = newObject->GetField(fieldIndex);
                        child = child->GetObject();
                        if (child->Kind() == ObjectKind::arrayObject)
                        {
                            CollectArray(static_cast<ArrayObject*>(child), newObject, fieldIndex, objectMap, mem, current, size, context);
                        }
                        else if (child->Kind() != ObjectKind::nilObject)
                        {
                            throw std::runtime_error("collect_array_element_object: invalid array object kind");
                        }
                        break;
                    }
                    case TypeKind::stringType:
                    {
                        Object* child = newObject->GetField(fieldIndex);
                        child = child->GetObject();
                        if (child->Kind() == ObjectKind::stringObject)
                        {
                            CollectString(static_cast<StringObject*>(child), newObject, fieldIndex, objectMap, mem, current, size, context);
                        }
                        else if (child->Kind() != ObjectKind::nilObject)
                        {
                            throw std::runtime_error("collect_array_element_object: invalid string object kind");
                        }
                        break;
                    }
                }
            }
        }
    }
}

void CollectArrayElementArray(ArrayObject* arrayObject, ArrayObject* parent, int32_t elementIndex, std::map<Object*, Object*>& objectMap, uint8_t* mem, uint8_t*& current, int32_t size, 
    ExecutionContext* context)
{
    auto it = objectMap.find(arrayObject);
    if (it != objectMap.end())
    {
        Object* newObject = it->second;
        parent->SetElement(elementIndex, newObject, context);
    }
    else
    {
        ArrayObject* newObject = CopyArray(arrayObject, mem, current, size);
        objectMap[arrayObject] = newObject;
        parent->SetElement(elementIndex, newObject, context);
        ArrayType* arrayType = arrayObject->GetArrayType();
        Type* elementType = arrayType->ElementType();
        if (elementType->IsObjectType() || elementType->IsArrayType() || elementType->IsStringType())
        {
            int32_t length = arrayObject->Length();
            for (int32_t elementIndex = 0; elementIndex < length; ++elementIndex)
            {
                Object* object = newObject->GetElement(elementIndex);
                object = object->GetObject();
                switch (object->Kind())
                {
                    case ObjectKind::heapObject:
                    {
                        CollectArrayElementObject(static_cast<HeapObject*>(object), newObject, elementIndex, objectMap, mem, current, size, context);
                        break;
                    }
                    case ObjectKind::arrayObject:
                    {
                        CollectArrayElementArray(static_cast<ArrayObject*>(object), newObject, elementIndex, objectMap, mem, current, size, context);
                        break;
                    }
                    case ObjectKind::stringObject:
                    {
                        CollectArrayElementString(static_cast<StringObject*>(object), newObject, elementIndex, objectMap, mem, current, size, context);
                        break;
                    }
                }
            }
        }
    }
}

void CollectArrayElementString(StringObject* stringObject, ArrayObject* parent, int32_t index, std::map<Object*, Object*>& objectMap, uint8_t* mem, uint8_t*& current, int32_t size,
    ExecutionContext* context)
{
    auto it = objectMap.find(stringObject);
    if (it != objectMap.end())
    {
        Object* newObject = it->second;
        parent->SetElement(index, newObject, context);
    }
    else
    {
        StringObject* newObject = CopyString(stringObject, mem, current, size);
        objectMap[stringObject] = newObject;
        parent->SetElement(index, newObject, context);
    }
}

void CollectArray(ArrayObject* arrayObject, HeapObject* parent, int32_t index, std::map<Object*, Object*>& objectMap, uint8_t* mem, uint8_t*& current, int32_t size,
    ExecutionContext* context)
{
    auto it = objectMap.find(arrayObject);
    if (it != objectMap.end())
    {
        Object* newObject = it->second;
        parent->SetField(index, newObject, context);
    }
    else
    {
        ArrayObject* newObject = CopyArray(arrayObject, mem, current, size);
        objectMap[arrayObject] = newObject;
        parent->SetField(index, newObject, context);
        ArrayType* arrayType = arrayObject->GetArrayType();
        Type* elementType = arrayType->ElementType();
        if (elementType->IsObjectType() || elementType->IsArrayType() || elementType->IsStringType())
        {
            int32_t length = arrayObject->Length();
            for (int32_t elementIndex = 0; elementIndex < length; ++elementIndex)
            {
                Object* object = newObject->GetElement(elementIndex);
                object = object->GetObject();
                switch (object->Kind())
                {
                    case ObjectKind::heapObject:
                    {
                        CollectArrayElementObject(static_cast<HeapObject*>(object), newObject, elementIndex, objectMap, mem, current, size, context);
                        break;
                    }
                    case ObjectKind::arrayObject:
                    {
                        CollectArrayElementArray(static_cast<ArrayObject*>(object), newObject, elementIndex, objectMap, mem, current, size, context);
                        break;
                    }
                    case ObjectKind::stringObject:
                    {
                        CollectArrayElementString(static_cast<StringObject*>(object), newObject, elementIndex, objectMap, mem, current, size, context);
                        break;
                    }
                }
            }
        }
    }
}

void CollectString(StringObject* stringObject, HeapObject* parent, int32_t index, std::map<Object*, Object*>& objectMap, uint8_t* mem, uint8_t*& current, int32_t size,
    ExecutionContext* context)
{
    auto it = objectMap.find(stringObject);
    if (it != objectMap.end())
    {
        Object* newObject = it->second;
        parent->SetField(index, newObject, context);
    }
    else
    {
        StringObject* newObject = CopyString(stringObject, mem, current, size);
        objectMap[stringObject] = newObject;
        parent->SetField(index, newObject, context);
    }
}

void CollectObject(HeapObject* object, HeapObject* parent, int32_t index, 
    std::map<Object*, Object*>& objectMap, uint8_t* mem, uint8_t*& current, int32_t size, ExecutionContext* context)
{
    auto it = objectMap.find(object);
    if (it != objectMap.end())
    {
        Object* newObject = it->second;
        parent->SetField(index, newObject, context);
    }
    else
    {
        HeapObject* newObject = CopyObject(object, mem, current, size);
        objectMap[object] = newObject;
        parent->SetField(index, newObject, context);
        ObjectType* objectType = newObject->GetType();
        int32_t fieldCount = objectType->FieldCount();
        for (int32_t fieldIndex = 0; fieldIndex < fieldCount; ++fieldIndex)
        {
            Type* fieldType = objectType->GetFieldType(fieldIndex);
            if (fieldType)
            {
                switch (fieldType->Kind())
                {
                    case TypeKind::objectType:
                    {
                        Object* child = newObject->GetField(fieldIndex);
                        child = child->GetObject();
                        if (child->Kind() == ObjectKind::heapObject)
                        {
                            CollectObject(static_cast<HeapObject*>(child), newObject, fieldIndex, objectMap, mem, current, size, context);
                        }
                        else if (child->Kind() != ObjectKind::nilObject)
                        {
                            throw std::runtime_error("collect_object: invalid object kind");
                        }
                        break;
                    }
                    case TypeKind::arrayType:
                    {
                        Object* child = newObject->GetField(fieldIndex);
                        child = child->GetObject();
                        if (child->Kind() == ObjectKind::arrayObject)
                        {
                            CollectArray(static_cast<ArrayObject*>(child), newObject, fieldIndex, objectMap, mem, current, size, context);
                        }
                        else if (child->Kind() != ObjectKind::nilObject)
                        {
                            throw std::runtime_error("collect_object: invalid array object kind");
                        }
                        break;
                    }
                    case TypeKind::stringType:
                    {
                        Object* child = newObject->GetField(fieldIndex);
                        child = child->GetObject();
                        if (child->Kind() == ObjectKind::stringObject)
                        {
                            CollectString(static_cast<StringObject*>(child), newObject, fieldIndex, objectMap, mem, current, size, context);
                        }
                        else if (child->Kind() != ObjectKind::nilObject)
                        {
                            throw std::runtime_error("collect_object: invalid string object kind");
                        }
                        break;
                    }
                }
            }
        }
    }
}

void CollectFrameObject(HeapObject* object, Frame* frame, int32_t index, 
    std::map<Object*, Object*>& objectMap, uint8_t* mem, uint8_t*& current, int32_t size, ExecutionContext* context)
{
    auto it = objectMap.find(object);
    if (it != objectMap.end())
    {
        Object* newObject = it->second;
        frame->SetRawObject(index, newObject);
    }
    else
    {
        HeapObject* newObject = CopyObject(object, mem, current, size);
        objectMap[object] = newObject;
        frame->SetRawObject(index, newObject);
        ObjectType* objectType = newObject->GetType();
        int32_t fieldCount = objectType->FieldCount();
        for (int32_t fieldIndex = 0; fieldIndex < fieldCount; ++fieldIndex)
        {
            Type* fieldType = objectType->GetFieldType(fieldIndex);
            if (fieldType)
            {
                switch (fieldType->Kind())
                {
                    case TypeKind::objectType:
                    {
                        Object* child = newObject->GetField(fieldIndex);
                        child = child->GetObject();
                        if (child->Kind() == ObjectKind::heapObject)
                        {
                            CollectObject(static_cast<HeapObject*>(child), newObject, fieldIndex, objectMap, mem, current, size, context);
                        }
                        else if (child->Kind() != ObjectKind::nilObject)
                        {
                            throw std::runtime_error("collect_frame_object: invalid object kind");
                        }
                        break;
                    }
                    case TypeKind::arrayType:
                    {
                        Object* child = newObject->GetField(fieldIndex);
                        child = child->GetObject();
                        if (child->Kind() == ObjectKind::arrayObject)
                        {
                            CollectArray(static_cast<ArrayObject*>(child), newObject, fieldIndex, objectMap, mem, current, size, context);
                        }
                        else if (child->Kind() != ObjectKind::nilObject)
                        {
                            throw std::runtime_error("collect_frame_object: invalid array object kind");
                        }
                        break;
                    }
                    case TypeKind::stringType:
                    {
                        Object* child = newObject->GetField(fieldIndex);
                        child = child->GetObject();
                        if (child->Kind() == ObjectKind::stringObject)
                        {
                            CollectString(static_cast<StringObject*>(child), newObject, fieldIndex, objectMap, mem, current, size, context);
                        }
                        else if (child->Kind() != ObjectKind::nilObject)
                        {
                            throw std::runtime_error("collect_frame_object: invalid string object kind");
                        }
                        break;
                    }
                }
            }
        }
    }
}

void CollectFrameArray(ArrayObject* arrayObject, Frame* frame, int32_t index, std::map<Object*, Object*>& objectMap, uint8_t* mem, uint8_t*& current, int32_t size, 
    ExecutionContext* context)
{
    auto it = objectMap.find(arrayObject);
    if (it != objectMap.end())
    {
        Object* newObject = it->second;
        frame->SetRawObject(index, newObject);
    }
    else
    {
        ArrayObject* newObject = CopyArray(arrayObject, mem, current, size);
        objectMap[arrayObject] = newObject;
        frame->SetRawObject(index, newObject);
        ArrayType* arrayType = arrayObject->GetArrayType();
        Type* elementType = arrayType->ElementType();
        if (elementType->IsObjectType() || elementType->IsArrayType() || elementType->IsStringType())
        {
            int32_t length = arrayObject->Length();
            for (int32_t elementIndex = 0; elementIndex < length; ++elementIndex)
            {
                Object* object = newObject->GetElement(elementIndex);
                object = object->GetObject();
                switch (object->Kind())
                {
                    case ObjectKind::heapObject:
                    {
                        CollectArrayElementObject(static_cast<HeapObject*>(object), newObject, elementIndex, objectMap, mem, current, size, context);
                        break;
                    }
                    case ObjectKind::arrayObject:
                    {
                        CollectArrayElementArray(static_cast<ArrayObject*>(object), newObject, elementIndex, objectMap, mem, current, size, context);
                        break;
                    }
                    case ObjectKind::stringObject:
                    {
                        CollectArrayElementString(static_cast<StringObject*>(object), newObject, elementIndex, objectMap, mem, current, size, context);
                        break;
                    }
                }
            }
        }
    }
}

void CollectFrameString(StringObject* stringObject, Frame* frame, int32_t index, std::map<Object*, Object*>& objectMap, uint8_t* mem, uint8_t*& current, int32_t size, 
    ExecutionContext* context)
{
    auto it = objectMap.find(stringObject);
    if (it != objectMap.end())
    {
        Object* newObject = it->second;
        frame->SetRawObject(index, newObject);
    }
    else
    {
        StringObject* newObject = CopyString(stringObject, mem, current, size);
        objectMap[stringObject] = newObject;
        frame->SetRawObject(index,  newObject);
    }
}

void CollectFrame(Frame* frame, std::map<Object*, Object*>& objectMap, uint8_t* mem, uint8_t*& current, int32_t size, ExecutionContext* context)
{
    int32_t frameSize = frame->Size();
    for (int32_t frameIndex = 0; frameIndex < frameSize; ++frameIndex)
    {
        ObjectKind objectKind = frame->GetObjectKind(frameIndex);
/*
        if (objectKind == ObjectKind::valueObject)
        {
            Object* object = frame->GetRawObject(frameIndex);
            object = object->GetObject();
            if (object->Kind() == ObjectKind::stringObject)
            {
                CollectFrameString(static_cast<StringObject*>(object), frame, frameIndex, objectMap, mem, current, size, context);
            }
            else if (object->Kind() == ObjectKind::nilObject)
            {
                Object* nilObject = NilObject::Ptr();
                frame->SetRawObject(frameIndex, nilObject);
            }
        }
*/
        switch (objectKind)
        {
            case ObjectKind::heapObject:
            {
                Object* object = frame->GetRawObject(frameIndex);
                object = object->GetObject();
                if (object->Kind() == ObjectKind::heapObject)
                {
                    CollectFrameObject(static_cast<HeapObject*>(object), frame, frameIndex, objectMap, mem, current, size, context);
                }
                else if (object->Kind() == ObjectKind::nilObject)
                {
                    Object* nilObject = NilObject::Ptr();
                    frame->SetRawObject(frameIndex, nilObject);
                }
                else
                {
                    throw std::runtime_error("collect_frame: invalid object kind");
                }
                break;
            }
            case ObjectKind::heapObjectPtr:
            {
                Object* object = frame->GetRawObject(frameIndex);
                object = object->GetObject();
                if (object->Kind() == ObjectKind::heapObject)
                {
                    CollectFrameObject(static_cast<HeapObject*>(object), frame, frameIndex, objectMap, mem, current, size, context);
                }
                else if (object->Kind() == ObjectKind::nilObject)
                {
                    Object* nilObject = NilObject::Ptr();
                    frame->SetRawObject(frameIndex, nilObject);
                }
                else
                {
                    throw std::runtime_error("collect_frame: invalid object kind");
                }
                break;
            }
            case ObjectKind::arrayObject:
            {
                Object* object = frame->GetRawObject(frameIndex);
                object = object->GetObject();
                if (object->Kind() == ObjectKind::arrayObject)
                {
                    CollectFrameArray(static_cast<ArrayObject*>(object), frame, frameIndex, objectMap, mem, current, size, context);
                }
                else if (object->Kind() == ObjectKind::nilObject)
                {
                    Object* nilObject = NilObject::Ptr();
                    frame->SetRawObject(frameIndex, nilObject);
                }
                else
                {
                    throw std::runtime_error("collect_frame: invalid array object kind");
                }
                break;
            }
            case ObjectKind::arrayObjectPtr:
            {
                Object* object = frame->GetRawObject(frameIndex);
                object = object->GetObject();
                if (object->Kind() == ObjectKind::arrayObject)
                {
                    CollectFrameArray(static_cast<ArrayObject*>(object), frame, frameIndex, objectMap, mem, current, size, context);
                }
                else if (object->Kind() == ObjectKind::nilObject)
                {
                    Object* nilObject = NilObject::Ptr();
                    frame->SetRawObject(frameIndex, nilObject);
                }
                else
                {
                    throw std::runtime_error("collect_frame: invalid array object kind");
                }
                break;
            }
            case ObjectKind::stringObject:
            {
                Object* object = frame->GetRawObject(frameIndex);
                object = object->GetObject();
                if (object->Kind() == ObjectKind::stringObject)
                {
                    CollectFrameString(static_cast<StringObject*>(object), frame, frameIndex, objectMap, mem, current, size, context);
                }
                else if (object->Kind() == ObjectKind::nilObject)
                {
                    Object* nilObject = NilObject::Ptr();
                    frame->SetRawObject(frameIndex, nilObject);
                }
                else
                {
                    throw std::runtime_error("collect_frame: invalid string object kind");
                }
                break;
            }
            case ObjectKind::stringObjectPtr:
            {
                Object* object = frame->GetRawObject(frameIndex);
                object = object->GetObject();
                if (object->Kind() == ObjectKind::stringObject)
                {
                    CollectFrameString(static_cast<StringObject*>(object), frame, frameIndex, objectMap, mem, current, size, context);
                }
                else if (object->Kind() == ObjectKind::nilObject)
                {
                    Object* nilObject = NilObject::Ptr();
                    frame->SetRawObject(frameIndex, nilObject);
                }
                else
                {
                    throw std::runtime_error("collect_frame: invalid string object kind");
                }
                break;
            }
        }
    }
}

void CollectStackObject(HeapObject* object, Stack* stack, int32_t stackIndex, 
    std::map<Object*, Object*>& objectMap, uint8_t* mem, uint8_t*& current, int32_t size, ExecutionContext* context, bool ptr, bool ptrObject)
{
    if (ptr && ptrObject)
    {
        throw std::runtime_error("collect_stack_object: invalid ptr combination");
    }
    auto it = objectMap.find(object);
    if (it != objectMap.end())
    {
        Object* newObject = it->second;
        if (ptr)
        {
            stack->SetValue(stackIndex, new HeapObjectPtr(static_cast<HeapObject*>(newObject)));
        }
        else if (ptrObject)
        {
            stack->SetValue(stackIndex, new PtrObject(static_cast<HeapObject*>(newObject)));
        }
        else
        {
            stack->SetValue(stackIndex, newObject);
        }
    }
    else
    {
        HeapObject* newObject = CopyObject(object, mem, current, size);
        objectMap[object] = newObject;
        if (ptr)
        {
            stack->SetValue(stackIndex, new HeapObjectPtr(newObject));
        }
        else if (ptrObject)
        {
            stack->SetValue(stackIndex, new PtrObject(static_cast<HeapObject*>(newObject)));
        }
        else
        {
            stack->SetValue(stackIndex, newObject);
        }
        ObjectType* objectType = newObject->GetType();
        int32_t fieldCount = objectType->FieldCount();
        for (int32_t fieldIndex = 0; fieldIndex < fieldCount; ++fieldIndex)
        {
            Type* fieldType = objectType->GetFieldType(fieldIndex);
            if (fieldType)
            {
                switch (fieldType->Kind())
                {
                    case TypeKind::objectType:
                    {
                        Object* child = newObject->GetField(fieldIndex);
                        child = child->GetObject();
                        if (!child)
                        {
                            child = NilObject::Ptr();
                        }
                        if (child->Kind() == ObjectKind::heapObject)
                        {
                            CollectObject(static_cast<HeapObject*>(child), newObject, fieldIndex, objectMap, mem, current, size, context);
                        }
                        else if (child->Kind() != ObjectKind::nilObject)
                        {
                            throw std::runtime_error("collect_frame_object: invalid object kind");
                        }
                        break;
                    }
                    case TypeKind::arrayType:
                    {
                        Object* child = newObject->GetField(fieldIndex);
                        child = child->GetObject();
                        if (!child)
                        {
                            child = NilObject::Ptr();
                        }
                        if (child->Kind() == ObjectKind::arrayObject)
                        {
                            CollectArray(static_cast<ArrayObject*>(child), newObject, fieldIndex, objectMap, mem, current, size, context);
                        }
                        else if (child->Kind() != ObjectKind::nilObject)
                        {
                            throw std::runtime_error("collect_frame_object: invalid array object kind");
                        }
                        break;
                    }
                    case TypeKind::stringType:
                    {
                        Object* child = newObject->GetField(fieldIndex);
                        child = child->GetObject();
                        if (child->Kind() == ObjectKind::stringObject)
                        {
                            CollectString(static_cast<StringObject*>(child), newObject, fieldIndex, objectMap, mem, current, size, context);
                        }
                        else if (child->Kind() != ObjectKind::nilObject)
                        {
                            throw std::runtime_error("collect_frame_object: invalid string object kind");
                        }
                        break;
                    }
                }
            }
        }
    }
}

void CollectStackArray(ArrayObject* arrayObject, Stack* stack, int32_t stackIndex,
    std::map<Object*, Object*>& objectMap, uint8_t* mem, uint8_t*& current, int32_t size, ExecutionContext* context, bool ptr, bool ptrObject)
{
    if (ptr && ptrObject)
    {
        throw std::runtime_error("collect_stack_array: invalid ptr combination");
    }
    auto it = objectMap.find(arrayObject);
    if (it != objectMap.end())
    {
        Object* newObject = it->second;
        if (ptr)
        {
            stack->SetValue(stackIndex, new ArrayObjectPtr(static_cast<ArrayObject*>(newObject)));
        }
        else if (ptrObject)
        {
            stack->SetValue(stackIndex, new PtrObject(static_cast<ArrayObject*>(newObject)));
        }
        else
        {
            stack->SetValue(stackIndex, newObject);
        }
    }
    else
    {
        ArrayObject* newObject = CopyArray(arrayObject, mem, current, size);
        objectMap[arrayObject] = newObject;
        if (ptr)
        {
            stack->SetValue(stackIndex, new ArrayObjectPtr(static_cast<ArrayObject*>(newObject)));
        }
        else if (ptrObject)
        {
            stack->SetValue(stackIndex, new PtrObject(static_cast<ArrayObject*>(newObject)));
        }
        else
        {
            stack->SetValue(stackIndex, newObject);
        }
        ArrayType* arrayType = arrayObject->GetArrayType();
        Type* elementType = arrayType->ElementType();
        if (elementType->IsObjectType() || elementType->IsArrayType() || elementType->IsStringType())
        {
            int32_t length = arrayObject->Length();
            for (int32_t elementIndex = 0; elementIndex < length; ++elementIndex)
            {
                Object* object = newObject->GetElement(elementIndex);
                object = object->GetObject();
                switch (object->Kind())
                {
                    case ObjectKind::heapObject:
                    {
                        CollectArrayElementObject(static_cast<HeapObject*>(object), newObject, elementIndex, objectMap, mem, current, size, context);
                        break;
                    }
                    case ObjectKind::arrayObject:
                    {
                        CollectArrayElementArray(static_cast<ArrayObject*>(object), newObject, elementIndex, objectMap, mem, current, size, context);
                        break;
                    }
                    case ObjectKind::stringObject:
                    {
                        CollectArrayElementString(static_cast<StringObject*>(object), newObject, elementIndex, objectMap, mem, current, size, context);
                        break;
                    }
                }
            }
        }
    }
}

void CollectStackString(StringObject* stringObject, Stack* stack, int32_t stackIndex,
    std::map<Object*, Object*>& objectMap, uint8_t* mem, uint8_t*& current, int32_t size, ExecutionContext* context, bool ptr, bool ptrObject)
{
    if (ptr && ptrObject)
    {
        throw std::runtime_error("collect_stack_string: invalid ptr combination");
    }
    auto it = objectMap.find(stringObject);
    if (it != objectMap.end())
    {
        Object* newObject = it->second;
        if (ptr)
        {
            stack->SetValue(stackIndex, new StringObjectPtr(static_cast<StringObject*>(newObject)));
        }
        else if (ptrObject)
        {
            stack->SetValue(stackIndex, new PtrObject(static_cast<StringObject*>(newObject)));
        }
        else
        {
            stack->SetValue(stackIndex, newObject);
        }
    }
    else
    {
        StringObject* newObject = CopyString(stringObject, mem, current, size);
        objectMap[stringObject] = newObject;
        if (ptr)
        {
            stack->SetValue(stackIndex, new StringObjectPtr(static_cast<StringObject*>(newObject)));
        }
        else if (ptrObject)
        {
            stack->SetValue(stackIndex, new PtrObject(static_cast<StringObject*>(newObject)));
        }
        else
        {
            stack->SetValue(stackIndex, newObject);
        }
    }
}

void CollectStack(std::map<Object*, Object*>& objectMap, uint8_t* mem, uint8_t*& current, int32_t size, ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    int32_t stackSize = stack->Values().size();
    for (int32_t stackIndex = 0; stackIndex < stackSize; ++stackIndex)
    {
        bool ptrObject = false;
        Object* object = stack->Values()[stackIndex].get();
        if (object->IsPtrObject())
        {
            ptrObject = true;
            object = object->GetObject();
            if (object->IsValueObject())
            {
                stack->SetValue(stackIndex, object->Clone());
            }
        }
        switch (object->Kind())
        {
            case ObjectKind::heapObject:
            {
                CollectStackObject(static_cast<HeapObject*>(object), stack, stackIndex, objectMap, mem, current, size, context, false, ptrObject);
                break;
            }
            case ObjectKind::heapObjectPtr:
            {
                object = object->GetObject();
                if (object->IsHeapObject())
                {
                    CollectStackObject(static_cast<HeapObject*>(object), stack, stackIndex, objectMap, mem, current, size, context, true, ptrObject);
                }
                else if (object->Kind() != ObjectKind::nilObject)
                {
                    throw std::runtime_error("collect_stack: invalid object kind");
                }
                break;
            }
            case ObjectKind::arrayObject:
            {
                CollectStackArray(static_cast<ArrayObject*>(object), stack, stackIndex, objectMap, mem, current, size, context, false, ptrObject);
                break;
            }
            case ObjectKind::arrayObjectPtr:
            {
                object = object->GetObject();
                if (object->IsArrayObject())
                {
                    CollectStackArray(static_cast<ArrayObject*>(object), stack, stackIndex, objectMap, mem, current, size, context, true, ptrObject);
                }
                else if (object->Kind() != ObjectKind::nilObject)
                {
                    throw std::runtime_error("collect_stack: invalid array object kind");
                }
                break;
            }
            case ObjectKind::stringObject:
            {
                CollectStackString(static_cast<StringObject*>(object), stack, stackIndex, objectMap, mem, current, size, context, false, ptrObject);
                break;
            }
            case ObjectKind::stringObjectPtr:
            {
                object = object->GetObject();
                if (object->IsStringObject())
                {
                    CollectStackString(static_cast<StringObject*>(object), stack, stackIndex, objectMap, mem, current, size, context, true, ptrObject);
                }
                else if (object->Kind() != ObjectKind::nilObject)
                {
                    throw std::runtime_error("collect_stack: invalid string object kind");
                }
                break;
            }
        }
    }
}

Heap::Heap() : Heap(defaultHeapSize)
{
}

Heap::~Heap()
{
    free(mem);
}

Heap::Heap(int32_t size_) : size(size_), mem(static_cast<uint8_t*>(malloc(size))), current(mem), collectionCount(0)
{
    std::memset(current, 0, size);
}

void Heap::Collect(ExecutionContext* context)
{
    ++collectionCount;
    std::map<Object*, Object*> objectMap;
    uint8_t* newMem = static_cast<uint8_t*>(malloc(size));
    std::memset(newMem, 0, size);
    uint8_t* newCurrent = newMem;
    CollectStack(objectMap, newMem, newCurrent, size, context);
    CollectFrame(context->GlobalFrame(), objectMap, newMem, newCurrent, size, context);
    for (const auto& frame : context->Frames())
    {
        CollectFrame(frame.get(), objectMap, newMem, newCurrent, size, context);
    }
    std::swap(mem, newMem);
    std::swap(current, newCurrent);
    free(newMem);
}

HeapObject* Heap::Allocate(ObjectType* objectType, ExecutionContext* context)
{
    int32_t fieldCount = objectType->FieldCount();
    int32_t allocationSize = HeapObjectSize(fieldCount);
    int32_t currentSize = current - mem;
    if (currentSize + allocationSize > size)
    {
        Collect(context);
        currentSize = current - mem;
    }
    if (currentSize + allocationSize > size)
    {
        throw std::runtime_error("heap: out of memory");
    }
    HeapObject* heapObject = new (static_cast<void*>(current))HeapObject(objectType, fieldCount, allocationSize);
    heapObject->Init(context);
    current += allocationSize;
    return heapObject;
}

ArrayObject* Heap::AllocateArray(ArrayType* arrayType, int32_t arrayLength, ExecutionContext* context)
{
    int32_t allocationSize = sizeof(ArrayObject) + arrayLength * valueSize;
    int32_t currentSize = current - mem;
    if (currentSize + allocationSize > size)
    {
        Collect(context);
        currentSize = current - mem;
    }
    if (currentSize + allocationSize > size)
    {
        throw std::runtime_error("heap: out of memory");
    }
    ArrayObject* arrayObject = new (static_cast<void*>(current))ArrayObject(arrayType, arrayLength, allocationSize);
    arrayObject->Init(context);
    current += allocationSize;
    return arrayObject;
}

StringObject* Heap::AllocateString(const std::string& str, ExecutionContext* context)
{
    int32_t allocationSize = sizeof(StringObject) + str.length() + 1;
    int32_t currentSize = current - mem;
    if (currentSize + allocationSize > size)
    {
        Collect(context);
        currentSize = current - mem;
    }
    if (currentSize + allocationSize > size)
    {
        throw std::runtime_error("heap: out of memory");
    }
    StringObject* stringObject = new (static_cast<void*>(current))StringObject(str, allocationSize);
    current += allocationSize;
    return stringObject;
}

int32_t HeapObjectSize(int32_t fieldCount)
{
    return sizeof(HeapObject) + fieldCount * valueSize;
}

} // namespace p
