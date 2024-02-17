
// this file has been automatically generated from 'C:/work/p-lang/p/parser/expression.parser' using soul parser generator spg version 5.0.0

export module p.expression.parser;

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

export namespace p::expression::parser {

template<typename LexerT>
struct ExpressionParser
{
    static soul::parser::Match ConstantExpression(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match Expression(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match RelationalOperator(LexerT& lexer);
    static soul::parser::Match SimpleExpression(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match WeakOperator(LexerT& lexer);
    static soul::parser::Match Term(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match StrongOperator(LexerT& lexer);
    static soul::parser::Match Factor(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match Primary(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match UnsignedConstant(LexerT& lexer);
    static soul::parser::Match UnsignedNumber(LexerT& lexer);
    static soul::parser::Match Sign(LexerT& lexer);
    static soul::parser::Match CharacterString(LexerT& lexer);
    static soul::parser::Match Qualifier(LexerT& lexer, ParsingContext* context, p::Node* primary);
    static soul::parser::Match Index(LexerT& lexer, ParsingContext* context, p::Node* primary);
    static soul::parser::Match MemberSelection(LexerT& lexer, ParsingContext* context, p::Node* primary);
    static soul::parser::Match ArgumentList(LexerT& lexer, ParsingContext* context, p::Node* owner);
    static soul::parser::Match Argument(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match NewExpression(LexerT& lexer, ParsingContext* context);
};

} // namespace p::expression::parser
