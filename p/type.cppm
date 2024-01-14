// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.type;

import p.ast;
import p.value;
import p.subroutine;
import p.vmt;
import soul.lexer;
import util;
import std.core;

export namespace p {

namespace type {}

class Block;
class ParsingContext;
class ExecutionContext;
class Writer;
class Reader;

enum class TypeKind
{
    none, booleanType, integerType, charType, enumeratedType, subrangeType, realType, stringType, objectType, arrayType, pointerType, nilType
};

util::uuid BooleanTypeId();
util::uuid IntegerTypeId();
util::uuid CharTypeId();
util::uuid RealTypeId();
util::uuid StringTypeId();
util::uuid PointerTypeId();
util::uuid NilTypeId();

class BoundExpressionNode;

class Type
{
public:
    Type(TypeKind kind_);
    Type(TypeKind kind_, const std::string& name_);
    virtual ~Type();
    virtual void Write(Writer& writer);
    virtual void Read(Reader& reader);
    virtual void Resolve(Context* context);
    TypeKind Kind() const { return kind; }
    bool IsBooleanType() const { return kind == TypeKind::booleanType; }
    bool IsIntegerType() const { return kind == TypeKind::integerType; }
    bool IsCharType() const { return kind == TypeKind::charType; }
    bool IsEnumeratedType() const { return kind == TypeKind::enumeratedType; }
    bool IsSubrangeType() const { return kind == TypeKind::subrangeType; }
    bool IsRealType() const { return kind == TypeKind::realType; }
    bool IsStringType() const { return kind == TypeKind::stringType; }
    bool IsObjectType() const { return kind == TypeKind::objectType; }
    bool IsArrayType() const { return kind == TypeKind::arrayType; }
    bool IsPointerType() const { return kind == TypeKind::pointerType; }
    bool IsNilType() const { return kind == TypeKind::nilType; }
    const std::string& Name() const { return name; }
    virtual bool IsOrdinalType() const { return false; }
    const util::uuid& Id() const { return id; }
    void SetId(const util::uuid& id_);
    virtual void Print(util::CodeFormatter& formatter, ExecutionContext* context);
    virtual void CheckInterface();
    virtual void MakeVmt(Context* context);
private:
    TypeKind kind;
    std::string name;
    util::uuid id;
};

class TypePtr
{
public:
    TypePtr(Type* type_);
    Type* GetType() const { return type; }
private:
    Type* type;
};

class OrdinalType : public Type
{
public:
    OrdinalType(TypeKind kind_);
    OrdinalType(TypeKind kind_, const std::string& name_);
    bool IsOrdinalType() const override { return true; }
};

class BooleanType : public OrdinalType
{
public:
    BooleanType();
    static bool Not(bool operand) { return !operand; }
    static bool Equal(bool left, bool right) { return left == right; }
    static bool NotEqual(bool left, bool right) { return left != right; }
    static bool And(bool left, bool right) { return left && right; }
    static bool Or(bool left, bool right) { return left || right; }
    static bool Xor(bool left, bool right) { return left != right; }
};

class IntegerType : public OrdinalType
{
public:
    IntegerType();
    static bool Equal(int32_t left, int32_t right) { return left == right; }
    static bool NotEqual(int32_t left, int32_t right) { return left != right; }
    static bool Less(int32_t left, int32_t right) { return left < right; }
    static bool LessOrEqual(int32_t left, int32_t right) { return left <= right; }
    static bool Greater(int32_t left, int32_t right) { return left > right; }
    static bool GreaterOrEqual(int32_t left, int32_t right) { return left >= right; }
    static int32_t Not(int32_t operand) { return ~operand; }
    static int32_t UnaryPlus(int32_t operand) { return operand; }
    static int32_t UnaryMinus(int32_t operand) { return -operand; }
    static int32_t And(int32_t left, int32_t right) { return left & right; }
    static int32_t Or(int32_t left, int32_t right) { return left | right; }
    static int32_t Xor(int32_t left, int32_t right) { return left ^ right; }
    static int32_t Shl(int32_t left, int32_t right) { return left << right; }
    static int32_t Shr(int32_t left, int32_t right) { return left >> right; }
    static int32_t Plus(int32_t left, int32_t right) { return left + right; }
    static int32_t Minus(int32_t left, int32_t right) { return left - right; }
    static int32_t Multiply(int32_t left, int32_t right) { return left * right; }
    static float FractionalDivide(int32_t left, int32_t right) { return (1.0f * left) / right; }
    static int32_t Div(int32_t left, int32_t right) { return left / right; }
    static int32_t Mod(int32_t left, int32_t right) { return left % right; }
};

class CharType : public OrdinalType
{
public:
    CharType();
    static bool Equal(char left, char right) { return left == right; }
    static bool NotEqual(char left, char right) { return left != right; }
};

class EnumeratedType : public OrdinalType
{
public:
    EnumeratedType();
    EnumeratedType(const std::string& name_);
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    void Resolve(Context* context) override;
    void AddEnumerationConstant(Constant* enumConstant);
    const std::vector<Constant*>& EnumConstants() const { return enumConstants; }
    void Print(util::CodeFormatter& formatter, ExecutionContext* context) override;
private:
    std::vector<Constant*> enumConstants;
    std::vector<util::uuid> enumConstantIds;
};

class SubrangeType : public OrdinalType
{
public:
    SubrangeType();
    SubrangeType(const std::string& name_, Value* rangeStart_, Value* rangeEnd_);
    Value* RangeStart() const { return rangeStart.get(); }
    Value* RangeEnd() const { return rangeEnd.get(); }
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    void Print(util::CodeFormatter& formatter, ExecutionContext* context) override;
private:
    std::unique_ptr<Value> rangeStart;
    std::unique_ptr<Value> rangeEnd;
};

class RealType : public Type
{
public:
    RealType();
    static bool Equal(float left, float right) { return left == right; }
    static bool NotEqual(float left, float right) { return left != right; }
    static bool Less(float left, float right) { return left < right; }
    static bool LessOrEqual(float left, float right) { return left <= right; }
    static bool Greater(float left, float right) { return left > right; }
    static bool GreaterOrEqual(float left, float right) { return left >= right; }
    static float Plus(float left, float right) { return left + right; }
    static float Minus(float left, float right) { return left - right; }
    static float Multiply(float left, float right) { return left * right; }
    static float FractionalDivide(float left, float right) { return left / right; }
    static float UnaryPlus(float operand) { return operand; }
    static float UnaryMinus(float operand) { return -operand; }
};

class StringType : public Type
{
public:
    StringType();
    static bool Equal(const std::string& left, const std::string& right) { return left == right; }
    static bool NotEqual(const std::string& left, const std::string& right) { return left != right; }
    static bool Less(const std::string& left, const std::string& right) { return left < right; }
    static bool LessOrEqual(const std::string& left, const std::string& right) { return left <= right; }
    static bool Greater(const std::string& left, const std::string& right) { return left > right; }
    static bool GreaterOrEqual(const std::string& left, const std::string& right) { return left >= right; }
    static std::string Plus(const std::string& left, const std::string& right) { return left + right; }
};

class PointerType : public Type
{
public:
    PointerType();
};

class NilType : public Type
{
public:
    NilType();
};

enum class ObjectTypeFlags
{
    none = 0, isVirtual = 1 << 0, vmtComputed = 1 << 1, resolved = 1 << 2
};

constexpr ObjectTypeFlags operator|(ObjectTypeFlags left, ObjectTypeFlags right)
{
    return ObjectTypeFlags(int(left) | int(right));
}

constexpr ObjectTypeFlags operator&(ObjectTypeFlags left, ObjectTypeFlags right)
{
    return ObjectTypeFlags(int(left) & int(right));
}

constexpr ObjectTypeFlags operator~(ObjectTypeFlags flags)
{
    return ObjectTypeFlags(~int(flags));
}

class ObjectType : public Type
{
public:
    ObjectType();
    ObjectType(const std::string& name_);
    void SetBaseType(ObjectType* baseType_) { baseType = baseType_; }
    ObjectType* BaseType() const { return baseType; }
    void AddFields(ObjectType* to);
    void AddFields(const std::vector<std::string>& fieldNames, Type* type);
    int32_t GetFieldIndex(const std::string& fieldName) const;
    Type* GetFieldType(const std::string& fieldName, bool throw_) const;
    const std::vector<Type*>& GetFieldTypes() const { return fieldTypes; }
    int32_t FieldCount() const { return fieldTypes.size(); }
    Type* GetFieldType(int32_t fieldIndex) const;
    void FinalizeLayout();
    const std::vector<std::unique_ptr<Subroutine>>& Methods() const { return methods; }
    Subroutine* AddMethod(ParsingContext* context, SubroutineHeading* methodHeading, soul::lexer::LexerBase<char>& lexer, int64_t pos);
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    void Resolve(Context* context) override;
    Constructor* GetDefaultConstructor() const;
    Constructor* GetConstructor(const std::vector<Type*>& parameterTypes) const;
    Constructor* GetConstructor(std::vector<std::unique_ptr<BoundExpressionNode>>& arguments, soul::lexer::LexerBase<char>& lexer, int64_t pos) const;
    Subroutine* GetMethod(const std::string& methodName) const;
    void Print(util::CodeFormatter& formatter, ExecutionContext* context) override;
    int32_t VmtPtrFieldIndex() const { return vmtPtrFieldIndex; }
    bool IsVirtual() const { return (flags & ObjectTypeFlags::isVirtual) != ObjectTypeFlags::none; }
    void SetVirtual() { flags = flags | ObjectTypeFlags::isVirtual; }
    void CheckInterface() override;
    void MakeVmt(Context* context) override;
    Vmt* GetVmtPtr() const { return const_cast<Vmt*>(&vmt); }
    void GenerateDefaults(ParsingContext* context, soul::lexer::LexerBase<char>& lexer, int64_t pos);
    bool IsResolved() const { return (flags & ObjectTypeFlags::resolved) != ObjectTypeFlags::none; }
    void SetResolved() { flags = flags | ObjectTypeFlags::resolved; }
    bool IsSameOrHasBaseType(ObjectType* objectType) const;
private:
    ObjectTypeFlags flags;
    ObjectType* baseType;
    util::uuid baseTypeId;
    std::map<std::string, int32_t> fieldIndexMap;
    std::vector<Type*> fieldTypes;
    std::vector<util::uuid> fieldTypeIds;
    std::vector<std::unique_ptr<Subroutine>> methods;
    int32_t vmtPtrFieldIndex;
    Vmt vmt;
};

class ArrayType : public Type
{
public:
    ArrayType();
    ArrayType(const std::string& name_, Type* elementType_);
    Type* ElementType() const { return elementType; }
    void Write(Writer& writer) override;
    void Read(Reader& reader) override;
    void Resolve(Context* context) override;
    void Print(util::CodeFormatter& formatter, ExecutionContext* context) override;
private:
    Type* elementType;
    util::uuid elementTypeId;
};

Type* MakeSubrangeType(ParsingContext* context, Node* rangeStart, Node* rangeEnd, soul::lexer::LexerBase<char>& lexer, int64_t pos);

Type* MakeEnumeratedType(ParsingContext* context, const std::vector<std::string>& enumConstantIds, soul::lexer::LexerBase<char>& lexer, int64_t pos);

ObjectType* MakeObjectType(ParsingContext* context, soul::lexer::LexerBase<char>& lexer, int64_t pos);

void SetHeritage(ParsingContext* context, ObjectType* objectType, IdentifierNode* baseTypeId, soul::lexer::LexerBase<char>& lexer, int64_t pos);

ArrayType* MakeArrayType(ParsingContext* context, Type* elementType, soul::lexer::LexerBase<char>& lexer, int64_t pos);

class GlobalTypeMap
{
public:
    GlobalTypeMap();
    void AddType(Type* type);
    Type* GetType(const util::uuid& id) const;
    Type* GetType(const std::string& typeName) const;
private:
    std::map<util::uuid, Type*> typeMap;
    std::map<std::string, Type*> typeNameMap;
};

Type* MakeType(TypeKind kind);

} // namespace p
