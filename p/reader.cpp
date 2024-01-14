// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.reader;

import p.instruction;

namespace p {

Reader::Reader(const std::string& filePath_) : 
    filePath(filePath_),
    file(filePath, util::OpenMode::binary | util::OpenMode::read),
    buffered(file),
    binaryReader(buffered),
    block(nullptr),
    modulePart(nullptr),
    context(nullptr)
{
}

void Reader::PushBlock(Block* block_)
{
    blockStack.push(block);
    block = block_;
}

void Reader::PopBlock()
{
    block = blockStack.top();
    blockStack.pop();
}

void Reader::PushModulePart(ModulePart* modulePart_)
{
    modulePartStack.push(modulePart);
    modulePart = modulePart_;
}

void Reader::PopModulePart()
{
    modulePart = modulePartStack.top();
    modulePartStack.pop();
}

void Reader::ClearInstructionMap()
{
    instructionMap.clear();
}

void Reader::MapInstruction(Instruction* instruction)
{
    instructionMap[instruction->InstIndex()] = instruction;
}

Instruction* Reader::GetInstruction(int32_t instIndex)
{
    auto it = instructionMap.find(instIndex);
    if (it != instructionMap.end())
    {
        return it->second;
    }
    else
    {
        throw std::runtime_error("error loading module file '" + filePath + "': instruction " + std::to_string(instIndex) + " not found");
    }
}

} // namespace p
