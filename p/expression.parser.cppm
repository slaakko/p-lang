
// this file has been automatically generated from 'C:/work/springpp/p/expression.parser' using soul parser generator spg version 5.0.0

export module p.expression.parser;

import std.core;
import soul.lexer;
import soul.parser;
import p.lexer;
import p.parsing_context;
import p.ast;

using namespace p::lexer;
using namespace p::parsing_context;
using namespace p::ast;

export namespace p::expression::parser {

template<typename LexerT>
struct ExpressionParser
{
    static soul::parser::Match Expression(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match RelationalOperator(LexerT& lexer);
    static soul::parser::Match ConstantExpression(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match SimpleExpression(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match WeakOperator(LexerT& lexer);
    static soul::parser::Match Term(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match StrongOperator(LexerT& lexer);
    static soul::parser::Match Factor(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match Primary(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match UnsignedConstant(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match UnsignedNumber(LexerT& lexer);
    static soul::parser::Match CharacterString(LexerT& lexer);
    static soul::parser::Match ConstantIdentifier(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match Sign(LexerT& lexer);
    static soul::parser::Match ArgumentList(LexerT& lexer, ParsingContext* context, Node* owner);
    static soul::parser::Match Argument(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match FunctionIdentifier(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match TypeName(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match ValueTypecast(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match TypeIdentifier(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match VariableReference(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match VariableIdentifier(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match Field(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match Method(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match Qualifier(LexerT& lexer, ParsingContext* context, Node* primary);
    static soul::parser::Match Index(LexerT& lexer, ParsingContext* context, Node* primary);
    static soul::parser::Match FieldDesignator(LexerT& lexer, ParsingContext* context, Node* primary);
    static soul::parser::Match Identifier(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match VariableTypecast(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match NewExpression(LexerT& lexer, ParsingContext* context);
};

} // namespace p::expression::parser
