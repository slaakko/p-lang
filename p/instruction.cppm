// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.instruction;

import util;
import std.core;

export namespace p {

class ExecutionContext;
class Value;
class Writer;
class Reader;
class Subroutine;
class Procedure;
class Function;
class Constructor;

enum class InstructionKind
{
    nop, load_local, store_local, load_parent, store_parent, load_global, store_global,
    load_result_var, push_value, pop_value, load_field, store_field, load_element, store_element, array_length, string_length, receive, jump, branch, case_inst,
    call_procedure, call_function, call_stdfn, call_stdproc, call_virtual, call_external, call_ctor, new_object, new_array,
    equal_bool, not_equal_bool, and_bool, or_bool, xor_bool, not_bool,
    equal_int, not_equal_int, less_int, greater_int, less_or_equal_int, greater_or_equal_int,
    plus_int, minus_int, mul_int, fractional_divide_int, div_int, mod_int, and_int, or_int, xor_int, shl_int, shr_int, not_int, unary_plus_int, unary_minus_int,
    equal_real, not_equal_real, less_real, greater_real, less_or_equal_real, greater_or_equal_real,
    plus_real, minus_real, mul_real, fractional_divide_real, unary_plus_real, unary_minus_real,
    equal_char, not_equal_char, equal_string, not_equal_string, plus_string,
    int_to_real, char_to_string,
    equal_nil
};

std::string InstructionKindStr(InstructionKind instructionKind);

class Instruction : public util::Component
{
public:
    Instruction(InstructionKind kind_);
    InstructionKind Kind() const { return kind; }
    virtual void Write(Writer& writer);
    virtual void Read(Reader& reader);
    virtual void Resolve(Reader& reader);
    virtual std::string ToString(ExecutionContext* context) const;
    virtual Instruction* Execute(ExecutionContext* context) = 0;
    int32_t InstIndex() const { return index; }
    void SetInstIndex(int32_t index_) { index = index_; }
    Instruction* Next() { return static_cast<Instruction*>(NextSibling()); }
private:
    InstructionKind kind;
    int32_t index;
};

class NoOperationInstruction : public Instruction
{
public:
    NoOperationInstruction();
    Instruction* Execute(ExecutionContext* context) override;
};

class LoadLocalInstruction : public Instruction
{
public:
    LoadLocalInstruction();
    void SetLocalIndex(int32_t localIndex_);
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    std::string ToString(ExecutionContext* context) const override;
    Instruction* Execute(ExecutionContext* context) override;
private:
    int32_t localIndex;
};

class StoreLocalInstruction : public Instruction
{
public:
    StoreLocalInstruction();
    void SetLocalIndex(int32_t localIndex_);
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    std::string ToString(ExecutionContext* context) const override;
    Instruction* Execute(ExecutionContext* context) override;
private:
    int32_t localIndex;
};

class LoadParentInstruction : public Instruction
{
public:
    LoadParentInstruction();
    void SetParentLevel(int32_t parentLevel_);
    void SetVariableIndex(int32_t variableIndex_);
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    std::string ToString(ExecutionContext* context) const override;
    Instruction* Execute(ExecutionContext* context) override;
private:
    int32_t parentLevel;
    int32_t variableIndex;
};

class StoreParentInstruction : public Instruction
{
public:
    StoreParentInstruction();
    void SetParentLevel(int32_t parentLevel_);
    void SetVariableIndex(int32_t variableIndex_);
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    std::string ToString(ExecutionContext* context) const override;
    Instruction* Execute(ExecutionContext* context) override;
private:
    int32_t parentLevel;
    int32_t variableIndex;
};


class LoadGlobalInstruction : public Instruction
{
public:
    LoadGlobalInstruction();
    void SetGlobalIndex(int32_t globalIndex_);
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    std::string ToString(ExecutionContext* context) const override;
    Instruction* Execute(ExecutionContext* context) override;
private:
    int32_t globalIndex;
};

class StoreGlobalInstruction : public Instruction
{
public:
    StoreGlobalInstruction();
    void SetGlobalIndex(int32_t globalIndex_);
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    std::string ToString(ExecutionContext* context) const override;
    Instruction* Execute(ExecutionContext* context) override;
private:
    int32_t globalIndex;
};

class LoadResultVarInstruction : public Instruction
{
public:
    LoadResultVarInstruction();
    void SetResultVarIndex(int32_t resultVarIndex_);
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    std::string ToString(ExecutionContext* context) const override;
    Instruction* Execute(ExecutionContext* context) override;
private:
    int32_t resultVarIndex;
};

class PushValueInstruction : public Instruction
{
public:
    PushValueInstruction();
    void SetValue(Value* value_);
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    std::string ToString(ExecutionContext* context) const override;
    Instruction* Execute(ExecutionContext* context) override;
private:
    std::unique_ptr<Value> value;
};

class PopValueInstruction : public Instruction
{
public:
    PopValueInstruction();
    Instruction* Execute(ExecutionContext* context) override;
};

class LoadFieldInstruction : public Instruction
{
public:
    LoadFieldInstruction();
    void SetFieldIndex(int32_t fieldIndex_);
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    std::string ToString(ExecutionContext* context) const override;
    Instruction* Execute(ExecutionContext* context) override;
private:
    int32_t fieldIndex;
};

class StoreFieldInstruction : public Instruction
{
public:
    StoreFieldInstruction();
    void SetFieldIndex(int32_t fieldIndex_);
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    std::string ToString(ExecutionContext* context) const override;
    Instruction* Execute(ExecutionContext* context) override;
private:
    int32_t fieldIndex;
};

class LoadElementInstruction : public Instruction
{
public:
    LoadElementInstruction();
    Instruction* Execute(ExecutionContext* context) override;
};

class StoreElementInstruction : public Instruction
{
public:
    StoreElementInstruction();
    Instruction* Execute(ExecutionContext* context) override;
};

class ArrayLengthInstruction : public Instruction
{
public:
    ArrayLengthInstruction();
    Instruction* Execute(ExecutionContext* context) override;
};

class StringLengthInstruction : public Instruction
{
public:
    StringLengthInstruction();
    Instruction* Execute(ExecutionContext* context) override;
};

class ReceiveInstruction : public Instruction
{
public:
    ReceiveInstruction();
    void SetCount(int32_t count_);
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    std::string ToString(ExecutionContext* context) const override;
    Instruction* Execute(ExecutionContext* context) override;
private:
    int32_t count;
};

class JumpInstruction : public Instruction
{
public:
    JumpInstruction();
    void SetTarget(Instruction* target_);
    int32_t TargetIndex() const { return targetIndex; }
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    void Resolve(Reader& reader) override;
    std::string ToString(ExecutionContext* context) const override;
    Instruction* Execute(ExecutionContext* context) override;
private:
    int32_t targetIndex;
    Instruction* target;
};

class BranchInstruction : public Instruction
{
public:
    BranchInstruction();
    void SetTrueNext(Instruction* trueNext_);
    void SetFalseNext(Instruction* falseNext_);
    int32_t TrueNextIndex() const { return trueNextIndex; }
    int32_t FalseNextIndex() const { return falseNextIndex; }
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    void Resolve(Reader& reader) override;
    std::string ToString(ExecutionContext* context) const override;
    Instruction* Execute(ExecutionContext* context) override;
private:
    int32_t trueNextIndex;
    int32_t falseNextIndex;
    Instruction* trueNext;
    Instruction* falseNext;
};

class Range
{
public:
    Range();
    Range(int32_t first_, int32_t last_);
    bool Match(int32_t value) const { return value >= first && value <= last; }
    void Write(Writer& writer);
    void Read(Reader& reader);
private:
    int32_t first;
    int32_t last;
};

class RangeList
{
public:
    RangeList();
    void AddRange(const Range& range);
    bool Match(int32_t value) const;
    Instruction* Next() const { return next; }
    void SetNext(Instruction* next_);
    void Write(Writer& writer);
    void Read(Reader& reader);
    void Resolve(Reader& reader);
private:
    std::vector<Range> ranges;
    Instruction* next;
    int32_t nextIndex;
};

class CaseInstruction : public Instruction
{
public:
    CaseInstruction();
    RangeList& AddRangeList();
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    void Resolve(Reader& reader) override;
    std::string ToString(ExecutionContext* context) const override;
    Instruction* Execute(ExecutionContext* context) override;
    void SetElseInst(Instruction* elseInst_);
private:
    std::vector<RangeList> rangeLists;
    Instruction* elseInst;
    int32_t elseInstIndex;
};

class CallProcedureInstruction : public Instruction
{
public:
    CallProcedureInstruction();
    void SetProcedure(Procedure* procedure_);
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    std::string ToString(ExecutionContext* context) const override;
    Instruction* Execute(ExecutionContext* context) override;
private:
    Procedure* procedure;
    int32_t moduleId;
    int32_t subroutineId;
};

class CallStdProcInstruction : public Instruction
{
public:
    CallStdProcInstruction();
    void SetProcedure(Procedure* procedure_);
    void SetArgumentCount(int32_t argumentCount_);
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    std::string ToString(ExecutionContext* context) const override;
    Instruction* Execute(ExecutionContext* context) override;
private:
    Procedure* procedure;
    int32_t stdprocId;
    int32_t argumentCount;
};

class CallFunctionInstruction : public Instruction
{
public:
    CallFunctionInstruction();
    void SetFunction(Function* function_);
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    std::string ToString(ExecutionContext* context) const override;
    Instruction* Execute(ExecutionContext* context) override;
private:
    Function* function;
    int32_t moduleId;
    int32_t subroutineId;
};

class CallStdFnInstruction : public Instruction
{
public:
    CallStdFnInstruction();
    void SetStdFnId(int32_t stdfnId_);
    void SetArgumentCount(int32_t argumentCount_);
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    std::string ToString(ExecutionContext* context) const override;
    Instruction* Execute(ExecutionContext* context) override;
private:
    int32_t stdfnId;
    int32_t argumentCount;
};

class CallVirtualInstruction : public Instruction
{
public:
    CallVirtualInstruction();
    void SetMethod(Subroutine* method_);
    Subroutine* GetMethod() const { return method; }
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    std::string ToString(ExecutionContext* context) const override;
    Instruction* Execute(ExecutionContext* context) override;
private:
    Subroutine* method;
    int32_t moduleId;
    int32_t methodId;
    int32_t methodIndex;
};

class CallExternalInstruction : public Instruction
{
public:
    CallExternalInstruction();
    void SetSubroutine(Subroutine* subroutine_);
    int32_t Id() const { return id; }
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    std::string ToString(ExecutionContext* context) const override;
    Instruction* Execute(ExecutionContext* context) override;
private:
    Subroutine* subroutine;
    int32_t id;
};

class NewObjectInstruction : public Instruction
{
public:
    NewObjectInstruction();
    void SetObjectTypeId(const util::uuid& objectTypeId_);
    const util::uuid& ObjectTypeId() const { return objectTypeId; }
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    std::string ToString(ExecutionContext* context) const override;
    Instruction* Execute(ExecutionContext* context) override;
private:
    util::uuid objectTypeId;
};

class NewArrayInstruction : public Instruction
{
public:
    NewArrayInstruction();
    void SetArrayTypeId(const util::uuid& arrayTypeId_);
    const util::uuid& ArrayTypeId() const { return arrayTypeId; }
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    std::string ToString(ExecutionContext* context) const override;
    Instruction* Execute(ExecutionContext* context) override;
private:
    util::uuid arrayTypeId;
};

class CallConstructorInstruction : public Instruction
{
public:
    CallConstructorInstruction();
    void SetConstructor(Constructor* constructor_);
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    std::string ToString(ExecutionContext* context) const override;
    Instruction* Execute(ExecutionContext* context) override;
private:
    Constructor* constructor;
    int32_t moduleId;
    int32_t subroutineId;
};

class EqualBoolInstruction : public Instruction
{
public:
    EqualBoolInstruction();
    Instruction* Execute(ExecutionContext* context) override;
};

class NotEqualBoolInstruction : public Instruction
{
public:
    NotEqualBoolInstruction();
    Instruction* Execute(ExecutionContext* context) override;
};

class AndBoolInstruction : public Instruction
{
public:
    AndBoolInstruction();
    Instruction* Execute(ExecutionContext* context) override;
};

class OrBoolInstruction : public Instruction
{
public:
    OrBoolInstruction();
    Instruction* Execute(ExecutionContext* context) override;
};

class XorBoolInstruction : public Instruction
{
public:
    XorBoolInstruction();
    Instruction* Execute(ExecutionContext* context) override;
};

class NotBoolInstruction : public Instruction
{
public:
    NotBoolInstruction();
    Instruction* Execute(ExecutionContext* context) override;
};

class EqualIntInstruction : public Instruction
{
public:
    EqualIntInstruction();
    Instruction* Execute(ExecutionContext* context) override;
};

class NotEqualIntInstruction : public Instruction
{
public:
    NotEqualIntInstruction();
    Instruction* Execute(ExecutionContext* context) override;
};

class LessIntInstruction : public Instruction
{
public:
    LessIntInstruction();
    Instruction* Execute(ExecutionContext* context) override;
};

class GreaterIntInstruction : public Instruction
{
public:
    GreaterIntInstruction();
    Instruction* Execute(ExecutionContext* context) override;
};

class LessOrEqualIntInstruction : public Instruction
{
public:
    LessOrEqualIntInstruction();
    Instruction* Execute(ExecutionContext* context) override;
};

class GreaterOrEqualIntInstruction : public Instruction
{
public:
    GreaterOrEqualIntInstruction();
    Instruction* Execute(ExecutionContext* context) override;
};

class PlusIntInstruction : public Instruction
{
public:
    PlusIntInstruction();
    Instruction* Execute(ExecutionContext* context) override;
};

class MinusIntInstruction : public Instruction
{
public:
    MinusIntInstruction();
    Instruction* Execute(ExecutionContext* context) override;
};

class MultiplyIntInstruction : public Instruction
{
public:
    MultiplyIntInstruction();
    Instruction* Execute(ExecutionContext* context) override;
};

class FractionalDivideIntInstruction : public Instruction
{
public:
    FractionalDivideIntInstruction();
    Instruction* Execute(ExecutionContext* context) override;
};

class DivIntInstruction : public Instruction
{
public:
    DivIntInstruction();
    Instruction* Execute(ExecutionContext* context) override;
};

class ModIntInstruction : public Instruction
{
public:
    ModIntInstruction();
    Instruction* Execute(ExecutionContext* context) override;
};

class AndIntInstruction : public Instruction
{
public:
    AndIntInstruction();
    Instruction* Execute(ExecutionContext* context) override;
};

class OrIntInstruction : public Instruction
{
public:
    OrIntInstruction();
    Instruction* Execute(ExecutionContext* context) override;
};

class XorIntInstruction : public Instruction
{
public:
    XorIntInstruction();
    Instruction* Execute(ExecutionContext* context) override;
};

class ShlIntInstruction : public Instruction
{
public:
    ShlIntInstruction();
    Instruction* Execute(ExecutionContext* context) override;
};

class ShrIntInstruction : public Instruction
{
public:
    ShrIntInstruction();
    Instruction* Execute(ExecutionContext* context) override;
};

class NotIntInstruction : public Instruction
{
public:
    NotIntInstruction();
    Instruction* Execute(ExecutionContext* context) override;
};

class UnaryPlusIntInstruction : public Instruction
{
public:
    UnaryPlusIntInstruction();
    Instruction* Execute(ExecutionContext* context) override;
};

class UnaryMinusIntInstruction : public Instruction
{
public:
    UnaryMinusIntInstruction();
    Instruction* Execute(ExecutionContext* context) override;
};

class EqualRealInstruction : public Instruction
{
public:
    EqualRealInstruction();
    Instruction* Execute(ExecutionContext* context) override;
};

class NotEqualRealInstruction : public Instruction
{
public:
    NotEqualRealInstruction();
    Instruction* Execute(ExecutionContext* context) override;
};

class LessRealInstruction : public Instruction
{
public:
    LessRealInstruction();
    Instruction* Execute(ExecutionContext* context) override;
};

class GreaterRealInstruction : public Instruction
{
public:
    GreaterRealInstruction();
    Instruction* Execute(ExecutionContext* context) override;
};

class LessOrEqualRealInstruction : public Instruction
{
public:
    LessOrEqualRealInstruction();
    Instruction* Execute(ExecutionContext* context) override;
};

class GreaterOrEqualRealInstruction : public Instruction
{
public:
    GreaterOrEqualRealInstruction();
    Instruction* Execute(ExecutionContext* context) override;
};

class PlusRealInstruction : public Instruction
{
public:
    PlusRealInstruction();
    Instruction* Execute(ExecutionContext* context) override;
};

class MinusRealInstruction : public Instruction
{
public:
    MinusRealInstruction();
    Instruction* Execute(ExecutionContext* context) override;
};

class MultiplyRealInstruction : public Instruction
{
public:
    MultiplyRealInstruction();
    Instruction* Execute(ExecutionContext* context) override;
};

class FractionalDivideRealInstruction : public Instruction
{
public:
    FractionalDivideRealInstruction();
    Instruction* Execute(ExecutionContext* context) override;
};

class UnaryPlusRealInstruction : public Instruction
{
public:
    UnaryPlusRealInstruction();
    Instruction* Execute(ExecutionContext* context) override;
};

class UnaryMinusRealInstruction : public Instruction
{
public:
    UnaryMinusRealInstruction();
    Instruction* Execute(ExecutionContext* context) override;
};

class EqualCharInstruction : public Instruction
{
public:
    EqualCharInstruction();
    Instruction* Execute(ExecutionContext* context) override;
};

class NotEqualCharInstruction : public Instruction
{
public:
    NotEqualCharInstruction();
    Instruction* Execute(ExecutionContext* context) override;
};

class EqualStringInstruction : public Instruction
{
public:
    EqualStringInstruction();
    Instruction* Execute(ExecutionContext* context) override;
};

class NotEqualStringInstruction : public Instruction
{
public:
    NotEqualStringInstruction();
    Instruction* Execute(ExecutionContext* context) override;
};

class PlusStringInstruction : public Instruction
{
public:
    PlusStringInstruction();
    Instruction* Execute(ExecutionContext* context) override;
};

class IntToRealInstruction : public Instruction
{
public:
    IntToRealInstruction();
    Instruction* Execute(ExecutionContext* context) override;
};

class CharToStringInstruction : public Instruction
{
public:
    CharToStringInstruction();
    Instruction* Execute(ExecutionContext* context) override;
};

class EqualNilInstruction : public Instruction
{
public:
    EqualNilInstruction();
    Instruction* Execute(ExecutionContext* context) override;
};

Instruction* MakeInstruction(InstructionKind kind);

} // namespace p
