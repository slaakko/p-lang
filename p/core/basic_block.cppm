// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.core.basic_block;

import p.core.instruction;
import util;
import std.core;

export namespace p {

class SubroutineSymbol;
class ExecutionContext;

class BasicBlock : public util::Component
{
public:
    BasicBlock(SubroutineSymbol* subroutine_);
    void AddInstruction(Instruction* instruction);
    Instruction* First();
    Instruction* Last();
    BasicBlock* Next();
    SubroutineSymbol* GetSubroutine();
    void Write(SymbolWriter& writer);
    void Read(SymbolReader& reader);
    void Resolve(SymbolReader& reader);
    void Execute(ExecutionContext* context);
    bool IsEmpty() const { return instructions.IsEmpty(); }
    void Print(util::CodeFormatter& formatter, ExecutionContext* context);
private:
    int32_t numInstructions;
    util::Container instructions;
};

} // namespace p
