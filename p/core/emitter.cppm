// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.core.emitter;

import p.core.instruction;
import std.core;

export namespace p {

class SubroutineSymbol;
class BoundCompoundStatementNode;
class BasicBlock;
class Context;

class Patch
{
public:
    Patch(BasicBlock* block_);
    virtual ~Patch();
    BasicBlock* GetBasicBlock() const { return block; }
    virtual void Apply();
private:
    BasicBlock* block;
};

class BranchTruePatch : public Patch
{
public:
    BranchTruePatch(BasicBlock* block_, BranchInstruction* branchInstruction_);
    void Apply() override;
private:
    BranchInstruction* branchInstruction;
};

class BranchFalsePatch : public Patch
{
public:
    BranchFalsePatch(BasicBlock* block_, BranchInstruction* branchInstruction_);
    void Apply() override;
private:
    BranchInstruction* branchInstruction;
};

class JumpNextPatch : public Patch
{
public:
    JumpNextPatch(BasicBlock* block_, JumpInstruction* jumpInstruction_);
    void Apply() override;
private:
    JumpInstruction* jumpInstruction;
};

class Emitter
{
public:
    Emitter(const std::string& filePath_, Context* context_);
    const std::string& FilePath() const { return filePath; }
    void Emit(Instruction* instruction);
    SubroutineSymbol* GetCurrentSubroutine() const { return currentSubroutine; }
    void SetCurrentSubroutine(SubroutineSymbol* currentSubroutine_);
    BasicBlock* CurrentBasicBlock() const { return currentBasicBlock; }
    void PushBasicBlock(BasicBlock* basicBlock);
    void PopBasicBlock();
    void IncrementOpenBasicBlocks();
    void EnqueuePatch(Patch* patch);
    void ApplyPatches();
    Context* GetContext() const { return context; }
private:
    std::string filePath;
    SubroutineSymbol* currentSubroutine;
    int32_t instructionIndex;
    BasicBlock* currentBasicBlock;
    std::stack<BasicBlock*> basicBlockStack;
    std::map<BasicBlock*, std::vector<std::unique_ptr<Patch>>> patchMap;
    Context* context;
    int32_t openBasicBlocks;
};

} // namespace p
