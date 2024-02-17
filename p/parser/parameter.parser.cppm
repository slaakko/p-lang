
// this file has been automatically generated from 'C:/work/p-lang/p/parser/parameter.parser' using soul parser generator spg version 5.0.0

export module p.parameter.parser;

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

export namespace p::parameter::parser {

template<typename LexerT>
struct ParameterParser
{
    static soul::parser::Match ParameterList(LexerT& lexer, ParsingContext* context, p::SubroutineHeadingNode* heading);
    static soul::parser::Match ParameterDeclaration(LexerT& lexer, ParsingContext* context, p::SubroutineHeadingNode* heading);
    static soul::parser::Match ParameterQualifier(LexerT& lexer);
};

} // namespace p::parameter::parser
