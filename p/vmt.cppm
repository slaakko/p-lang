// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.vmt;

import std.core;

export namespace p {

class Subroutine;
class ObjectType;
class Writer;
class Reader;
class Context;

class Vmt
{
public:
    Vmt();
    int32_t Size() const { return methods.size(); }
    void AddMethod(Subroutine* method);
    void SetMethod(Subroutine* method);
    Subroutine* GetMethod(int32_t methodIndex) const;
    Subroutine* GetMethod(const std::string& commonName) const;
    void Write(Writer& writer);
    void Read(Reader& reader);
    void Resolve(Context* context);
private:
    std::vector<Subroutine*> methods;
    std::vector<std::pair<int32_t, int32_t>> methodIds;
};

void MakeVmt(Vmt& vmt, ObjectType* objectType, Context* context);

} // namespace p
