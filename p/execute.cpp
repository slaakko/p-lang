// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.execute;

import p.value;

namespace p {

Frame::Frame(int32_t size_, const std::vector<Parameter>& parameters, Subroutine* subroutine_) : 
    size(size_), mem(static_cast<uint8_t*>(malloc(size * valueSize))), subroutine(subroutine_)
{
    std::memset(mem, 0, size * valueSize);
    parameterQualifiers.resize(size);
    int32_t n = parameters.size();
    for (int32_t i = 0; i < n; ++i)
    {
        const auto& parameter = parameters[i];
        parameterQualifiers[i] = parameter.Qualifier();
    }
    owners.resize(size);
    objectKinds.resize(size);
}

Frame::~Frame()
{
    free(mem);
}

const OwnerIndex& Frame::GetOwner(int32_t index)
{
    if (index >= 0 && index < size)
    {
        return owners[index];
    }
    else
    {
        throw std::runtime_error("get_owner: invalid frame index");
    }
}

void Frame::SetOwner(int32_t index, const OwnerIndex& ownerIndex)
{
    if (index >= 0 && index < size)
    {
        owners[index] = ownerIndex;
    }
    else
    {
        throw std::runtime_error("set_owner: invalid frame index");
    }
}

ObjectKind Frame::GetObjectKind(int index) const
{
    if (index >= 0 && index < size)
    {
        return objectKinds[index];
    }
    else
    {
        throw std::runtime_error("get_object_kind: invalid frame index");
    }
}

Object* Frame::GetRawObject(int32_t index)
{
    if (index >= 0 && index < size)
    {
        uint8_t* addr = mem + index * valueSize;
        Object* object = static_cast<Object*>(static_cast<void*>(addr));
        return object;
    }
    else
    {
        throw std::runtime_error("get_raw_object: invalid frame index");
    }
}

void Frame::SetRawObject(int32_t index, Object* object)
{
    if (index >= 0 && index < size)
    {
        uint8_t* addr = mem + index * valueSize;
        void* ptr = static_cast<void*>(addr);
        switch (object->Kind())
        {
            case ObjectKind::heapObject:
            {
                HeapObjectPtr heapObjectPtr(static_cast<HeapObject*>(object));
                new (ptr)HeapObjectPtr(heapObjectPtr);
                break;
            }
            case ObjectKind::arrayObject:
            {
                ArrayObjectPtr arrayObjectPtr(static_cast<ArrayObject*>(object));
                new (ptr)ArrayObjectPtr(arrayObjectPtr);
                break;
            }
            case ObjectKind::stringObject:
            {
                StringObjectPtr stringObjectPtr(static_cast<StringObject*>(object));
                new (ptr)StringObjectPtr(stringObjectPtr);
                break;
            }
            case ObjectKind::nilObject:
            {
                HeapObjectPtr heapObjectPtr(static_cast<HeapObject*>(object));
                new (ptr)HeapObjectPtr(heapObjectPtr);
                break;
            }
            default:
            {
                throw std::runtime_error("set_raw_object: invalid object kind");
            }
        }
    }
    else
    {
        throw std::runtime_error("set_raw_object: invalid frame index");
    }
}

Object* Frame::GetObject(int32_t index)
{
    if (index >= 0 && index < size)
    {
        uint8_t* addr = mem + index * valueSize;
        if (HasNullContent(addr))
        {
            return nullptr;
        }
        Object* object = static_cast<Object*>(static_cast<void*>(addr));
        const OwnerIndex& ownerIndex = GetOwner(index);
        if (ownerIndex.IsDefault())
        {
            object->SetOwner(this);
            object->SetOwnerIndex(index);
        }
        else
        {
            object = ownerIndex.owner->GetObject(ownerIndex.index);
        }
        return object;
    }
    else
    {
        throw std::runtime_error("get_object: invalid frame index");
    }
}

void Frame::SetObject(int32_t index, Object* object, ExecutionContext* context)
{
    if (index >= 0 && index < size)
    {
        if (parameterQualifiers[index] == ParameterQualifier::varParam)
        {
            const OwnerIndex& ownerIndex = GetOwner(index);
            if (!ownerIndex.IsDefault())
            {
                ownerIndex.owner->SetObject(ownerIndex.index, object, context);
            }
            else
            {
                OwnerIndex ownerIndex(object->Owner(), object->OwnerIndex());
                SetOwner(index, ownerIndex);
            }
        }
        uint8_t* addr = mem + index * valueSize;
        void* ptr = static_cast<void*>(addr);
        objectKinds[index] = object->Kind();
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
                        new (ptr)BooleanValue(*booleanValue);
                        break;
                    }
                    case ValueKind::enumerationValue:
                    {
                        EnumerationValue* enumerationValue = static_cast<EnumerationValue*>(value);
                        IntegerValue integerValue(enumerationValue->GetIntegerValue());
                        new (ptr)IntegerValue(integerValue);
                        break;
                    }
                    case ValueKind::integerValue:
                    {
                        IntegerValue* integerValue = static_cast<IntegerValue*>(value);
                        new (ptr)IntegerValue(*integerValue);
                        break;
                    }
                    case ValueKind::charValue:
                    {
                        CharValue* charValue = static_cast<CharValue*>(value);
                        new (ptr)CharValue(*charValue);
                        break;
                    }
                    case ValueKind::realValue:
                    {
                        RealValue* realValue = static_cast<RealValue*>(value);
                        new (ptr)RealValue(*realValue);
                        break;
                    }
                    case ValueKind::stringValue:
                    {
                        StringValue* stringValue = static_cast<StringValue*>(value);
                        Heap* heap = context->GetHeap();
                        StringObject* stringObject = heap->AllocateString(stringValue->GetStringValue(), context);
                        StringObjectPtr stringObjectPtr(stringObject);
                        new (ptr)StringObjectPtr(stringObjectPtr);
                        objectKinds[index] = stringObject->Kind();
                        break;
                    }
                    case ValueKind::genericPointerValue:
                    {
                        GenericPointerValue* pointerValue = static_cast<GenericPointerValue*>(value);
                        new (ptr)GenericPointerValue(*pointerValue);
                        break;
                    }
                    case ValueKind::objectValue:
                    {
                        PtrObject objectPtr(value);
                        new (ptr)PtrObject(objectPtr);
                        break;
                    }
                    case ValueKind::arrayValue:
                    {
                        PtrObject objectPtr(value);
                        new (ptr)PtrObject(objectPtr);
                        break;
                    }
                }
                break;
            }
            case ObjectKind::heapObjectPtr:
            {
                HeapObjectPtr* heapObjectPtr = static_cast<HeapObjectPtr*>(object);
                new (ptr)HeapObjectPtr(*heapObjectPtr);
                break;
            }
            case ObjectKind::heapObject:
            {
                HeapObjectPtr heapObjectPtr(static_cast<HeapObject*>(object));
                new (ptr)HeapObjectPtr(heapObjectPtr);
                break;
            }
            case ObjectKind::arrayObjectPtr:
            {
                ArrayObjectPtr* arrayObjectPtr = static_cast<ArrayObjectPtr*>(object);
                new (ptr)ArrayObjectPtr(*arrayObjectPtr);
                break;
            }
            case ObjectKind::arrayObject:
            {
                ArrayObjectPtr arrayObjectPtr(static_cast<ArrayObject*>(object));
                new (ptr)ArrayObjectPtr(arrayObjectPtr);
                break;
            }
            case ObjectKind::stringObject:
            {
                StringObjectPtr stringObjectPtr(static_cast<StringObject*>(object));
                new (ptr)StringObjectPtr(stringObjectPtr);
                break;
            }
            case ObjectKind::stringObjectPtr:
            {
                StringObjectPtr* stringObjectPtr = static_cast<StringObjectPtr*>(object);
                new (ptr)StringObjectPtr(*stringObjectPtr);
                break;
            }
            case ObjectKind::nilObject:
            {
                HeapObjectPtr heapObjectPtr(static_cast<HeapObject*>(object));
                new (ptr)HeapObjectPtr(heapObjectPtr);
                break;
            }
        }
    }
    else
    {
        throw std::runtime_error("set_object: invalid frame index");
    }
}

Stack::Stack()
{
}

void Stack::Push(Object* value)
{
    values.push_back(std::unique_ptr<Object>(value));
}

void Stack::Dup()
{
    if (values.empty())
    {
        throw std::runtime_error("stack is empty");
    }
    values.push_back(std::unique_ptr<Object>(values.back()->Clone()));
}

std::unique_ptr<Object> Stack::Pop()
{
    if (values.empty())
    {
        throw std::runtime_error("stack is empty");
    }
    std::unique_ptr<Object> value = std::move(values.back());
    values.pop_back();
    return value;
}

void Stack::SetValue(int32_t index, Object* value)
{
    if (index >= 0 && index < values.size())
    {
        values[index].reset(value);
    }
    else
    {
        throw std::runtime_error("invalid stack index");
    }
}

ExecutionContext::ExecutionContext() : currentSubroutine(nullptr), next(nullptr), heap(nullptr), globalVariableMap(nullptr)
{
}

Frame* ExecutionContext::GlobalFrame() const
{
    return globalVariableMap->GlobalFrame();
}

Frame* ExecutionContext::CurrentFrame()
{
    if (frames.empty())
    {
        throw std::runtime_error("current_frame: frames empty");
    }
    return frames.back().get();
}

Frame* ExecutionContext::ParentFrame(int32_t parentLevel)
{
    if (parentLevel >= 0 && parentLevel < parentFrames.size())
    {
        if (!parentFrames[parentLevel])
        {
            throw std::runtime_error("parent_frame: parent frame " + std::to_string(parentLevel) + " is null");
        }
        return parentFrames[parentLevel];
    }
    else
    {
        throw std::runtime_error("parent_frame: invalid parent level " + std::to_string(parentLevel));
    }
}

void ExecutionContext::SetParentFrame(int32_t parentLevel, Frame* frame)
{
    while (parentLevel != -1 && parentLevel >= parentFrames.size())
    {
        parentFrames.push_back(nullptr);
    }
    if (parentLevel != -1)
    {
        parentFrames[parentLevel] = frame;
    }
}

void ExecutionContext::PushFrame(Frame* frame)
{
    frames.push_back(std::unique_ptr<Frame>(frame));
}

void ExecutionContext::PopFrame()
{
    if (frames.empty())
    {
        throw std::runtime_error("pop_frame: frames empty");
    }
    frames.pop_back();
}

void ExecutionContext::PushSubroutine(Subroutine* subroutine)
{
    subroutineStack.push(currentSubroutine);
    currentSubroutine = subroutine;
}

void ExecutionContext::PopSubroutine()
{
    currentSubroutine = subroutineStack.top();
    subroutineStack.pop();
}

void ExecutionContext::PushArgumentCount(int32_t argumentCount_)
{
    argumentCountStack.push(argumentCount);
    argumentCount = argumentCount_;
}

void ExecutionContext::PopArgumentCount()
{
    argumentCount = argumentCountStack.top();
    argumentCountStack.pop();
}

} // namespace p
