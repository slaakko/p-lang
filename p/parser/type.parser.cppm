
// this file has been automatically generated from 'C:/work/p-lang/p/parser/type.parser' using soul parser generator spg version 5.0.0

export module p.type.parser;

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

export namespace p::type::parser {

template<typename LexerT>
struct TypeParser
{
    static soul::parser::Match TypeName(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match TypeDeclaration(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match Type(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match SimpleType(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match OrdinalType(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match SubrangeType(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match EnumeratedType(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match OrdinalTypeName(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match RealType(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match StringType(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match PointerType(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match ObjectType(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match Heritage(LexerT& lexer);
    static soul::parser::Match GenericPart(LexerT& lexer);
    static soul::parser::Match ComponentList(LexerT& lexer, ParsingContext* context, ObjectTypeNode* objectType);
    static soul::parser::Match ObjectFieldList(LexerT& lexer, ParsingContext* context, ObjectTypeNode* objectType);
    static soul::parser::Match MethodList(LexerT& lexer, ParsingContext* context, ObjectTypeNode* objectType);
    static soul::parser::Match ArrayType(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match Specialization(LexerT& lexer, ParsingContext* context);
};

} // namespace p::type::parser
