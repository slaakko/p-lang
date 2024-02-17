// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.core.rt;

import p.ast;
import p.core.subroutine_symbol;
import util;
import std.core;

export namespace p {

class ExternalSubroutineSymbol : public SubroutineSymbol
{
public:
    ExternalSubroutineSymbol(const soul::ast::Span& span_, const std::string& name_, const util::uuid& id_);
};

ExternalSubroutineSymbol* GetExternalSubroutine(const std::string& externalSubroutineName, Node* node);
ExternalSubroutineSymbol* GetExternalSubroutineNoThrow(const std::string& externalSubroutineName);
ExternalSubroutineSymbol* GetExternalSubroutine(const util::uuid& externalSubroutineId);

void Init(const std::string& filePath, const soul::ast::Span& span);
void Done();

} // namespace p
