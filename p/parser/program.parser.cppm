
// this file has been automatically generated from 'C:/work/p-lang/p/parser/program.parser' using soul parser generator spg version 5.0.0

export module p.program.parser;

import std.core;
import soul.lexer;
import soul.parser;
import p.ast;
import p.lexer;
import p.core.parsing_context;
import soul.ast.span;

using namespace p::ast;
using namespace p::lexer;
using namespace p::core::parsing_context;
using namespace soul::ast::span;

export namespace p::program::parser {

template<typename LexerT>
struct ProgramParser
{
    static soul::parser::Match Program(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match ProgramParameterList(LexerT& lexer);
};

} // namespace p::program::parser
