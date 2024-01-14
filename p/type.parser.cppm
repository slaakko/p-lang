
// this file has been automatically generated from 'C:/work/springpp/p/type.parser' using soul parser generator spg version 5.0.0

export module p.type.parser;

import std.core;
import soul.lexer;
import soul.parser;
import p.lexer;
import p.parsing_context;
import p.ast;
import p.type;
import p.subroutine;

using namespace p::lexer;
using namespace p::parsing_context;
using namespace p::ast;
using namespace p::type;
using namespace p::subroutine;

export namespace p::type::parser {

template<typename LexerT>
struct TypeParser
{
    static soul::parser::Match TypeDeclaration(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match Type(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match SimpleType(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match OrdinalType(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match OrdinalTypeName(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match SubrangeType(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match EnumeratedType(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match RealType(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match StringType(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match PointerType(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match ObjectType(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match Heritage(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match ObjectTypeIdentifier(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match ComponentList(LexerT& lexer, ParsingContext* context, p::ObjectType* objectType);
    static soul::parser::Match ObjectFieldList(LexerT& lexer, ParsingContext* context, p::ObjectType* objectType);
    static soul::parser::Match MethodList(LexerT& lexer, ParsingContext* context, p::ObjectType* objectType);
    static soul::parser::Match Virtuality(LexerT& lexer);
    static soul::parser::Match MethodHeading(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match ConstructorDeclaration(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match ConstructorHeading(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match ObjectName(LexerT& lexer);
    static soul::parser::Match ConstructorCall(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match ArrayType(LexerT& lexer, ParsingContext* context);
};

} // namespace p::type::parser
