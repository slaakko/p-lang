
// this file has been automatically generated from 'C:/work/springpp/p/main.parser' using soul parser generator spg version 5.0.0

module p.main_parser;

import util;
import soul.ast.spg;
import p.token;
import p.program.parser;
import p.unit.parser;

using namespace p::token;
using namespace p::program::parser;
using namespace p::unit::parser;

namespace p::main_parser {

template<typename LexerT>
void MainParser<LexerT>::Parse(LexerT& lexer, ParsingContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (lexer.Log())
    {
        lexer.Log()->WriteBeginRule("parse");
        lexer.Log()->IncIndent();
    }
    #endif
    ++lexer;
    soul::parser::Match match = MainParser<LexerT>::Main(lexer, context);
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (lexer.Log())
    {
        lexer.Log()->DecIndent();
        lexer.Log()->WriteEndRule("parse");
    }
    #endif
    if (match.hit)
    {
        if (*lexer == soul::lexer::END_TOKEN)
        {
            return;
        }
        else
        {
            lexer.ThrowFarthestError();
        }
    }
    else
    {
        lexer.ThrowFarthestError();
    }
    return;
}

template<typename LexerT>
soul::parser::Match MainParser<LexerT>::Main(LexerT& lexer, ParsingContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Main");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 5007228952788336641);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    switch (*lexer)
    {
        case PROGRAM:
        {
            soul::parser::Match match = ProgramParser<LexerT>::Program(lexer, context);
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case UNIT:
        {
            soul::parser::Match match = UnitParser<LexerT>::Unit(lexer, context);
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Main");
        else soul::lexer::WriteFailureToLog(lexer, "Main");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template struct MainParser<soul::lexer::Lexer<p::lexer::PLexer<char>, char>>;

} // namespace p::main_parser
