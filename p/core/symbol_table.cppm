// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.core.symbol_table;

import p.core.root_symbol;
import p.ast;
import util;
import soul.ast.span;
import std.core;

export namespace p {

class ConstantSymbol;
class SubroutineSymbol;
class ExecutionContext;

class SymbolTable
{
public:
    SymbolTable();
    void SetRoot(RootSymbol* root_);
    RootSymbol* Root() const { return root.get(); }
    void Import(SymbolTable* symbolTableToImport);
    void Write(SymbolWriter& writer);
    void Read(SymbolReader& reader);
    TypeSymbol* GetType(const util::uuid& id) const;
    TypeSymbol* GetType(const util::uuid& id, Node* node) const;
    TypeSymbol* GetType(const util::uuid& id, const std::string& sourceFilePath, const soul::ast::Span& span) const;
    TypeSymbol* GetType(const std::string& typeName, const std::string& sourceFilePath, const soul::ast::Span& span) const;
    ArrayTypeSymbol* GetArrayType(const util::uuid& elementTypeId, Node* node) const;
    TypeSymbol* GetFundamentalType(SymbolKind kind, Node* node) const;
    void MapType(TypeSymbol* type);
    void AddTypeToRoot(TypeSymbol* type);
    void MapConstant(ConstantSymbol* constantSymbol);
    ConstantSymbol* GetConstant(const util::uuid& id, const std::string& sourceFilePath, const soul::ast::Span& span);
    ConstantSymbol* GetConstant(const util::uuid& id) const;
    void MapSubroutine(SubroutineSymbol* subroutine);
    SubroutineSymbol* GetSubroutine(const util::uuid& id) const;
    SubroutineSymbol* GetSubroutine(const util::uuid& id, const std::string& sourceFilePath, const soul::ast::Span& span) const;
    void AddSymbol(Symbol* symbol);
    void Resolve();
    void MakeVmts();
    void CheckDefined();
    void RunInits(ExecutionContext* context);
    void AddUsedUnitName(const std::string& usedUnitName);
    void Print(util::CodeFormatter& formatter, ExecutionContext* context);
private:
    std::unique_ptr<RootSymbol> root;
    std::map<util::uuid, TypeSymbol*> typeIdMap;
    std::map<std::string, TypeSymbol*> typeNameMap;
    std::map<util::uuid, ArrayTypeSymbol*> arrayTypeIdMap;
    std::map<util::uuid, ConstantSymbol*> constantIdMap;
    std::map<util::uuid, SubroutineSymbol*> subroutineIdMap;
    std::vector<Symbol*> symbols;
    std::vector<SymbolTable*> importedUnits;
    std::vector<std::string> usedUnitNames;
    bool initRun;
};

} // namespace p
