
// this file has been automatically generated from 'C:/work/p-lang/p/parser/common.parser' using soul parser generator spg version 5.0.0

export module p.common.parser;

import std.core;
import soul.lexer;
import soul.parser;
import soul.ast.span;
import p.lexer;
import p.core.parsing_context;
import p.ast;

using namespace soul::ast::span;
using namespace p::lexer;
using namespace p::core::parsing_context;
using namespace p::ast;

export namespace p::common::parser {

template<typename LexerT>
struct CommonParser
{
    static soul::parser::Match UnitName(LexerT& lexer);
    static soul::parser::Match UsesClause(LexerT& lexer);
    static soul::parser::Match Identifier(LexerT& lexer);
    static soul::parser::Match IdentifierList(LexerT& lexer);
};

} // namespace p::common::parser
