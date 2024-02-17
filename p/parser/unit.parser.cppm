
// this file has been automatically generated from 'C:/work/p-lang/p/parser/unit.parser' using soul parser generator spg version 5.0.0

export module p.unit.parser;

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

export namespace p::unit::parser {

template<typename LexerT>
struct UnitParser
{
    static soul::parser::Match Unit(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match InterfacePart(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match ProcedureAndFunctionHeadingPart(LexerT& lexer, ParsingContext* context, p::BlockNode* block);
    static soul::parser::Match ImplementationPart(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match InitializationPart(LexerT& lexer, ParsingContext* context);
};

} // namespace p::unit::parser
