// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.core.reader;

import soul.ast.span;
import p.ast;
import std.core;

export namespace p {

class Symbol;
class ContainerSymbol;
class SymbolTable;
class Value;
class Context;
class Instruction;

class SymbolReader : public AstReader
{
public:
    SymbolReader(const std::string& filePath_);
    void ReadHeader();
    Symbol* ReadSymbol(ContainerSymbol* parent);
    void SetSymbolTable(SymbolTable* symbolTable_) { symbolTable = symbolTable_; }
    SymbolTable* GetSymbolTable() const { return symbolTable; }
    Value* ReadValue();
    void SetContext(Context* context_) { context = context_; }
    Context* GetContext() const { return context; }
    void ClearInstructionMap();
    void MapInstruction(Instruction* instruction);
    Instruction* GetInstruction(int32_t instIndex);
private:
    SymbolTable* symbolTable;
    Context* context;
    std::map<int32_t, Instruction*> instructionMap;
};

} // namespace p
