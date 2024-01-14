// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.heap;

import p.object;
import std.core;

export namespace p {

const int32_t defaultHeapSize = 16384 * 1024; // 16 MB

class Heap
{
public:
    Heap();
    ~Heap();
    Heap(int32_t size_);
    HeapObject* Allocate(ObjectType* objectType, ExecutionContext* context);
    ArrayObject* AllocateArray(ArrayType* arrayType, int32_t arraySize, ExecutionContext* context);
    StringObject* AllocateString(const std::string& str, ExecutionContext* context);
    int32_t CollectionCount() const { return collectionCount; }
private:
    void Collect(ExecutionContext* context);
    int32_t size;
    uint8_t* mem;
    uint8_t* current;
    int32_t collectionCount;
};

int32_t HeapObjectSize(int32_t fieldCount);

} // namespace p
