// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.core.unit_symbol;

import p.core.root_symbol;
import soul.ast.span;
import std.core;

export namespace p {

std::string GetUnitPCodeFilePath(const std::string& unitName);

class UnitSymbol : public RootSymbol
{
public:
    UnitSymbol(const soul::ast::Span& span_, const std::string& name_);
    UnitSymbol(const soul::ast::Span& span_, const std::string& name_, const std::string& sourceFilePath_);
};

} // namespace p
