// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.basic_block;

import p.subroutine;
import p.writer;
import p.reader;

namespace p {

BasicBlock::BasicBlock(Subroutine* subroutine_) : instructions(subroutine_), numInstructions(0)
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

Subroutine* BasicBlock::GetSubroutine()
{
    return static_cast<Subroutine*>(instructions.Parent());
}

void BasicBlock::Write(Writer& writer)
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

void BasicBlock::Read(Reader& reader)
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

void BasicBlock::Resolve(Reader& reader)
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
