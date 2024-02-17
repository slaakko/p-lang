// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.core.program_symbol;

import util;

namespace p {

std::string GetProgramPCodeFilePath(const std::string& programUnitName)
{
    std::string plangPRoot = util::GetFullPath(util::PLangRoot());
    return util::Path::Combine(util::Path::Combine(plangPRoot, "prog"), programUnitName + ".pcode");
}

ProgramSymbol::ProgramSymbol(const soul::ast::Span& span_, const std::string& name_) : RootSymbol(SymbolKind::programSymbol, span_, name_)
{
}

ProgramSymbol::ProgramSymbol(const soul::ast::Span& span_, const std::string& name_, const std::string& sourceFilePath_) :
    RootSymbol(SymbolKind::programSymbol, span_, name_, sourceFilePath_, GetProgramPCodeFilePath(name_))
{
}

} // namespace p
