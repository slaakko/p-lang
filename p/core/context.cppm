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
class SpecializationSymbol;

enum class ContextFlags
{
    none, verbose = 1 << 0, updateUnits = 1 << 1, instantiatingGeneric = 1 << 2, noThrow = 1 << 3
};

constexpr ContextFlags operator|(ContextFlags left, ContextFlags right)
{
    return ContextFlags(int(left) | int(right));
}

constexpr ContextFlags operator&(ContextFlags left, ContextFlags right)
{
    return ContextFlags(int(left) & int(right));
}

constexpr ContextFlags operator~(ContextFlags flags)
{
    return ContextFlags(~int(flags));
}

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
    bool GetFlag(ContextFlags flag) const { return (flags & flag) != ContextFlags::none; }
    void SetFlag(ContextFlags flag) { flags = flags | flag; }
    void PushFlags(ContextFlags flags_);
    void PopFlags();
    bool Verbose() const { return GetFlag(ContextFlags::verbose); }
    void SetVerbose() { SetFlag(ContextFlags::verbose); }
    void AddUpdatedUnit(const std::string& updatedUnitName);
    const std::set<std::string>& UpdatedUnits() const { return updatedUnits; }
    bool UnitUpdated(const std::string& unitName) const;
    bool UpdateUnits() const { return GetFlag(ContextFlags::updateUnits); }
    void SetUpdateUnits() { SetFlag(ContextFlags::updateUnits); }
    bool InstantiatingGeneric() const { return GetFlag(ContextFlags::instantiatingGeneric); }
    void PushInstantiatingGeneric();
    void PopInstantiatingGeneric();
    bool NoThrow() const { return GetFlag(ContextFlags::noThrow); }
    void PushNoThrow();
    void PopNoThrow();
    void PushSpecialization(SpecializationSymbol* specialization_);
    void PopSpecialization();
    SpecializationSymbol* Specialization() const { return specialization; }
    const std::string& SpecializationName() const { return specializationName; }
    void SetSpecializationName(const std::string& specializationName_);
private:
    UnitLoader* unitLoader;
    SymbolTable* symbolTable;
    ConstructorSymbol* currentConstructor;
    std::stack<Node*> nodeStack;
    Node* node;
    UnitPartKind currentUnitPartKind;
    std::set<std::string> updatedUnits;
    ContextFlags flags;
    std::stack<ContextFlags> flagStack;
    SpecializationSymbol* specialization;
    std::stack<SpecializationSymbol*> specializationStack;
    std::string specializationName;
};

} // namespace p
