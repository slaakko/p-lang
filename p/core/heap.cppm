// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.core.heap;

import p.core.object;
import std.core;

export namespace p {

const int32_t defaultHeapSize = 16384 * 1024; // 16 MB

class Heap
{
public:
    Heap();
    ~Heap();
    Heap(int32_t size_);
    HeapObject* Allocate(ObjectTypeSymbol* objectType, ExecutionContext* context);
    ArrayObject* AllocateArray(ArrayTypeSymbol* arrayType, int32_t arraySize, ExecutionContext* context);
    StringObject* AllocateString(const std::string& str, ExecutionContext* context);
    int32_t CollectionCount() const { return collectionCount; }
    void RemoveDisposeObject(HeapObject* heapObject);
private:
    void Collect(ExecutionContext* context);
    void DisposeObjects(ExecutionContext* context);
    int32_t size;
    uint8_t* mem;
    uint8_t* current;
    int32_t collectionCount;
    std::set<HeapObject*> objectsToDispose;
};

int32_t HeapObjectSize(int32_t fieldCount);

} // namespace p
