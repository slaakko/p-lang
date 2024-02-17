
// this file has been automatically generated from 'C:/work/p-lang/p/parser/main.parser' using soul parser generator spg version 5.0.0

export module p.main_parser;

import std.core;
import soul.lexer;
import soul.parser;
import p.lexer;
import p.core.parsing_context;
import p.ast;

using namespace p::lexer;
using namespace p::core::parsing_context;
using namespace p::ast;

export namespace p::main_parser {

template<typename LexerT>
struct MainParser
{
    static std::unique_ptr<p::Node> Parse(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match Main(LexerT& lexer, ParsingContext* context);
};

} // namespace p::main_parser
