
// this file has been automatically generated from 'C:/work/springpp/p/function.parser' using soul parser generator spg version 5.0.0

export module p.function.parser;

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

export namespace p::function::parser {

template<typename LexerT>
struct FunctionParser
{
    static soul::parser::Match FunctionDeclaration(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match FunctionHeading(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match FunctionName(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match ResultType(LexerT& lexer, ParsingContext* context);
};

} // namespace p::function::parser
