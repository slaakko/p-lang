// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.core.unit_symbol;

import util;

namespace p {

std::string GetUnitPCodeFilePath(const std::string& unitName)
{
    std::string plangPRoot = util::GetFullPath(util::PLangRoot());
    return util::Path::Combine(util::Path::Combine(plangPRoot, "unit"), unitName + ".pcode");
}

UnitSymbol::UnitSymbol(const soul::ast::Span& span_, const std::string& name_) : RootSymbol(SymbolKind::unitSymbol, span_, name_) 
{
}

UnitSymbol::UnitSymbol(const soul::ast::Span& span_, const std::string& name_, const std::string& sourceFilePath_) :
    RootSymbol(SymbolKind::unitSymbol, span_, name_, sourceFilePath_, GetUnitPCodeFilePath(name_))
{
}

} // namespace p
