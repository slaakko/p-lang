// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.core.vmt;

import util;
import std.core;

export namespace p {

class SubroutineSymbol;
class ObjectTypeSymbol;
class SymbolWriter;
class SymbolReader;
class Context;
class SymbolTable;

class Vmt
{
public:
    Vmt();
    int32_t Size() const { return methods.size(); }
    void AddMethod(SubroutineSymbol* method);
    void SetMethod(SubroutineSymbol* method);
    SubroutineSymbol* GetMethod(int32_t methodIndex) const;
    SubroutineSymbol* GetMethod(const std::string& commonName) const;
    void Write(SymbolWriter& writer);
    void Read(SymbolReader& reader);
    void Resolve(SymbolTable* symbolTable);
private:
    std::vector<SubroutineSymbol*> methods;
    std::vector<util::uuid> methodIds;
};

void MakeVmt(Vmt& vmt, ObjectTypeSymbol* objectType);

} // namespace p
