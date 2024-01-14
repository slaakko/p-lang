// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.constant;

import util;
import p.value;
import std.core;

export namespace p {

namespace constant {}

class Type;
class Writer;
class Reader;
class Block;
class Context;

class Constant
{
public:
    Constant();
    Constant(const std::string& name_);
    virtual ~Constant();
    void Write(Writer& writer);
    void Read(Reader& reader);
    void Resolve(Context* context);
    const util::uuid& Id() const { return id; }
    const std::string& Name() const { return name; }
    Type* GetType() const { return type; }
    void SetType(Type* type_);
    void SetValue(Value* value_);
    Value* GetValue() const { return value.get(); }
    void Print(util::CodeFormatter& formatter);
private:
    util::uuid id;
    std::string name;
    Type* type;
    util::uuid typeId;
    std::unique_ptr<Value> value;
};

class GlobalConstantMap
{
public:
    GlobalConstantMap();
    void AddConstant(Constant* constant);
    Constant* GetConstant(const util::uuid& id) const;
private:
    std::map<util::uuid, Constant*> constantMap;
};

} // namespace p
