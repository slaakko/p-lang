// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.core.context;

import p.ast;
import p.core.subroutine_symbol;
import std.core;

export namespace p {

class UnitLoader;
class SymbolTable;
class UnitHeaderCache;

class Context
{
public:
    Context();
    void SetUnitLoader(UnitLoader* unitLoader_) { unitLoader = unitLoader_; }
    UnitLoader* GetUnitLoader() const { return unitLoader; }
    SymbolTable* GetSymbolTable() const { return symbolTable; }
    void SetSymbolTable(SymbolTable* symbolTable_) { symbolTable = symbolTable_; }
    void SetCurrentConstructor(ConstructorSymbol* currentConstructor_) { currentConstructor = currentConstructor_; }
    ConstructorSymbol* CurrentConstructor() const { return currentConstructor; }
    Node* GetNode() const { return node; }
    void PushNode(Node* node_);
    void PopNode();
    UnitPartKind GetCurrentUnitPartKind() const { return currentUnitPartKind; }
    void SetCurrentUnitPartKind(UnitPartKind unitPartKind) { currentUnitPartKind = unitPartKind; }
    bool Verbose() const { return verbose; }
    void SetVerbose() { verbose = true; }
    void AddUpdatedUnit(const std::string& updatedUnitName);
    const std::set<std::string>& UpdatedUnits() const { return updatedUnits; }
    bool UnitUpdated(const std::string& unitName) const;
    bool UpdateUnits() const { return updateUnits; }
    void SetUpdateUnits() { updateUnits = true; }
private:
    UnitLoader* unitLoader;
    SymbolTable* symbolTable;
    ConstructorSymbol* currentConstructor;
    std::stack<Node*> nodeStack;
    Node* node;
    UnitPartKind currentUnitPartKind;
    bool verbose;
    std::set<std::string> updatedUnits;
    bool updateUnits;
};

} // namespace p
