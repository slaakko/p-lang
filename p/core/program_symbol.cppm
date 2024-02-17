// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.core.program_symbol;

import p.core.root_symbol;
import soul.ast.span;
import std.core;

export namespace p {

std::string GetProgramPCodeFilePath(const std::string& programUnitName);

class ProgramSymbol : public RootSymbol
{
public:
    ProgramSymbol(const soul::ast::Span& span_, const std::string& name_);
    ProgramSymbol(const soul::ast::Span& span_, const std::string& name_, const std::string& sourceFilePath_);
};

} // namespace p
