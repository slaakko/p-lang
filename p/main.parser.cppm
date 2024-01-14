
// this file has been automatically generated from 'C:/work/springpp/p/main.parser' using soul parser generator spg version 5.0.0

export module p.main_parser;

import std.core;
import soul.lexer;
import soul.parser;
import p.lexer;
import p.parsing_context;

using namespace p::lexer;
using namespace p::parsing_context;

export namespace p::main_parser {

template<typename LexerT>
struct MainParser
{
    static void Parse(LexerT& lexer, ParsingContext* context);
    static soul::parser::Match Main(LexerT& lexer, ParsingContext* context);
};

} // namespace p::main_parser
