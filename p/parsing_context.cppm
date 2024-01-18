// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.parsing_context;

import p.context;
import p.mod;
import std.core;

export namespace p {

namespace parsing_context {}

class Value;
class Emitter;
class Constructor;

enum class Flags
{
    none, makeType = 1 << 0, makeConstant = 1 << 1, addToConstant = 1 << 2, initializationPart = 1 << 3
};

constexpr Flags operator|(Flags left, Flags right)
{
    return Flags(int(left) | int(right));
}

constexpr Flags operator&(Flags left, Flags right)
{
    return Flags(int(left) & int(right));
}

constexpr Flags operator&(Flags flags)
{
    return Flags(~int(flags));
}

class ParsingContext : public Context
{
public:
    ParsingContext(Context* parent);
    Module* GetModule() const { return mod.get(); }
    void SetModulePart(ModulePart* modulePart_);
    ModulePart* GetModulePart() const { return modulePart; }
    const std::string& TypeName() const { return typeName; }
    void SetTypeName(const std::string& typeName_);
    const std::string& ConstantName() const { return constantName; }
    void SetConstantName(const std::string& constantName_);
    const std::string& FieldName() const { return fieldName; }
    void SetFieldName(const std::string& fieldName_);
    void PushFlags(Flags flags_);
    void PopFlags();
    Flags GetFlags() const { return flags; }
    bool MakeType() const { return (flags & Flags::makeType) != Flags::none; }
    bool MakeConstant() const { return (flags & Flags::makeConstant) != Flags::none; }
    bool AddToConstant() const { return (flags & Flags::addToConstant) != Flags::none; }
    void PushCurrentValue(Value* currentValue_);
    void PopCurrentValue();
    Value* GetCurrentValue() const { return currentValue; }
    Emitter* GetEmitter() { return emitter; }
    void SetEmitter(Emitter* emitter_) { emitter = emitter_; }
    void PushConstructor(Constructor* constructor_);
    void PopConstructor();
    Constructor* GetConstructor() const { return constructor; }
    void PushNode(Node* node_);
    void PopNode();
    Node* GetNode() const { return node; }
private:
    std::unique_ptr<Module> mod;
    ModulePart* modulePart;
    std::string typeName;
    std::string constantName;
    std::string fieldName;
    Value* currentValue;
    std::stack<Value*> valueStack;
    Flags flags;
    std::stack<Flags> flagStack;
    Emitter* emitter;
    Constructor* constructor;
    std::stack<Constructor*> constructorStack;
    Node* node;
    std::stack<Node*> nodeStack;
};

} // namespace p
