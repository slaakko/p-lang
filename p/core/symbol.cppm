// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.core.symbol;

import soul.ast.span;
import util;
import std.core;

export namespace p {

extern std::string pcodeHeaderMagic;

class SymbolWriter;
class SymbolReader;
class TypeSymbol;
class SymbolTable;
class Context;
class ExecutionContext;
class ContainerSymbol;
class BlockSymbol;

enum class SymbolKind
{
    none, programSymbol, unitSymbol, unitPartSymbol, blockSymbol,
    booleanTypeSymbol, integerTypeSymbol, charTypeSymbol, enumeratedTypeSymbol, subrangeTypeSymbol, realTypeSymbol, stringTypeSymbol, pointerTypeSymbol, nilTypeSymbol,
    objectTypeSymbol, arrayTypeSymbol, aliasTypeSymbol, typeParamSymbol, boundTypeParamSymbol, specializationSymbol,
    constantSymbol, variableSymbol, parameterSymbol, fieldSymbol, procedureSymbol, functionSymbol, constructorSymbol, constructorGroupSymbol, externalSubroutineSymbol,
    ordSymbol, chrSymbol, sinSymbol, cosSymbol, arcTanSymbol, sqrtSymbol, absSymbol, succSymbol, predSymbol, minSymbol, maxSymbol, writeSymbol, writelnSymbol
};

std::string SymbolKindStr(SymbolKind kind);

class Symbol
{
public:
    Symbol(SymbolKind kind_, const soul::ast::Span& span_, const std::string& name_);
    virtual ~Symbol();
    SymbolKind Kind() const { return kind; }
    bool IsProgramSymbol() const { return kind == SymbolKind::programSymbol; }
    bool IsUnitSymbol() const { return kind == SymbolKind::unitSymbol; }
    bool IsUnitPartSymbol() const { return kind == SymbolKind::unitPartSymbol; }
    bool IsBlockSymbol() const { return kind == SymbolKind::blockSymbol; }
    bool IsConstantSymbol() const { return kind == SymbolKind::constantSymbol; }
    bool IsBooleanTypeSymbol() const { return kind == SymbolKind::booleanTypeSymbol; }
    bool IsIntegerTypeSymbol() const { return kind == SymbolKind::integerTypeSymbol; }
    bool IsRealTypeSymbol() const { return kind == SymbolKind::realTypeSymbol; }
    bool IsStringTypeSymbol() const { return kind == SymbolKind::stringTypeSymbol; }
    bool IsPointerTypeSymbol() const { return kind == SymbolKind::pointerTypeSymbol; }
    bool IsNilTypeSymbol() const { return kind == SymbolKind::nilTypeSymbol; }
    bool IsObjectTypeSymbol() const { return kind == SymbolKind::objectTypeSymbol; }
    bool IsArrayTypeSymbol() const { return kind == SymbolKind::arrayTypeSymbol; }
    bool IsAliasTypeSymbol() const { return kind == SymbolKind::aliasTypeSymbol; }
    bool IsConstructorSymbol() const { return kind == SymbolKind::constructorSymbol; }
    bool IsConstructorGroupSymbol() const { return kind == SymbolKind::constructorGroupSymbol; }
    bool IsParameterSymbol() const { return kind == SymbolKind::parameterSymbol; }
    bool IsVariableSymbol() const { return kind == SymbolKind::variableSymbol; }
    bool IsFieldSymbol() const { return kind == SymbolKind::fieldSymbol; }
    bool IsTypeParamSymbol() const { return kind == SymbolKind::typeParamSymbol; }
    bool IsBoundTypeParamSymbol() const { return kind == SymbolKind::boundTypeParamSymbol; }
    bool IsSpecializationSymbol() const { return kind == SymbolKind::specializationSymbol; }
    bool IsObjectTypeOrSpecializationSymbol() const { return IsObjectTypeSymbol() || IsSpecializationSymbol(); }
    const soul::ast::Span& Span() const { return span; }
    const std::string& Name() const { return name; }
    std::string FullName() const;
    ContainerSymbol* Parent() const { return parent; }
    void SetParent(ContainerSymbol* parent_) { parent = parent_; }
    virtual const std::string& SourceFilePath() const;
    virtual void Write(SymbolWriter& writer) = 0;
    virtual void Read(SymbolReader& reader) = 0;
    virtual void Resolve();
    BlockSymbol* CurrentBlock() const;
    virtual SymbolTable* GetSymbolTable() const;
    virtual bool IsTypeSymbol() const { return false; }
    virtual bool IsProcedureSymbol() const { return false; }
    virtual bool IsFunctionSymbol() const { return false; }
    virtual bool IsSubroutineSymbol() const { return false; }
    virtual void MakeVmts();
    virtual void CheckDefined();
    virtual void Print(util::CodeFormatter& formatter, bool full, ExecutionContext* context);
private:
    SymbolKind kind;
    soul::ast::Span span;
    std::string name;
    ContainerSymbol* parent;
};

Symbol* CreateSymbol(SymbolKind kind, const soul::ast::Span& span, const std::string& name);

} // namespace p
