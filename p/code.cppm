// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.code;

import soul.lexer;
import p.instruction;
import p.mod;
import p.type;
import std.core;

export namespace p {

class Block;
class Subroutine;
class BoundCompoundStatementNode;
class BasicBlock;

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
    Emitter(soul::lexer::LexerBase<char>& lexer_, int64_t pos, Context* context_);
    soul::lexer::LexerBase<char>& Lexer() { return lexer; }
    void Emit(Instruction* instruction);
    Subroutine* GetCurrentSubroutine() const { return currentSubroutine; }
    void SetCurrentSubroutine(Subroutine* currentSubroutine_);
    BasicBlock* CurrentBasicBlock() const { return currentBasicBlock; }
    void PushBasicBlock(BasicBlock* basicBlock);
    void PopBasicBlock();
    void IncrementOpenBasicBlocks();
    void EnqueuePatch(Patch* patch);
    void ApplyPatches();
    Context* GetContext() const { return context; }
private:
    soul::lexer::LexerBase<char>& lexer;
    Subroutine* currentSubroutine;
    int32_t instructionIndex;
    BasicBlock* currentBasicBlock;
    std::stack<BasicBlock*> basicBlockStack;
    std::map<BasicBlock*, std::vector<std::unique_ptr<Patch>>> patchMap;
    Context* context;
    int32_t openBasicBlocks;
};

void GenerateCode(Subroutine* subroutine, Block* block, BoundCompoundStatementNode* boundCompoundStatementNode, soul::lexer::LexerBase<char>& lexer, int64_t pos, Context* context);

} // namespace p
