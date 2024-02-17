// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.core.writer;

import soul.ast.span;
import p.ast;
import std.core;

export namespace p {

class Symbol;
class Value;

class SymbolWriter : public AstWriter
{
public:
    SymbolWriter(const std::string& filePath_);
    void WriteHeader();
    void WriteSymbol(Symbol* symbol);
    void WriteValue(Value* value);
};

} // namespace p
