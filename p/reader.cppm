// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.reader;

import util;
import std.core;

export namespace p {

class Block;
class Instruction;
class ModulePart;
class Context;

class Reader
{
public:
    Reader(const std::string& filePath_);
    util::BinaryStreamReader& GetBinaryReader() { return binaryReader; }
    void PushBlock(Block* block_);
    void PopBlock();
    Block* GetBlock() const { return block; }
    void PushModulePart(ModulePart* modulePart_);
    void PopModulePart();
    ModulePart* GetModulePart() const { return modulePart; }
    void ClearInstructionMap();
    void MapInstruction(Instruction* instruction);
    Instruction* GetInstruction(int32_t instIndex);
    void SetContext(Context* context_) { context = context_; }
    Context* GetContext() const { return context; }
private:
    std::string filePath;
    util::FileStream file;
    util::BufferedStream buffered;
    util::BinaryStreamReader binaryReader;
    Block* block;
    std::stack<Block*> blockStack;
    ModulePart* modulePart;
    std::stack<ModulePart*> modulePartStack;
    std::map<int32_t, Instruction*> instructionMap;
    Context* context;
};

} // namespace p
