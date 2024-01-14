// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.basic_block;

import p.instruction;
import util;
import std.core;

export namespace p {

class Subroutine;
class ExecutionContext;

class BasicBlock : public util::Component
{
public:
    BasicBlock(Subroutine* subroutine_);
    void AddInstruction(Instruction* instruction);
    Instruction* First();
    Instruction* Last();
    BasicBlock* Next();
    Subroutine* GetSubroutine();
    void Write(Writer& writer);
    void Read(Reader& reader);
    void Resolve(Reader& reader);
    void Execute(ExecutionContext* context);
    bool IsEmpty() const { return instructions.IsEmpty(); }
    void Print(util::CodeFormatter& formatter, ExecutionContext* context);
private:
    int32_t numInstructions;
    util::Container instructions;
};

} // namespace p
