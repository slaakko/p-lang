// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.execute;

import p.object;
import p.variable;
import p.code;
import p.context;
import p.subroutine;
import p.instruction;
import p.heap;
import std.core;

export namespace p {

namespace execute {}

class ExecutionContext;

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
    Frame(int32_t size_, const std::vector<Parameter>& parameters, Subroutine* subroutine_);
    ~Frame();
    Object* GetRawObject(int32_t index);
    void SetRawObject(int32_t index, Object* object);
    Object* GetObject(int32_t index);
    void SetObject(int32_t index, Object* object, ExecutionContext* context);
    const OwnerIndex& GetOwner(int32_t index);
    void SetOwner(int32_t index, const OwnerIndex& ownerIndex);
    int32_t Size() const { return size; }
    ObjectKind GetObjectKind(int index) const;
    Subroutine* GetSubroutine() const { return subroutine; }
private:
    int32_t size;
    uint8_t* mem;
    std::vector<ParameterQualifier> parameterQualifiers;
    std::vector<OwnerIndex> owners;
    std::vector<ObjectKind> objectKinds;
    Subroutine* subroutine;
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
    Heap* GetHeap() const { return heap; }
    void SetHeap(Heap* heap_) { heap = heap_; }
    void PushFrame(Frame* frame);
    void PopFrame();
    void PushSubroutine(Subroutine* subroutine);
    void PopSubroutine();
    Subroutine* CurrentSubroutine() const { return currentSubroutine; }
    int32_t ArgumentCount() const { return argumentCount; }
    void PushArgumentCount(int32_t argumentCount_);
    void PopArgumentCount();
    void SetNext(Instruction* next_) { next = next_; }
    Instruction* Next() const { return next; }
    void SetGlobalVariableMap(GlobalVariableMap* globalVariableMap_) { globalVariableMap = globalVariableMap_; }
    GlobalVariableMap* GetGlobalVariableMap() const { return globalVariableMap; }
private:
    Stack stack;
    std::vector<std::unique_ptr<Frame>> frames;
    std::vector<Frame*> parentFrames;
    Subroutine* currentSubroutine;
    std::stack<Subroutine*> subroutineStack;
    Instruction* next;
    Heap* heap;
    int32_t argumentCount;
    std::stack<int32_t> argumentCountStack;
    GlobalVariableMap* globalVariableMap;
};

} // namespace p
