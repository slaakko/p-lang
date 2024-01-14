
// this file has been automatically generated from 'C:/work/springpp/p/statement.parser' using soul parser generator spg version 5.0.0

export module p.statement.parser;

import std.core;
import soul.lexer;
import soul.parser;
import p.lexer;
import p.parsing_context;
import p.ast;

using namespace p::lexer;
using namespace p::parsing_context;
using namespace p::ast;

export namespace p::statement::parser {

template<typename LexerT>
struct StatementParser
{
    static soul::parser::Match Statement(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match SimpleStatent(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match AssignmentStatement(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match ProcedureStatement(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match ProcedureIdentifier(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match StructuredStatement(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match CompoundStatement(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match ConditionalStatement(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match IfStatement(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match CaseStatement(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match Case(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match ConstantRange(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match ElsePart(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match RepetitiveStatement(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match RepeatStatement(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match WhileStatement(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match Direction(LexerT& lexer);
    static soul::parser::Match ForStatement(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match ControlVariable(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match InitialValue(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match FinalValue(LexerT& lexer, ParsingContext* context);
};

} // namespace p::statement::parser
