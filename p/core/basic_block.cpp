// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.core.basic_block;

import p.core.subroutine_symbol;
import p.core.writer;
import p.core.reader;

namespace p {

BasicBlock::BasicBlock(SubroutineSymbol* subroutine_) : instructions(subroutine_), numInstructions(0)
{
}

void BasicBlock::AddInstruction(Instruction* instruction)
{
    ++numInstructions;
    instructions.AddChild(instruction);
}

Instruction* BasicBlock::First()
{
    if (instructions.IsEmpty())
    {
        throw std::runtime_error("basic block is empty");
    }
    return static_cast<Instruction*>(instructions.FirstChild());
}

Instruction* BasicBlock::Last()
{
    if (instructions.IsEmpty())
    {
        throw std::runtime_error("basic block is empty");
    }
    return static_cast<Instruction*>(instructions.LastChild());
}

BasicBlock* BasicBlock::Next()
{
    return static_cast<BasicBlock*>(NextSibling());
}

SubroutineSymbol* BasicBlock::GetSubroutine()
{
    return static_cast<SubroutineSymbol*>(instructions.Parent());
}

void BasicBlock::Write(SymbolWriter& writer)
{
    writer.GetBinaryWriter().Write(numInstructions);
    Instruction* instruction = First();
    while (instruction != nullptr)
    {
        writer.GetBinaryWriter().Write(static_cast<uint8_t>(instruction->Kind()));
        instruction->Write(writer);
        instruction = instruction->Next();
    }
}

void BasicBlock::Read(SymbolReader& reader)
{
    int32_t n = reader.GetBinaryReader().ReadInt();
    for (int32_t i = 0; i < n; ++i)
    {
        InstructionKind kind = static_cast<InstructionKind>(reader.GetBinaryReader().ReadByte());
        Instruction* instruction = MakeInstruction(kind);
        instruction->Read(reader);
        reader.MapInstruction(instruction);
        AddInstruction(instruction);
    }
}

void BasicBlock::Resolve(SymbolReader& reader)
{
    Instruction* instruction = First();
    while (instruction)
    {
        instruction->Resolve(reader);
        instruction = instruction->Next();
    }
}

void BasicBlock::Execute(ExecutionContext* context)
{
    Instruction* instruction = First();
    while (instruction)
    {
        instruction = instruction->Execute(context);
    }
}

void BasicBlock::Print(util::CodeFormatter& formatter, ExecutionContext* context)
{
    Instruction* instruction = First();
    while (instruction)
    {
        formatter.WriteLine(instruction->ToString(context));
        instruction = instruction->Next();
    }
}

} // namespace p
