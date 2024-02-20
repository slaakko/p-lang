// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.core.subroutine_symbol;

import p.ast;
import p.core.container_symbol;
import p.core.basic_block;
import util;
import soul.ast.span;
import std.core;

export namespace p {

class BlockSymbol;
class ParameterSymbol;
class ExecutionContext;
class Emitter;

enum class SubroutineFlags 
{
    none = 0, external = 1 << 0, forward = 1 << 1, standard = 1 << 2, defined = 1 << 3, generated = 1 << 4
};

constexpr SubroutineFlags operator|(SubroutineFlags left, SubroutineFlags right)
{
    return SubroutineFlags(int(left) | int(right));
}

constexpr SubroutineFlags operator&(SubroutineFlags left, SubroutineFlags right)
{
    return SubroutineFlags(int(left) & int(right));
}

constexpr SubroutineFlags operator~(SubroutineFlags flags)
{
    return SubroutineFlags(~int(flags));
}

std::string SubroutineFlagsStr(SubroutineFlags flags);

class SubroutineSymbol : public ContainerSymbol, public util::Component
{
public:
    SubroutineSymbol(SymbolKind kind_, const soul::ast::Span& span_, const std::string& name_);
    SubroutineSymbol(SymbolKind kind_, const soul::ast::Span& span_, const std::string& name_, const util::uuid& id_);
    void AddSymbol(Symbol* symbol) override;
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    bool IsSubroutineSymbol() const override { return true; }
    virtual ObjectTypeSymbol* GetObjectType() const;
    const std::vector<ParameterSymbol*>& Parameters() const { return parameters; }
    void SetParameters(std::vector<std::unique_ptr<ParameterSymbol>>& parameters_);
    virtual int ParameterCount() const { return parameters.size(); }
    void CheckParameters(const std::vector<std::unique_ptr<ParameterSymbol>>& parameters_, Node* node);
    void SetFrameSize(Context* context);
    bool Match(const std::vector<TypeSymbol*>& parameterTypes, int& numConversions) const;
    SubroutineFlags Flags() const { return flags; }
    const util::uuid& Id() const { return id; }
    void SetVmtIndex(int32_t vmtIndex_) { vmtIndex = vmtIndex_; }
    int32_t VmtIndex() const { return vmtIndex;}
    bool IsExternal() const { return (flags & SubroutineFlags::external) != SubroutineFlags::none; }
    void SetExternal() { flags = flags | SubroutineFlags::external; }
    bool IsForward() const { return (flags & SubroutineFlags::forward) != SubroutineFlags::none; }
    void SetForward() { flags = flags | SubroutineFlags::forward; }
    bool IsStandard() const { return (flags & SubroutineFlags::standard) != SubroutineFlags::none; }
    void SetStandard() { flags = flags | SubroutineFlags::standard; }
    bool IsDefined() const { return (flags & SubroutineFlags::defined) != SubroutineFlags::none; }
    void SetDefined() { flags = flags | SubroutineFlags::defined; }
    void ResetDefined() { flags = flags & ~SubroutineFlags::defined; }
    bool IsGenerated() const { return (flags & SubroutineFlags::generated) != SubroutineFlags::none; }
    void SetGenerated() { flags = flags | SubroutineFlags::generated; }
    ParameterSymbol* ThisParam() const { return parameters[0]; }
    void SetVirtual(Virtual virtual__, Node* node);
    Virtual GetVirtual() const { return virtual_; }
    BlockSymbol* Block() const { return block; }
    virtual bool IsVariableParamSubroutine() const { return false; }
    virtual void Execute(ExecutionContext* context);
    void SetExternalSubroutineId(const util::uuid& externalSubroutineId_);
    const util::uuid& GetExternalSubroutineId() const { return externalSubroutineId; }
    VariableSymbol* MakeTempVar(const soul::ast::Span& span, TypeSymbol* type);
    BasicBlock* AddBasicBlock();
    BasicBlock* FirstBasicBlock() { return static_cast<BasicBlock*>(code.FirstChild()); }
    BasicBlock* LastBasicBlock() { return static_cast<BasicBlock*>(code.LastChild()); }
    int32_t Level() const { return level; }
    void SetLevel(int32_t level_) { level = level_; }
    void MakeVmts() override;
    void CheckDefined() override;
    void Print(util::CodeFormatter& formatter, bool full, ExecutionContext* context) override;
    void SetSourceFilePath(const std::string& sourceFilePath_);
    const std::string& SourceFilePath() const override;
private:
    SubroutineFlags flags;
    util::uuid id;
    int32_t level;
    int32_t vmtIndex;
    Virtual virtual_;
    std::vector<ParameterSymbol*> parameters;
    std::vector<VariableSymbol*> variables;
    std::string sourceFilePath;
    BlockSymbol* block;
    util::uuid externalSubroutineId;
    int32_t nextTempVarIndex;
    int32_t frameSize;
    int32_t numBasicBlocks;
    util::Container code;
};

class ProcedureSymbol : public SubroutineSymbol
{
public:
    ProcedureSymbol(const soul::ast::Span& span_, const std::string& name_);
    ProcedureSymbol(SymbolKind kind_, const soul::ast::Span& span_, const std::string& name_);
    bool IsProcedureSymbol() const override { return true; }
};

class FunctionSymbol : public SubroutineSymbol
{
public:
    FunctionSymbol(const soul::ast::Span& span_, const std::string& name_);
    FunctionSymbol(SymbolKind kind_, const soul::ast::Span& span_, const std::string& name_);
    virtual TypeSymbol* ResultType(const std::vector<TypeSymbol*>& argumentTypes, Node* node) const;
    virtual Value* Evaluate(const std::vector<std::unique_ptr<Value>>& argumentValues, Context* context, Node* node) const;
    virtual void GenerateCode(Emitter* emitter, const soul::ast::Span& span);
    void SetResultType(TypeSymbol* resultType_) { resultType = resultType_; }
    void CheckResultType(TypeSymbol* resultType_, Node* node);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void Resolve() override;
    bool IsFunctionSymbol() const override { return true; }
private:
    TypeSymbol* resultType;
    util::uuid resultTypeId;
};

class ConstructorSymbol : public SubroutineSymbol
{
public:
    ConstructorSymbol(const soul::ast::Span& span_, const std::string& name_);
    ObjectTypeSymbol* GetObjectType() const override;
    void SetConstructorCall(Node* constructorCall_) { constructorCall = constructorCall_; }
    Node* ConstructorCall() const { return constructorCall; }
private:
    Node* constructorCall;
};

class ConstructorGroupSymbol : public ContainerSymbol
{
public:
    ConstructorGroupSymbol(const soul::ast::Span& span_, const std::string& name_);
    const std::vector<std::unique_ptr<ConstructorSymbol>>& Constructors() const { return constructors; }
    ConstructorSymbol* GetConstructor(const std::vector<TypeSymbol*>& parameterTypes) const;
    ConstructorSymbol* GetConstructor(const std::vector<std::unique_ptr<ParameterSymbol>>& parameters) const;
    void AddConstructor(ConstructorSymbol* constructor, Node* node);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void CheckDefined() override;
    void Print(util::CodeFormatter& formatter, bool full, ExecutionContext* context) override;
private:
    std::vector<std::unique_ptr<ConstructorSymbol>> constructors;
};

} // namespace p
