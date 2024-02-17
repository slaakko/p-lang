// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.core.reader;

import p.core.symbol;
import p.core.symbol_table;
import p.core.value;
import p.core.instruction;

namespace p {

SymbolReader::SymbolReader(const std::string& filePath_) : AstReader(filePath_), symbolTable(nullptr), context(nullptr)
{
}

void SymbolReader::ReadHeader()
{
    std::string headerMagic;
    for (int i = 0; i < pcodeHeaderMagic.length(); ++i)
    {
        headerMagic.append(1, GetBinaryReader().ReadChar());
    }
    if (headerMagic != pcodeHeaderMagic)
    {
        throw std::runtime_error("unknown PCODE format version read from file '" + FilePath() + "': " + pcodeHeaderMagic + " expected. Please compile project(s) from sources.");
    }
}

Symbol* SymbolReader::ReadSymbol(ContainerSymbol* parent)
{
    SymbolKind kind = static_cast<SymbolKind>(GetBinaryReader().ReadByte());
    if (kind == SymbolKind::none)
    {
        return nullptr;
    }
    else
    {
        soul::ast::Span span = ReadSpan();
        std::string name = GetBinaryReader().ReadUtf8String();
        Symbol* symbol = CreateSymbol(kind, span, name);
        symbol->SetParent(parent);
        symbol->Read(*this);
        symbolTable->AddSymbol(symbol);
        return symbol;
    }
}

Value* SymbolReader::ReadValue()
{
    ValueKind kind = static_cast<ValueKind>(GetBinaryReader().ReadByte());
    if (kind == ValueKind::none)
    {
        return nullptr;
    }
    else
    {
        Value* value = MakeValue(kind);
        value->Read(*this);
        return value;
    }
}

void SymbolReader::ClearInstructionMap()
{
    instructionMap.clear();
}

void SymbolReader::MapInstruction(Instruction* instruction)
{
    instructionMap[instruction->InstIndex()] = instruction;
}

Instruction* SymbolReader::GetInstruction(int32_t instIndex)
{
    auto it = instructionMap.find(instIndex);
    if (it != instructionMap.end())
    {
        return it->second;
    }
    else
    {
        throw std::runtime_error("error loading PCODE file '" + FilePath() + "': instruction " + std::to_string(instIndex) + " not found");
    }
}

} // namespace p
