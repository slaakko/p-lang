// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.variable;

import p.object;
import util;
import std.core;

export namespace p {

class Type;
class Writer;
class Reader;
class Block;
class Context;
class Frame;

enum class VariableKind
{
    local, parameter, global
};

std::string VariableKindStr(VariableKind kind);

class Variable
{
public:
    Variable();
    Variable(const std::string& name_);
    VariableKind Kind() const { return kind; }
    void SetKind(VariableKind kind_) { kind = kind_; }
    bool IsLocal() const { return kind == VariableKind::local; }
    bool IsParameter() const { return kind == VariableKind::parameter; }
    bool IsGlobal() const { return kind == VariableKind::global; }
    virtual ~Variable();
    virtual bool IsConst() const { return false; }
    const std::string& Name() const { return name; }
    int32_t Index() const { return index; }
    void SetIndex(int32_t index_) { index = index_; }
    Type* GetType() const { return type; }
    void SetType(Type* type_);
    Block* GetBlock() const { return block; }
    void SetBlock(Block* block_) { block = block_; }
    void Write(Writer& writer);
    void Read(Reader& reader);
    void Resolve(Context* context);
    void Print(util::CodeFormatter& formatter);
private:
    VariableKind kind;
    std::string name;
    int32_t index;
    Type* type;
    util::uuid typeId;
    Block* block;
};

class GlobalVariableMap
{
public:
    GlobalVariableMap();
    void AddVariable(Variable* variable);
    Variable* GetVariable(int32_t index) const;
    void AllocateFrame();
    Object* GetObject(int32_t index) const;
    void SetObject(int32_t index, Object* object, ExecutionContext* context);
    Frame* GlobalFrame() const { return globalFrame.get(); }
private:
    std::map<int32_t, Variable*> variableMap;
    std::unique_ptr<Frame> globalFrame;
};

} // namespace p
