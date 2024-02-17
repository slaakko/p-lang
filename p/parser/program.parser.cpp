
// this file has been automatically generated from 'C:/work/p-lang/p/parser/program.parser' using soul parser generator spg version 5.0.0

module p.program.parser;

import util;
import soul.ast.spg;
import p.token;
import p.common.parser;
import p.block.parser;

using namespace p::token;
using namespace p::common::parser;
using namespace p::block::parser;

namespace p::program::parser {

template<typename LexerT>
soul::parser::Match ProgramParser<LexerT>::Program(LexerT& lexer, ParsingContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Program");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 636845565828661249);
    soul::ast::Span span = soul::ast::Span();
    std::unique_ptr<p::ProgramNode> programNode = std::unique_ptr<p::ProgramNode>();
    std::unique_ptr<p::UnitNameNode> unitName;
    std::unique_ptr<p::IdentifierListNode> programParameters;
    std::unique_ptr<p::UnitNameListNode> usesList;
    std::unique_ptr<p::BlockNode> programBlock;
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
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch6 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch7 = &match;
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch8 = &match;
                                    {
                                        int64_t pos = lexer.GetPos();
                                        soul::parser::Match match(false);
                                        if (*lexer == PROGRAM)
                                        {
                                            ++lexer;
                                            match.hit = true;
                                        }
                                        if (match.hit)
                                        {
                                            span = lexer.GetSpan(pos);
                                        }
                                        *parentMatch8 = match;
                                    }
                                    *parentMatch7 = match;
                                }
                                if (match.hit)
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch9 = &match;
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch10 = &match;
                                        {
                                            int64_t pos = lexer.GetPos();
                                            soul::parser::Match match = CommonParser<LexerT>::UnitName(lexer);
                                            unitName.reset(static_cast<p::UnitNameNode*>(match.value));
                                            if (match.hit)
                                            {
                                                span.Union(lexer.GetSpan(pos));
                                                programNode.reset(new p::ProgramNode(span, lexer.FileName(), unitName.release()));
                                            }
                                            *parentMatch10 = match;
                                        }
                                        *parentMatch9 = match;
                                    }
                                    *parentMatch7 = match;
                                }
                                *parentMatch6 = match;
                            }
                            if (match.hit)
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch11 = &match;
                                {
                                    soul::parser::Match match(true);
                                    int64_t save = lexer.GetPos();
                                    soul::parser::Match* parentMatch12 = &match;
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch13 = &match;
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch14 = &match;
                                            {
                                                int64_t pos = lexer.GetPos();
                                                soul::parser::Match match = ProgramParser<LexerT>::ProgramParameterList(lexer);
                                                programParameters.reset(static_cast<p::IdentifierListNode*>(match.value));
                                                if (match.hit)
                                                {
                                                    programNode->SetProgramParameters(programParameters.release());
                                                }
                                                *parentMatch14 = match;
                                            }
                                            *parentMatch13 = match;
                                        }
                                        if (match.hit)
                                        {
                                            *parentMatch12 = match;
                                        }
                                        else
                                        {
                                            lexer.SetPos(save);
                                        }
                                    }
                                    *parentMatch11 = match;
                                }
                                *parentMatch6 = match;
                            }
                            *parentMatch5 = match;
                        }
                        if (match.hit)
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch15 = &match;
                            {
                                soul::parser::Match match(false);
                                if (*lexer == SEMICOLON)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                *parentMatch15 = match;
                            }
                            *parentMatch5 = match;
                        }
                        *parentMatch4 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch16 = &match;
                        {
                            soul::parser::Match match(true);
                            int64_t save = lexer.GetPos();
                            soul::parser::Match* parentMatch17 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch18 = &match;
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch19 = &match;
                                    {
                                        int64_t pos = lexer.GetPos();
                                        soul::parser::Match match = CommonParser<LexerT>::UsesClause(lexer);
                                        usesList.reset(static_cast<p::UnitNameListNode*>(match.value));
                                        if (match.hit)
                                        {
                                            programNode->SetUsesList(usesList.release());
                                        }
                                        *parentMatch19 = match;
                                    }
                                    *parentMatch18 = match;
                                }
                                if (match.hit)
                                {
                                    *parentMatch17 = match;
                                }
                                else
                                {
                                    lexer.SetPos(save);
                                }
                            }
                            *parentMatch16 = match;
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch20 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch21 = &match;
                        {
                            int64_t pos = lexer.GetPos();
                            soul::parser::Match match = BlockParser<LexerT>::Block(lexer, context);
                            programBlock.reset(static_cast<p::BlockNode*>(match.value));
                            if (match.hit)
                            {
                                programNode->SetBlock(programBlock.release());
                            }
                            *parentMatch21 = match;
                        }
                        *parentMatch20 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch22 = &match;
                {
                    soul::parser::Match match(false);
                    if (*lexer == DOT)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    *parentMatch22 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Program");
                #endif
                return soul::parser::Match(true, programNode.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Program");
        else soul::lexer::WriteFailureToLog(lexer, "Program");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match ProgramParser<LexerT>::ProgramParameterList(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ProgramParameterList");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 636845565828661250);
    std::unique_ptr<p::IdentifierListNode> identifierList;
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
                    if (*lexer == LPAREN)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch4 = &match;
                    {
                        soul::parser::Match match(true);
                        int64_t save = lexer.GetPos();
                        soul::parser::Match* parentMatch5 = &match;
                        {
                            soul::parser::Match match = CommonParser<LexerT>::IdentifierList(lexer);
                            identifierList.reset(static_cast<p::IdentifierListNode*>(match.value));
                            if (match.hit)
                            {
                                *parentMatch5 = match;
                            }
                            else
                            {
                                lexer.SetPos(save);
                            }
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch6 = &match;
                {
                    soul::parser::Match match(false);
                    if (*lexer == RPAREN)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    *parentMatch6 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ProgramParameterList");
                #endif
                return soul::parser::Match(true, identifierList.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ProgramParameterList");
        else soul::lexer::WriteFailureToLog(lexer, "ProgramParameterList");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template struct ProgramParser<soul::lexer::Lexer<p::lexer::PLexer<char>, char>>;

} // namespace p::program::parser
