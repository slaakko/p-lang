
// this file has been automatically generated from 'C:/work/p-lang/p/parser/variable.parser' using soul parser generator spg version 5.0.0

export module p.variable.parser;

import std.core;
import soul.lexer;
import soul.parser;
import soul.ast.span;
import p.lexer;
import p.core.parsing_context;
import p.ast;
import p.common.parser;
import p.type.parser;

using namespace soul::ast::span;
using namespace p::lexer;
using namespace p::core::parsing_context;
using namespace p::ast;
using namespace p::common::parser;
using namespace p::type::parser;

export namespace p::variable::parser {

template<typename LexerT>
struct VariableParser
{
    static soul::parser::Match VariableDeclaration(LexerT& lexer, ParsingContext* context);
};

} // namespace p::variable::parser
