
// this file has been automatically generated from 'C:/work/springpp/p/program.parser' using soul parser generator spg version 5.0.0

export module p.program.parser;

import std.core;
import soul.lexer;
import soul.parser;
import p.lexer;
import p.parsing_context;
import p.block;
import p.subroutine;

using namespace p::lexer;
using namespace p::parsing_context;
using namespace p::block;
using namespace p::subroutine;

export namespace p::program::parser {

template<typename LexerT>
struct ProgramParser
{
    static soul::parser::Match Program(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match ProgramHeading(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match ProgramId(LexerT& lexer);
    static soul::parser::Match ProgramParameterList(LexerT& lexer);
    static soul::parser::Match UsesClause(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match UnitName(LexerT& lexer, ParsingContext* context);
};

} // namespace p::program::parser
