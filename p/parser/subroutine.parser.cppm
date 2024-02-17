
// this file has been automatically generated from 'C:/work/p-lang/p/parser/subroutine.parser' using soul parser generator spg version 5.0.0

export module p.subroutine.parser;

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

export namespace p::subroutine::parser {

template<typename LexerT>
struct SubroutineParser
{
    static soul::parser::Match ProcedureDeclaration(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match FunctionDeclaration(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match ConstructorDeclaration(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match ConstructorCall(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match Virtual(LexerT& lexer);
    static soul::parser::Match MethodHeading(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match SubroutineId(LexerT& lexer);
    static soul::parser::Match ProcedureHeading(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match FunctionHeading(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match ConstructorHeading(LexerT& lexer, ParsingContext* context);
};

} // namespace p::subroutine::parser
