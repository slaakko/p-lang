
// this file has been automatically generated from 'C:/work/springpp/p/unit.parser' using soul parser generator spg version 5.0.0

export module p.unit.parser;

import std.core;
import soul.lexer;
import soul.parser;
import p.lexer;
import p.parsing_context;
import p.subroutine;
import p.ast;

using namespace p::lexer;
using namespace p::parsing_context;
using namespace p::subroutine;
using namespace p::ast;

export namespace p::unit::parser {

template<typename LexerT>
struct UnitParser
{
    static soul::parser::Match Unit(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match UnitHeading(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match UnitIdentifier(LexerT& lexer);
    static soul::parser::Match InterfacePart(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match ProcedureAndFunctionHeadingPart(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match ImplementationPart(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match InitializationPart(LexerT& lexer, ParsingContext* context);
};

} // namespace p::unit::parser
