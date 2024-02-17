
// this file has been automatically generated from 'C:/work/p-lang/p/parser/variable.parser' using soul parser generator spg version 5.0.0

module p.variable.parser;

import util;
import soul.ast.spg;
import p.token;

using namespace p::token;

namespace p::variable::parser {

template<typename LexerT>
soul::parser::Match VariableParser<LexerT>::VariableDeclaration(LexerT& lexer, ParsingContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "VariableDeclaration");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 6197679748483645441);
    soul::ast::Span span = soul::ast::Span();
    std::unique_ptr<p::IdentifierListNode> identifierList;
    std::unique_ptr<p::Node> typeName;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch4 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch5 = &match;
                        {
                            int64_t pos = lexer.GetPos();
                            soul::parser::Match match = CommonParser<LexerT>::IdentifierList(lexer);
                            identifierList.reset(static_cast<p::IdentifierListNode*>(match.value));
                            if (match.hit)
                            {
                                span = lexer.GetSpan(pos);
                            }
                            *parentMatch5 = match;
                        }
                        *parentMatch4 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch6 = &match;
                        {
                            soul::parser::Match match(false);
                            if (*lexer == COLON)
                            {
                                ++lexer;
                                match.hit = true;
                            }
                            *parentMatch6 = match;
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch7 = &match;
                    {
                        soul::parser::Match match = TypeParser<LexerT>::TypeName(lexer, context);
                        typeName.reset(static_cast<p::Node*>(match.value));
                        *parentMatch7 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch8 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch9 = &match;
                    {
                        int64_t pos = lexer.GetPos();
                        soul::parser::Match match(false);
                        if (*lexer == SEMICOLON)
                        {
                            ++lexer;
                            match.hit = true;
                        }
                        if (match.hit)
                        {
                            span.Union(lexer.GetSpan(pos));
                        }
                        *parentMatch9 = match;
                    }
                    *parentMatch8 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "VariableDeclaration");
                #endif
                return soul::parser::Match(true, new VariableDeclarationNode(span, identifierList.release(), typeName.release()));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "VariableDeclaration");
        else soul::lexer::WriteFailureToLog(lexer, "VariableDeclaration");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template struct VariableParser<soul::lexer::Lexer<p::lexer::PLexer<char>, char>>;

} // namespace p::variable::parser
