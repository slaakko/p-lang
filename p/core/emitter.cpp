// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.core.emitter;

import p.core.basic_block;
import p.core.subroutine_symbol;

namespace p {

Patch::Patch(BasicBlock* block_) : block(block_)
{
}

Patch::~Patch()
{
}

void Patch::Apply()
{
}

BranchTruePatch::BranchTruePatch(BasicBlock* block_, BranchInstruction* branchInstruction_) : Patch(block_), branchInstruction(branchInstruction_)
{
}

void BranchTruePatch::Apply()
{
    branchInstruction->SetTrueNext(GetBasicBlock()->First());
}

BranchFalsePatch::BranchFalsePatch(BasicBlock* block_, BranchInstruction* branchInstruction_) : Patch(block_), branchInstruction(branchInstruction_)
{
}

void BranchFalsePatch::Apply()
{
    branchInstruction->SetFalseNext(GetBasicBlock()->First());
}

JumpNextPatch::JumpNextPatch(BasicBlock* block_, JumpInstruction* jumpInstruction_) : Patch(block_), jumpInstruction(jumpInstruction_)
{
}

void JumpNextPatch::Apply()
{
    jumpInstruction->SetTarget(GetBasicBlock()->First());
}


Emitter::Emitter(const std::string& filePath_, Context* context_) : filePath(filePath_),
    currentSubroutine(nullptr), currentBasicBlock(nullptr), instructionIndex(0), context(context_), openBasicBlocks(0)
{
}

void Emitter::SetCurrentSubroutine(SubroutineSymbol* currentSubroutine_)
{
    currentSubroutine = currentSubroutine_;
    currentBasicBlock = currentSubroutine->AddBasicBlock();
}

void Emitter::PushBasicBlock(BasicBlock* basicBlock)
{
    basicBlockStack.push(currentBasicBlock);
    currentBasicBlock = basicBlock;
}

void Emitter::PopBasicBlock()
{
    while (openBasicBlocks > 0)
    {
        currentBasicBlock = basicBlockStack.top();
        basicBlockStack.pop();
        --openBasicBlocks;
    }
    currentBasicBlock = basicBlockStack.top();
    basicBlockStack.pop();
}

void Emitter::IncrementOpenBasicBlocks()
{
    ++openBasicBlocks;
}

void Emitter::Emit(Instruction* instruction)
{
    currentBasicBlock->AddInstruction(instruction);
    instruction->SetInstIndex(instructionIndex++);
    ApplyPatches();
}

void Emitter::EnqueuePatch(Patch* patch)
{
    std::vector<std::unique_ptr<Patch>>& patches = patchMap[patch->GetBasicBlock()];
    patches.push_back(std::unique_ptr<Patch>(patch));
}

void Emitter::ApplyPatches()
{
    if (currentBasicBlock && !currentBasicBlock->IsEmpty())
    {
        auto it = patchMap.find(currentBasicBlock);
        if (it != patchMap.end())
        {
            std::vector<std::unique_ptr<Patch>>& patches = patchMap[currentBasicBlock];
            for (const auto& patch : patches)
            {
                patch->Apply();
            }
            patchMap.erase(currentBasicBlock);
        }
    }
}

} // namespace p
