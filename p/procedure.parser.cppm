
// this file has been automatically generated from 'C:/work/springpp/p/procedure.parser' using soul parser generator spg version 5.0.0

export module p.procedure.parser;

import std.core;
import soul.lexer;
import soul.parser;
import p.lexer;
import p.parsing_context;
import p.ast;
import p.subroutine;

using namespace p::lexer;
using namespace p::parsing_context;
using namespace p::ast;
using namespace p::subroutine;

export namespace p::procedure::parser {

template<typename LexerT>
struct ProcedureParser
{
    static soul::parser::Match ProcedureDeclaration(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match ProcedureHeading(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match ProcedureName(LexerT& lexer, ParsingContext* context);
};

} // namespace p::procedure::parser
