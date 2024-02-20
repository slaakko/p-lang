// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.core.container_symbol;

import p.ast;
import p.core.symbol;
import util;
import soul.ast.span;
import std.core;

export namespace p {

class ObjectTypeSymbol;
class ArrayTypeSymbol;
class ProcedureSymbol;
class FunctionSymbol;
class ConstructorSymbol;
class SubroutineSymbol;
class ParameterSymbol;
class VariableSymbol;
class ConstructorGroupSymbol;
class FieldSymbol;
class GlobalVariableMap;

class ContainerSymbol : public Symbol
{
public:
    ContainerSymbol(SymbolKind kind_, const soul::ast::Span& span_, const std::string& name_);
    ~ContainerSymbol();
    virtual void AddSymbol(Symbol* symbol);
    void Resolve() override;
    void Import(ContainerSymbol* that);
    void MapSymbol(Symbol* symbol);
    void MapArrayTypeSymbol(ArrayTypeSymbol* arrayTypeSymbol);
    Symbol* GetSymbol(const std::string& symbolName, Node* node, Context* context) const;
    virtual Symbol* GetSymbol(const std::string& symbolName, Node* node, bool searchBase, bool mustExist) const;
    TypeSymbol* GetType(const std::string& typeName, Node* node, Context* context) const;
    ArrayTypeSymbol* GetArrayType(const std::string& elementTypeName) const;
    VariableSymbol* GetVariable(const std::string& variableName, Node* node, Context* context) const;
    ObjectTypeSymbol* GetObjectType(const std::string& objectTypeName, Node* node, Context* context) const;
    ProcedureSymbol* GetProcedure(const std::string& procedureName) const;
    ProcedureSymbol* GetProcedure(const std::string& procedureName, Node* node, bool mustExist) const;
    FunctionSymbol* GetFunction(const std::string& functionName, Node* node, bool mustExist) const;
    ConstructorSymbol* GetConstructor(const std::vector<TypeSymbol*>& parameterTypes) const;
    ConstructorSymbol* GetOrInsertConstructor(std::vector<std::unique_ptr<ParameterSymbol>>& parameters, Node* node, bool mustExist);
    SubroutineSymbol* GetSubroutineNoThrow(const std::string& subroutineName) const;
    FieldSymbol* GetField(const std::string& fieldName) const;
    FieldSymbol* GetFieldNoThrow(const std::string& fieldName) const;
    const std::vector<std::unique_ptr<Symbol>>& Symbols() const { return symbols; }
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void MakeVmts() override;
    void CheckDefined() override;
    void SetGlobalVariableMap(GlobalVariableMap* globalVariableMap_);
    GlobalVariableMap* GetGlobalVariableMap() const;
    void Print(util::CodeFormatter& formatter, bool full, ExecutionContext* context) override;
private:
    std::vector<std::unique_ptr<Symbol>> symbols;
    std::map<std::string, Symbol*> symbolMap;
    std::map<std::string, ArrayTypeSymbol*> arrayTypeSymbolMap;
    std::set<ArrayTypeSymbol*> arrayTypesToMap;
    ConstructorGroupSymbol* constructorGroup;
    GlobalVariableMap* globalVariableMap;
};

} // namespace p
