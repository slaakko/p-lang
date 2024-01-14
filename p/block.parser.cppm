
// this file has been automatically generated from 'C:/work/springpp/p/block.parser' using soul parser generator spg version 5.0.0

export module p.block.parser;

import std.core;
import soul.lexer;
import soul.parser;
import p.lexer;
import p.parsing_context;
import p.ast;
import p.type;
import p.constant;
import p.subroutine;
import p.block;

using namespace p::lexer;
using namespace p::parsing_context;
using namespace p::ast;
using namespace p::type;
using namespace p::constant;
using namespace p::subroutine;
using namespace p::block;

export namespace p::block::parser {

template<typename LexerT>
struct BlockParser
{
    static soul::parser::Match Block(LexerT& lexer, ParsingContext* context, Subroutine* subroutine, bool root);
    static soul::parser::Match SubroutineBlock(LexerT& lexer, ParsingContext* context, Subroutine* subroutine);
    static soul::parser::Match DeclarationPart(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match ConstantDeclarationPart(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match TypeDeclarationPart(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match VariableDeclarationPart(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match SubroutineDeclarationPart(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match StatementPart(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match ConstantDeclaration(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match Constant(LexerT& lexer, ParsingContext* context, p::Type* type);
    static soul::parser::Match ArrayConstant(LexerT& lexer, ParsingContext* context, p::Type* type);
    static soul::parser::Match ObjectConstant(LexerT& lexer, ParsingContext* context, p::Type* type);
    static soul::parser::Match FieldName(LexerT& lexer);
    static soul::parser::Match VariableDeclaration(LexerT& lexer, ParsingContext* context);
};

} // namespace p::block::parser
