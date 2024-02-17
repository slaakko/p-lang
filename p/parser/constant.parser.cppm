
// this file has been automatically generated from 'C:/work/p-lang/p/parser/constant.parser' using soul parser generator spg version 5.0.0

export module p.constant.parser;

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

export namespace p::constant::parser {

template<typename LexerT>
struct ConstantParser
{
    static soul::parser::Match ConstantDeclaration(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match Constant(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match ArrayConstant(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match ObjectConstant(LexerT& lexer, ParsingContext* context);
};

} // namespace p::constant::parser
