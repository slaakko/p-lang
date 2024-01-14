
// this file has been automatically generated from 'C:/work/springpp/p/parameter.parser' using soul parser generator spg version 5.0.0

export module p.parameter.parser;

import std.core;
import soul.lexer;
import soul.parser;
import p.lexer;
import p.parsing_context;
import p.type;
import p.subroutine;

using namespace p::lexer;
using namespace p::parsing_context;
using namespace p::type;
using namespace p::subroutine;

export namespace p::parameter::parser {

template<typename LexerT>
struct ParameterParser
{
    static soul::parser::Match ParameterList(LexerT& lexer, ParsingContext* context, SubroutineHeading* heading);
    static soul::parser::Match ParameterDeclaration(LexerT& lexer, ParsingContext* context, SubroutineHeading* heading);
    static soul::parser::Match ParameterQualifier(LexerT& lexer);
    static soul::parser::Match ParameterType(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match IdentifierList(LexerT& lexer);
};

} // namespace p::parameter::parser
