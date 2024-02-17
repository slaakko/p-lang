// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.core.variable_symbol;

import p.ast;
import p.core.symbol;
import util;
import std.core;

export namespace p {

class Frame;
class Object;
class ExecutionContext;

class VariableSymbol : public Symbol
{
public:
    VariableSymbol(const soul::ast::Span& span_, const std::string& name_);
    VariableSymbol(SymbolKind kind_, const soul::ast::Span& span_, const std::string& name_);
    TypeSymbol* Type() const { return type; }
    void SetType(TypeSymbol* type_) { type = type_; }
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void Resolve() override;
    virtual bool IsConst() const { return false; }
    bool IsGlobal() const { return global; }
    void SetGlobal() { global = true; }
    int32_t Index() const { return index; }
    void SetIndex(int32_t index_) { index = index_; }
    const util::uuid& GlobalId() const { return globalId; }
    void Print(util::CodeFormatter& formatter, bool full, ExecutionContext* context) override;
private:
    TypeSymbol* type;
    util::uuid typeId;
    bool global;
    int32_t index;
    util::uuid globalId;
};

class ParameterSymbol : public VariableSymbol
{
public:
    ParameterSymbol(const soul::ast::Span& span_, const std::string& name_);
    ParameterQualifier Qualifier() const { return qualifier; }
    void SetQualifier(ParameterQualifier qualifier_) { qualifier = qualifier_; }
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    bool IsConst() const override { return qualifier == ParameterQualifier::constParam; }
    void Print(util::CodeFormatter& formatter, bool full, ExecutionContext* context) override;
private:
    ParameterQualifier qualifier;
};

class GlobalVariableMap
{
public:
    GlobalVariableMap();
    void Import(GlobalVariableMap* that);
    void AddVariable(VariableSymbol* variable);
    VariableSymbol* GetVariable(const util::uuid& id) const;
    void AllocateFrame();
    int32_t GetObjectIndex(const util::uuid& id) const;
    Object* GetObject(int32_t index) const;
    void SetObject(int32_t index, Object* object, ExecutionContext* context);
    Frame* GlobalFrame() const { return globalFrame.get(); }
    void Print(util::CodeFormatter& formatter, bool full, ExecutionContext* context);
private:
    std::map<util::uuid, VariableSymbol*> variableMap;
    std::map<util::uuid, int32_t> objectIdMap;
    std::unique_ptr<Frame> globalFrame;
};

} // namespace p
