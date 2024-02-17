
// this file has been automatically generated from 'C:/work/p-lang/p/parser/block.parser' using soul parser generator spg version 5.0.0

export module p.block.parser;

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

export namespace p::block::parser {

template<typename LexerT>
struct BlockParser
{
    static soul::parser::Match Block(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match SubroutineBlock(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match DeclarationPart(LexerT& lexer, ParsingContext* context, p::BlockNode* block);
    static soul::parser::Match ConstantDeclarationPart(LexerT& lexer, ParsingContext* context, p::BlockNode* block);
    static soul::parser::Match TypeDeclarationPart(LexerT& lexer, ParsingContext* context, p::BlockNode* block);
    static soul::parser::Match VariableDeclarationPart(LexerT& lexer, ParsingContext* context, p::BlockNode* block);
    static soul::parser::Match SubroutineDeclarationPart(LexerT& lexer, ParsingContext* context, p::BlockNode* block);
    static soul::parser::Match StatementPart(LexerT& lexer, ParsingContext* context, p::BlockNode* block);
};

} // namespace p::block::parser
