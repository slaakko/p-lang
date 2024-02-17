// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.core.execution_context;

import p.ast;
import p.core.object;
import p.core.context;
import std.core;

export namespace p {

class Heap;
class Frame;
class ExecutionContext;
class ParameterSymbol;
class SubroutineSymbol;
class GlobalVariableMap;

struct OwnerIndex
{
    OwnerIndex() : owner(nullptr), index(-1) {}
    OwnerIndex(Frame* owner_, int32_t index_) : owner(owner_), index(index_) {}
    bool IsDefault() const { return owner == nullptr; }
    Frame* owner;
    int32_t index;
};

class Frame
{
public:
    Frame(int32_t size_, const std::vector<ParameterSymbol*>& parameters, SubroutineSymbol* subroutine_);
    ~Frame();
    Object* GetRawObject(int32_t index);
    void SetRawObject(int32_t index, Object* object);
    Object* GetObject(int32_t index);
    void SetObject(int32_t index, Object* object, ExecutionContext* context);
    const OwnerIndex& GetOwner(int32_t index);
    void SetOwner(int32_t index, const OwnerIndex& ownerIndex);
    int32_t Size() const { return size; }
    ObjectKind GetObjectKind(int index) const;
    SubroutineSymbol* GetSubroutine() const { return subroutine; }
private:
    int32_t size;
    uint8_t* mem;
    std::vector<ParameterQualifier> parameterQualifiers;
    std::vector<OwnerIndex> owners;
    std::vector<ObjectKind> objectKinds;
    SubroutineSymbol* subroutine;
};

class Stack
{
public:
    Stack();
    void Push(Object* value);
    void Dup();
    std::unique_ptr<Object> Pop();
    const std::vector<std::unique_ptr<Object>>& Values() const { return values; }
    void SetValue(int32_t index, Object* value);
private:
    std::vector<std::unique_ptr<Object>> values;
};

class ExecutionContext : public Context
{
public:
    ExecutionContext();
    Stack* GetStack() { return &stack; }
    Frame* GlobalFrame() const;
    const std::vector<std::unique_ptr<Frame>>& Frames() const { return frames; }
    Frame* CurrentFrame();
    Frame* ParentFrame(int32_t parentLevel);
    void SetParentFrame(int32_t parentLevel, Frame* frame);
    void PushFrame(Frame* frame);
    void PopFrame();
    void PushSubroutine(SubroutineSymbol* subroutine);
    void PopSubroutine();
    SubroutineSymbol* CurrentSubroutine() const { return currentSubroutine; }
    Heap* GetHeap() { return heap; }
    void SetHeap(Heap* heap_) { heap = heap_; }
    int32_t ArgumentCount() const { return argumentCount; }
    void PushArgumentCount(int32_t argumentCount_);
    void PopArgumentCount();
    void SetGlobalVariableMap(GlobalVariableMap* globalVariableMap_) { globalVariableMap = globalVariableMap_; }
    GlobalVariableMap* GetGlobalVariableMap() const { return globalVariableMap; }
private:
    Stack stack;
    Heap* heap;
    std::vector<std::unique_ptr<Frame>> frames;
    std::vector<Frame*> parentFrames;
    SubroutineSymbol* currentSubroutine;
    std::stack<SubroutineSymbol*> subroutineStack;
    int32_t argumentCount;
    std::stack<int32_t> argumentCountStack;
    GlobalVariableMap* globalVariableMap;
};

} // namespace p
