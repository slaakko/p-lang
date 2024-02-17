// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.core.value;

import p.ast;
import p.core.object;
import soul.ast.span;
import std.core;

export namespace p {

class SymbolReader;
class SymbolWriter;
class FunctionSymbol;
class TypeSymbol;

enum class ValueKind
{
    none, ptrValue, booleanValue, integerValue, charValue, enumerationValue, realValue, stringValue, constantValue, arrayValue, objectValue, functionValue,
    genericPointerValue, nilValue
};

class Value : public Object
{
public:
    Value(ValueKind kind_);
    ValueKind Kind() const { return kind; }
    virtual Value* GetValue() { return this; }
    virtual void Write(SymbolWriter& writer);
    virtual void Read(SymbolReader& reader);
    virtual std::string ToValueString() const { return std::string(); }
    virtual Value* Succ() const;
    virtual Value* Pred() const;
    virtual Value* Succ(Node* node) const;
    virtual Value* Pred(Node* node) const;
    TypeSymbol* Type() const { return type; }
    void SetType(TypeSymbol* type_) { type = type_; }
    bool IsIntegerValue() const { return kind == ValueKind::integerValue; }
    bool IsBooleanValue() const { return kind == ValueKind::booleanValue; }
    bool IsCharValue() const { return kind == ValueKind::charValue; }
    bool IsEnumerationValue() const { return kind == ValueKind::enumerationValue; }
    bool IsRealValue() const { return kind == ValueKind::realValue; }
    bool IsStringValue() const { return kind == ValueKind::stringValue; }
    bool IsConstantValue() const { return kind == ValueKind::constantValue; }
    bool IsArrayValue() const { return kind == ValueKind::arrayValue; }
    bool IsObjectValue() const { return kind == ValueKind::objectValue; }
    bool IsFunctionValue() const { return kind == ValueKind::functionValue; }
    bool IsGenericPointerValue() const { return kind == ValueKind::genericPointerValue; }
    bool IsNilValue() const { return kind == ValueKind::nilValue; }
    bool IsNilObject() const override { return kind == ValueKind::nilValue; }
private:
    ValueKind kind;
    soul::ast::Span span;
    TypeSymbol* type;
};

class PtrValue : public Value
{
public:
    PtrValue();
    PtrValue(Value* value_);
    Value* GetValue() override { return value->GetValue(); }
    Object* Clone() const override;
private:
    Value* value;
};

class BooleanValue : public Value
{
public:
    BooleanValue();
    BooleanValue(bool value_);
    bool GetBooleanValue() const { return value; }
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    Object* Clone() const override;
    bool ToBoolean() const override { return value; }
    int32_t ToInteger() const override { return value ? 1 : 0; }
    float ToReal() const override { return value ? 1.0f : 0.0f; }
    std::string ToValueString() const override { return value ? "true" : "false"; }
    Value* Succ() const override;
    Value* Pred() const override;
    Value* Succ(Node* node) const override;
    Value* Pred(Node* node) const override;
private:
    bool value;
};

class IntegerValue : public Value
{
public:
    IntegerValue();
    IntegerValue(int32_t value_);
    int32_t GetIntegerValue() const { return value; }
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    Object* Clone() const override;
    bool ToBoolean() const override { return value != 0; }
    int32_t ToInteger() const override { return value; }
    float ToReal() const override { return static_cast<float>(value); }
    char ToChar() const override { return static_cast<char>(value); }
    Value* Succ() const override;
    Value* Pred() const override;
    Value* Succ(Node* node) const override;
    Value* Pred(Node* node) const override;
    std::string ToValueString() const override { return std::to_string(value); }
private:
    int32_t value;
};

class CharValue : public Value
{
public:
    CharValue();
    CharValue(unsigned char value_);
    unsigned char GetCharValue() const { return value; }
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    Object* Clone() const override;
    char ToChar() const override { return value; }
    int32_t ToInteger() const override { return static_cast<int32_t>(value); }
    std::string ToString() const override { return std::string(1, value); }
    Value* Succ() const override;
    Value* Pred() const override;
    Value* Succ(Node* node) const override;
    Value* Pred(Node* node) const override;
    std::string ToValueString() const override { return "'" + std::string(1, value) + "'"; }
private:
    unsigned char value;
};

class EnumerationValue : public Value
{
public:
    EnumerationValue();
    EnumerationValue(int32_t value_);
    int32_t GetIntegerValue() const { return value; }
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    Object* Clone() const override;
    int32_t ToInteger() const override { return value; }
    Value* Succ() const override;
    Value* Pred() const override;
    Value* Succ(Node* node) const override;
    Value* Pred(Node* node) const override;
    std::string ToValueString() const override { return std::to_string(value); }
private:
    int32_t value;
};

class RealValue : public Value
{
public:
    RealValue();
    RealValue(float value_);
    float GetRealValue() const { return value; }
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    Object* Clone() const override;
    bool ToBoolean() const override { return value != 0.0f; }
    float ToReal() const override { return value; }
    int32_t ToInteger() const override { return static_cast<int32_t>(value); }
    std::string ToValueString() const override { return std::to_string(value); }
private:
    float value;
};

class StringValue : public Value
{
public:
    StringValue();
    StringValue(const std::string& value_);
    const std::string& GetStringValue() const { return value; }
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    Object* Clone() const override;
    std::string ToString() const override { return value; }
    std::string ToValueString() const override { return "'" + value + "'"; }
    StringObject* ToStringObject(ExecutionContext* context) override;
private:
    std::string value;
};

class ConstantValue : public Value
{
public:
    ConstantValue(Value* value_);
    Value* GetValue() override { return value->GetValue(); }
    Object* Clone() const override;
    std::string ToValueString() const override { return value->ToValueString(); }
private:
    Value* value;
};

class ArrayValue : public Value
{
public:
    ArrayValue();
    const std::vector<std::unique_ptr<Value>>& Elements() const { return elements; }
    int32_t Length() const { return elements.size(); }
    void AddElement(Value* element);
    Value* GetElement(int32_t elementIndex) const;
    Value* GetElement(int32_t elementIndex, Node* node) const;
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    Object* Clone() const override;
    std::string ToValueString() const override { return "<array>"; }
private:
    std::vector<std::unique_ptr<Value>> elements;
};

class Field
{
public:
    Field();
    Field(int32_t index_, const std::string& name_, Value* value_);
    int32_t Index() const { return index; }
    const std::string& Name() const { return name; }
    Value* GetValue() const { return value.get(); }
    void Write(SymbolWriter& writer);
    void Read(SymbolReader& reader);
private:
    int32_t index;
    std::string name;
    std::unique_ptr<Value> value;
};

class ObjectValue : public Value
{
public:
    ObjectValue();
    void AddField(Field&& field);
    const std::vector<Field>& Fields() const { return fields; }
    const Field& GetField(int32_t index) const;
    const Field& GetField(int32_t index, Node* node) const;
    const Field& GetField(const std::string& fieldName, Node* node) const;
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    Object* Clone() const override;
    std::string ToValueString() const override { return "<object>"; }
private:
    std::vector<Field> fields;
};

class FunctionValue : public Value
{
public:
    FunctionValue();
    FunctionValue(FunctionSymbol* function_);
    FunctionSymbol* GetFunction() const { return function; }
    Object* Clone() const override;
    std::string ToValueString() const override { return "<function>"; }
private:
    FunctionSymbol* function;
};

class GenericPointerValue : public Value
{
public:
    GenericPointerValue();
    GenericPointerValue(void* pointer_);
    void* Pointer() const { return pointer; }
    Object* Clone() const override;
    std::string ToValueString() const override { return "<generic_pointer>"; }
private:
    void* pointer;
};

class NilValue : public Value
{
public:
    NilValue();
    Object* Clone() const override;
    std::string ToValueString() const override { return "<nil>"; }
};

ValueKind CommonType(ValueKind left, ValueKind right);
Value* MakeValue(ValueKind kind);

const int32_t valueSize = sizeof(IntegerValue);

} // namespace p
