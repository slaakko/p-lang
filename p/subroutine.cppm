// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.subroutine;

import p.ast;
import p.basic_block;
import p.instruction;
import p.variable;
import util;
import soul.lexer;
import std.core;

export namespace p {

namespace subroutine {}

class Block;
class Value;
class Emitter;
class Writer;
class Reader;
class ObjectType;
class Context;

enum class ParameterQualifier
{
    valueParam, varParam, constParam
};

class Parameter : public Variable
{
public:
    Parameter();
    Parameter(const std::string& name_, ParameterQualifier qualifier_, Type* type_);
    ParameterQualifier Qualifier() const { return qualifier; }
    void Write(Writer& writer);
    void Read(Reader& reader);
    void Print(util::CodeFormatter& formatter);
    bool IsVarParam() const { return qualifier == ParameterQualifier::varParam; }
    bool IsConst() const override { return qualifier == ParameterQualifier::constParam;  }
private:
    ParameterQualifier qualifier;
};

enum class SubroutineHeadingKind
{
    procedureHeading, functionHeading, constructorHeading
};

enum class Virtuality
{
    none, virtual_, override_
};

class SubroutineHeading
{
public:
    SubroutineHeading(SubroutineHeadingKind kind_);
    SubroutineHeading(SubroutineHeadingKind kind_, const std::string& fullName_);
    SubroutineHeadingKind Kind() const { return kind; }
    bool IsProcedureHeading() const { return kind == SubroutineHeadingKind::procedureHeading; }
    bool IsFunctionHeading() const { return kind == SubroutineHeadingKind::functionHeading; }
    bool IsConstructorHeading() const { return kind == SubroutineHeadingKind::constructorHeading; }
    const std::string& FullName() const { return fullName; }
    const std::string& CommonName() const { return commonName; }
    void SetCommonName(const std::string& commonName_);
    void AddParameter(Parameter& parameter);
    void SetParamIndeces();
    const std::vector<Parameter>& Parameters() const { return parameters; }
    std::vector<Parameter>& Parameters() { return parameters; }
    void InsertThisParam();
    Parameter* ThisParam() { return &parameters[0]; }
    std::vector<Type*> ParameterTypes() const;
    void SetObjectType(ObjectType* objectType_) { objectType = objectType_; }
    ObjectType* GetObjectType() const { return objectType; }
    void SetVirtuality(Virtuality virtuality_) { virtuality = virtuality_; }
    Virtuality GetVirtuality() const { return virtuality; }
    virtual void Write(Writer& writer);
    virtual void Read(Reader& reader);
    virtual void Resolve(Context* context);
    virtual void Print(util::CodeFormatter& formatter);
private:
    SubroutineHeadingKind kind;
    std::string fullName;
    std::string commonName;
    std::vector<Parameter> parameters;
    Virtuality virtuality;
    ObjectType* objectType;
    util::uuid objectTypeId;
};

enum class SubroutineKind
{
    procedure, function, constructor, external
};

std::string SubroutineKindStr(SubroutineKind kind);

enum class DeclarationKind
{
    declaration, definition
};

enum class SubroutineFlags 
{
    none = 0, generated = 1 << 1, implementationGenerated = 1 << 2
};

constexpr SubroutineFlags operator|(SubroutineFlags left, SubroutineFlags right)
{
    return SubroutineFlags(int(left) | int(right));
}

constexpr SubroutineFlags operator&(SubroutineFlags left, SubroutineFlags right)
{
    return SubroutineFlags(int(left) & int(right));
}

constexpr SubroutineFlags operator&(SubroutineFlags flags)
{
    return SubroutineFlags(~int(flags));
}

class Subroutine : public util::Component
{
public:
    Subroutine(SubroutineKind kind_, SubroutineHeading* heading_);
    virtual ~Subroutine();
    virtual void Write(Writer& writer);
    virtual void Read(Reader& reader);
    void ResolveHeading(Context* context);
    void Resolve(Context* context);
    SubroutineKind Kind() const { return kind; }
    void SetKind(SubroutineKind kind_) { kind = kind_; }
    bool IsProcedure() const { return kind == SubroutineKind::procedure; }
    bool IsFunction() const { return kind == SubroutineKind::function; }
    bool IsConstructor() const { return kind == SubroutineKind::constructor; }
    DeclarationKind GetDeclarationKind() const { return declarationKind; }
    bool IsDeclaration() const { return declarationKind == DeclarationKind::declaration; }
    bool IsDefinition() const { return declarationKind == DeclarationKind::definition; }
    void SetAsDeclaration() { declarationKind = DeclarationKind::declaration; }
    SubroutineHeading* Heading() const { return heading.get(); }
    void SetDeclaration(Subroutine* declaration_) { declaration = declaration_; }
    Subroutine* GetDeclaration() const { return declaration; }
    void SetForward() { forward = true; }
    bool IsForward() const { return forward; }
    void SetExternal() { external = true; }
    bool IsExternal() const { return external; }
    void SetBlock(Block* block_);
    Block* GetBlock() const { return block.get(); }
    const std::string& FullName() const { return heading->FullName(); }
    const std::string& CommonName() const { return heading->CommonName(); }
    virtual std::string InfoName() const { return FullName(); }
    const std::vector<Parameter>& Parameters() const { return heading->Parameters(); }
    std::vector<Parameter>& Parameters() { return heading->Parameters(); }
    virtual int32_t MinParameterCount() const { return heading->Parameters().size(); }
    virtual int32_t ParameterCount() const { return heading->Parameters().size(); }
    void AddParameters(Block* block);
    void SetParamIndeces();
    Variable* MakeTempVar(Block* block, Type* type);
    int32_t ModuleId() const { return moduleId; }
    void SetModuleId(int32_t moduleId_) { moduleId = moduleId_; }
    int32_t Id() const { return id; }
    void SetId(int32_t id_) { id = id_; }
    int32_t ImplementationId() const { return implementationId; }
    void SetImplementationId(int32_t implementationId_) { implementationId = implementationId_; }
    void SetFrameSize(ParsingContext* context);
    int32_t FrameSize() const { return frameSize; }
    void SetFrameSize(int32_t frameSize_) { frameSize = frameSize_; }
    virtual void Execute(ExecutionContext* context);
    virtual void ResolveDeclaration(ParsingContext* context, soul::lexer::LexerBase<char>& lexer, int64_t pos);
    BasicBlock* AddBasicBlock();
    BasicBlock* FirstBasicBlock() { return static_cast<BasicBlock*>(code.FirstChild()); }
    BasicBlock* LastBasicBlock() { return static_cast<BasicBlock*>(code.LastChild()); }
    std::string ExternalSubroutineName() const;
    virtual void Print(util::CodeFormatter& formatter, ExecutionContext* context);
    void CheckInterface();
    void MakeVmts(Context* context);
    int32_t VmtIndex() const { return vmtIndex; }
    void SetVmtIndex(int32_t vmtIndex_) { vmtIndex = vmtIndex_; }
    void SetGenerated() { flags = flags | SubroutineFlags::generated; }
    bool IsGenerated() const { return (flags & SubroutineFlags::generated) != SubroutineFlags::none; }
    void SetImplementationGenerated() { flags = flags | SubroutineFlags::implementationGenerated; }
    bool IsImplementationGenerated() const { return (flags & SubroutineFlags::implementationGenerated) != SubroutineFlags::none; }
    bool IsVirtualOrOverride() const { return heading->GetVirtuality() != Virtuality::none; }
    int32_t Level() const { return level; }
    void SetLevel(int32_t level_) { level = level_; }
private:
    SubroutineKind kind;
    DeclarationKind declarationKind;
    std::unique_ptr<SubroutineHeading> heading;
    Subroutine* declaration;
    SubroutineFlags flags;
    bool forward;
    bool external;
    int32_t level;
    int32_t nextTempVarIndex;
    std::unique_ptr<Block> block;
    int32_t moduleId;
    int32_t id;
    int32_t implementationId;
    int32_t vmtIndex;
    int32_t frameSize;
    int32_t numBasicBlocks;
    util::Container code;
};

class ProcedureHeading : public SubroutineHeading
{
public:
    ProcedureHeading();
    ProcedureHeading(const std::string& fullName_);
    ProcedureHeading(ParsingContext* context, QualifiedIdNode* qualifiedId, soul::lexer::LexerBase<char>& lexer, int64_t pos);
};

enum class ProcedureKind
{
    none, regularproc, standardproc
};

class Procedure : public Subroutine
{
public:
    Procedure();
    Procedure(ProcedureKind kind_, const std::string& fullName_);
    Procedure(ProcedureKind kind_, ProcedureHeading* heading_);
    ProcedureKind Kind() const { return kind; }
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    std::string InfoName() const override;
    const ProcedureHeading* GetProcedureHeading() const { return static_cast<const ProcedureHeading*>(Heading()); }
    bool IsStandardProcedure() const { return kind == ProcedureKind::standardproc; }
    bool IsRegularProcedure() const { return kind == ProcedureKind::regularproc; }
    void ResolveDeclaration(ParsingContext* context, soul::lexer::LexerBase<char>& lexer, int64_t pos) override;
private:
    ProcedureKind kind;
};

class FunctionHeading : public SubroutineHeading
{
public:
    FunctionHeading();
    FunctionHeading(const std::string& fullName_);
    FunctionHeading(ParsingContext* context, QualifiedIdNode* qualifiedId, soul::lexer::LexerBase<char>& lexer, int64_t pos);
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    void Resolve(Context* context) override;
    Type* ResultType() const { return resultType; }
    void SetResultType(Type* resultType_) { resultType = resultType_; }
private:
    Type* resultType;
    util::uuid resultTypeId;
};

enum class FunctionKind
{
    none, regularfn, standardfn, operatorfn, conversionfn
};

class Function : public Subroutine
{
public:
    Function();
    Function(FunctionKind kind_, const std::string& fullName_);
    Function(FunctionKind kind_, FunctionHeading* heading_);
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    std::string InfoName() const override;
    FunctionKind FnKind() const { return fnkind; }
    const FunctionHeading* GetFunctionHeading() const { return static_cast<const FunctionHeading*>(Heading()); }
    FunctionHeading* GetFunctionHeading() { return static_cast<FunctionHeading*>(Heading()); }
    virtual Type* ResultType(const std::vector<Type*>& parameterTypes) const;
    virtual Value* Evaluate(const std::vector<std::unique_ptr<Value>>& argumentValues, soul::lexer::LexerBase<char>& lexer, int64_t pos);
    virtual void GenerateCode(Emitter* emitter, int64_t pos);
    bool IsRegularFunction() const { return fnkind == FunctionKind::regularfn; }
    bool IsStandardFunction() const { return fnkind == FunctionKind::standardfn; }
    void ResolveDeclaration(ParsingContext* context, soul::lexer::LexerBase<char>& lexer, int64_t pos) override;
private:
    FunctionKind fnkind;
};

class ConstructorHeading : public SubroutineHeading
{
public:
    ConstructorHeading();
    void SetObjectName(ParsingContext* context, const std::string& objectName_, soul::lexer::LexerBase<char>& lexer, int64_t pos);
    const std::string& ObjectName() const { return objectName; }
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
private:
    std::string objectName;
};

enum class ConstructorCallKind
{
    baseCall = 0, thisCall = 1
};

class ConstructorCall
{
public:
    ConstructorCall(ConstructorCallKind kind_);
    ConstructorCallKind Kind() const { return kind; }
    void AddArgument(Node* argument);
    const std::vector<std::unique_ptr<Node>>& Arguments() const { return arguments; }
    bool IsBaseCall() const { return kind == ConstructorCallKind::baseCall; }
    bool IsThisCall() const { return kind == ConstructorCallKind::thisCall; }
private:
    ConstructorCallKind kind;
    std::vector<std::unique_ptr<Node>> arguments;
};

class Constructor : public Subroutine
{
public:
    Constructor();
    Constructor(ConstructorHeading* heading_);
    ConstructorHeading* GetConstructorHeading() const { return static_cast<ConstructorHeading*>(Heading()); }
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    void ResolveDeclaration(ParsingContext* context, soul::lexer::LexerBase<char>& lexer, int64_t pos) override;
    std::string InfoName() const override;
    void SetImplementation(Constructor* implementation_) { implementation = implementation_; }
    Constructor* GetImplementation() const { return implementation; }
    ConstructorCall* GetConstructorCall() const { return constructorCall.get(); }
    void SetConstructorCall(ConstructorCall * constructorCall_);
private:
    Constructor* implementation;
    std::unique_ptr<ConstructorCall> constructorCall;
};

Procedure* MakeProcedure(ParsingContext* context, const std::string& fullName, soul::lexer::LexerBase<char>& lexer, int64_t pos);
Procedure* MakeProcedure(ParsingContext* context, ProcedureHeading* heading, soul::lexer::LexerBase<char>& lexer, int64_t pos);
Function* MakeFunction(ParsingContext* context, const std::string& fullName, soul::lexer::LexerBase<char>& lexer, int64_t pos);
Function* MakeFunction(ParsingContext* context, FunctionHeading* heading, soul::lexer::LexerBase<char>& lexer, int64_t pos);
Constructor* MakeConstructor(ParsingContext* context, soul::lexer::LexerBase<char>& lexer, int64_t pos);
Constructor* MakeConstructor(ParsingContext* context, ConstructorHeading* heading, soul::lexer::LexerBase<char>& lexer, int64_t pos);

std::string MakeFullName(QualifiedIdNode* qualifiedIdNode);

void AddParameters(SubroutineHeading* heading, ParameterQualifier qualifier, const std::vector<std::string>& identifiers, Type* type);

} // namespace p
