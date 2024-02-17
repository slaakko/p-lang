
// this file has been automatically generated from 'C:/work/p-lang/p/parser/subroutine.parser' using soul parser generator spg version 5.0.0

module p.subroutine.parser;

import util;
import soul.ast.spg;
import p.token;
import p.parameter.parser;
import p.common.parser;
import p.type.parser;
import p.block.parser;
import p.expression.parser;

using namespace p::token;
using namespace p::parameter::parser;
using namespace p::common::parser;
using namespace p::type::parser;
using namespace p::block::parser;
using namespace p::expression::parser;

namespace p::subroutine::parser {

template<typename LexerT>
soul::parser::Match SubroutineParser<LexerT>::ProcedureDeclaration(LexerT& lexer, ParsingContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ProcedureDeclaration");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 8014466136055742465);
    soul::ast::Span span = soul::ast::Span();
    std::unique_ptr<p::ProcedureHeadingNode> procedureHeading;
    std::unique_ptr<p::Node> subroutineBlock;
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
                            soul::parser::Match match = SubroutineParser<LexerT>::ProcedureHeading(lexer, context);
                            procedureHeading.reset(static_cast<p::ProcedureHeadingNode*>(match.value));
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
                            if (*lexer == SEMICOLON)
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
                        soul::parser::Match match = BlockParser<LexerT>::SubroutineBlock(lexer, context);
                        subroutineBlock.reset(static_cast<p::Node*>(match.value));
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
                    if (*lexer == SEMICOLON)
                    {
                        ++lexer;
                        match.hit = true;
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
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ProcedureDeclaration");
                #endif
                return soul::parser::Match(true, new p::ProcedureDeclarationNode(span, procedureHeading.release(), subroutineBlock.release()));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ProcedureDeclaration");
        else soul::lexer::WriteFailureToLog(lexer, "ProcedureDeclaration");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match SubroutineParser<LexerT>::FunctionDeclaration(LexerT& lexer, ParsingContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "FunctionDeclaration");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 8014466136055742466);
    soul::ast::Span span = soul::ast::Span();
    std::unique_ptr<p::FunctionHeadingNode> functionHeading;
    std::unique_ptr<p::Node> subroutineBlock;
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
                            soul::parser::Match match = SubroutineParser<LexerT>::FunctionHeading(lexer, context);
                            functionHeading.reset(static_cast<p::FunctionHeadingNode*>(match.value));
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
                            if (*lexer == SEMICOLON)
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
                        soul::parser::Match match = BlockParser<LexerT>::SubroutineBlock(lexer, context);
                        subroutineBlock.reset(static_cast<p::Node*>(match.value));
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
                    if (*lexer == SEMICOLON)
                    {
                        ++lexer;
                        match.hit = true;
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
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "FunctionDeclaration");
                #endif
                return soul::parser::Match(true, new p::FunctionDeclarationNode(span, functionHeading.release(), subroutineBlock.release()));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "FunctionDeclaration");
        else soul::lexer::WriteFailureToLog(lexer, "FunctionDeclaration");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match SubroutineParser<LexerT>::ConstructorDeclaration(LexerT& lexer, ParsingContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ConstructorDeclaration");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 8014466136055742467);
    soul::ast::Span span = soul::ast::Span();
    std::unique_ptr<p::ConstructorHeadingNode> constructorHeading;
    std::unique_ptr<p::ConstructorCallNode> constructorCall;
    std::unique_ptr<p::Node> subroutineBlock;
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
                                int64_t pos = lexer.GetPos();
                                soul::parser::Match match = SubroutineParser<LexerT>::ConstructorHeading(lexer, context);
                                constructorHeading.reset(static_cast<p::ConstructorHeadingNode*>(match.value));
                                if (match.hit)
                                {
                                    span = lexer.GetSpan(pos);
                                }
                                *parentMatch6 = match;
                            }
                            *parentMatch5 = match;
                        }
                        if (match.hit)
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch7 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch8 = &match;
                                {
                                    soul::parser::Match match(true);
                                    int64_t save = lexer.GetPos();
                                    soul::parser::Match* parentMatch9 = &match;
                                    {
                                        soul::parser::Match match = SubroutineParser<LexerT>::ConstructorCall(lexer, context);
                                        constructorCall.reset(static_cast<p::ConstructorCallNode*>(match.value));
                                        if (match.hit)
                                        {
                                            *parentMatch9 = match;
                                        }
                                        else
                                        {
                                            lexer.SetPos(save);
                                        }
                                    }
                                    *parentMatch8 = match;
                                }
                                *parentMatch7 = match;
                            }
                            *parentMatch5 = match;
                        }
                        *parentMatch4 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch10 = &match;
                        {
                            soul::parser::Match match(false);
                            if (*lexer == SEMICOLON)
                            {
                                ++lexer;
                                match.hit = true;
                            }
                            *parentMatch10 = match;
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch11 = &match;
                    {
                        soul::parser::Match match = BlockParser<LexerT>::SubroutineBlock(lexer, context);
                        subroutineBlock.reset(static_cast<p::Node*>(match.value));
                        *parentMatch11 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch12 = &match;
                {
                    soul::parser::Match match(false);
                    if (*lexer == SEMICOLON)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    *parentMatch12 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ConstructorDeclaration");
                #endif
                return soul::parser::Match(true, new p::ConstructorDeclarationNode(span, constructorHeading.release(), constructorCall.release(), subroutineBlock.release()));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ConstructorDeclaration");
        else soul::lexer::WriteFailureToLog(lexer, "ConstructorDeclaration");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match SubroutineParser<LexerT>::ConstructorCall(LexerT& lexer, ParsingContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ConstructorCall");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 8014466136055742468);
    soul::ast::Span span = soul::ast::Span();
    std::unique_ptr<p::ConstructorCallNode> node = std::unique_ptr<p::ConstructorCallNode>();
    std::unique_ptr<p::Node> first;
    std::unique_ptr<p::Node> next;
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
                                int64_t pos = lexer.GetPos();
                                soul::parser::Match match(false);
                                if (*lexer == COLON)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                if (match.hit)
                                {
                                    span = lexer.GetSpan(pos);
                                }
                                *parentMatch6 = match;
                            }
                            *parentMatch5 = match;
                        }
                        if (match.hit)
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch7 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch8 = &match;
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch9 = &match;
                                    switch (*lexer)
                                    {
                                        case BASE:
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch10 = &match;
                                            {
                                                int64_t pos = lexer.GetPos();
                                                soul::parser::Match match(false);
                                                if (*lexer == BASE)
                                                {
                                                    ++lexer;
                                                    match.hit = true;
                                                }
                                                if (match.hit)
                                                {
                                                    span.Union(lexer.GetSpan(pos));
                                                    node.reset(new p::ConstructorCallNode(span, new p::BaseNode(lexer.GetSpan(pos))));
                                                }
                                                *parentMatch10 = match;
                                            }
                                            if (match.hit)
                                            {
                                                *parentMatch9 = match;
                                            }
                                            break;
                                        }
                                        case THIS:
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch11 = &match;
                                            {
                                                int64_t pos = lexer.GetPos();
                                                soul::parser::Match match(false);
                                                if (*lexer == THIS)
                                                {
                                                    ++lexer;
                                                    match.hit = true;
                                                }
                                                if (match.hit)
                                                {
                                                    span.Union(lexer.GetSpan(pos));
                                                    node.reset(new p::ConstructorCallNode(span, new p::ThisNode(lexer.GetSpan(pos))));
                                                }
                                                *parentMatch11 = match;
                                            }
                                            if (match.hit)
                                            {
                                                *parentMatch9 = match;
                                            }
                                            break;
                                        }
                                    }
                                    *parentMatch8 = match;
                                }
                                *parentMatch7 = match;
                            }
                            *parentMatch5 = match;
                        }
                        *parentMatch4 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch12 = &match;
                        {
                            soul::parser::Match match(false);
                            if (*lexer == LPAREN)
                            {
                                ++lexer;
                                match.hit = true;
                            }
                            *parentMatch12 = match;
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch13 = &match;
                    {
                        soul::parser::Match match(true);
                        int64_t save = lexer.GetPos();
                        soul::parser::Match* parentMatch14 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch15 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch16 = &match;
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch17 = &match;
                                    {
                                        int64_t pos = lexer.GetPos();
                                        soul::parser::Match match = ExpressionParser<LexerT>::Expression(lexer, context);
                                        first.reset(static_cast<p::Node*>(match.value));
                                        if (match.hit)
                                        {
                                            node->AddArgument(first.release());
                                        }
                                        *parentMatch17 = match;
                                    }
                                    *parentMatch16 = match;
                                }
                                if (match.hit)
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch18 = &match;
                                    {
                                        soul::parser::Match match(true);
                                        soul::parser::Match* parentMatch19 = &match;
                                        {
                                            while (true)
                                            {
                                                int64_t save = lexer.GetPos();
                                                {
                                                    soul::parser::Match match(false);
                                                    soul::parser::Match* parentMatch20 = &match;
                                                    {
                                                        soul::parser::Match match(false);
                                                        soul::parser::Match* parentMatch21 = &match;
                                                        {
                                                            soul::parser::Match match(false);
                                                            if (*lexer == COMMA)
                                                            {
                                                                ++lexer;
                                                                match.hit = true;
                                                            }
                                                            *parentMatch21 = match;
                                                        }
                                                        if (match.hit)
                                                        {
                                                            soul::parser::Match match(false);
                                                            soul::parser::Match* parentMatch22 = &match;
                                                            {
                                                                soul::parser::Match match(false);
                                                                soul::parser::Match* parentMatch23 = &match;
                                                                {
                                                                    int64_t pos = lexer.GetPos();
                                                                    soul::parser::Match match = ExpressionParser<LexerT>::Expression(lexer, context);
                                                                    next.reset(static_cast<p::Node*>(match.value));
                                                                    if (match.hit)
                                                                    {
                                                                        node->AddArgument(next.release());
                                                                    }
                                                                    *parentMatch23 = match;
                                                                }
                                                                *parentMatch22 = match;
                                                            }
                                                            *parentMatch21 = match;
                                                        }
                                                        *parentMatch20 = match;
                                                    }
                                                    if (match.hit)
                                                    {
                                                        *parentMatch19 = match;
                                                    }
                                                    else
                                                    {
                                                        lexer.SetPos(save);
                                                        break;
                                                    }
                                                }
                                            }
                                        }
                                        *parentMatch18 = match;
                                    }
                                    *parentMatch16 = match;
                                }
                                *parentMatch15 = match;
                            }
                            if (match.hit)
                            {
                                *parentMatch14 = match;
                            }
                            else
                            {
                                lexer.SetPos(save);
                            }
                        }
                        *parentMatch13 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch24 = &match;
                {
                    soul::parser::Match match(false);
                    if (*lexer == RPAREN)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    *parentMatch24 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ConstructorCall");
                #endif
                return soul::parser::Match(true, node.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ConstructorCall");
        else soul::lexer::WriteFailureToLog(lexer, "ConstructorCall");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match SubroutineParser<LexerT>::Virtual(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Virtual");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 8014466136055742469);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    switch (*lexer)
    {
        case VIRTUAL:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch1 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == VIRTUAL)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Virtual");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<p::Virtual>(p::Virtual::virtual_));
                    }
                }
                *parentMatch1 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case OVERRIDE:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == OVERRIDE)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Virtual");
                        #endif
                        return soul::parser::Match(true, new soul::parser::Value<p::Virtual>(p::Virtual::override_));
                    }
                }
                *parentMatch2 = match;
            }
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Virtual");
        else soul::lexer::WriteFailureToLog(lexer, "Virtual");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match SubroutineParser<LexerT>::MethodHeading(LexerT& lexer, ParsingContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "MethodHeading");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 8014466136055742470);
    std::unique_ptr<p::ProcedureHeadingNode> procedureHeadingNode = std::unique_ptr<p::ProcedureHeadingNode>();
    std::unique_ptr<p::FunctionHeadingNode> functionHeadingNode = std::unique_ptr<p::FunctionHeadingNode>();
    std::unique_ptr<p::ProcedureHeadingNode> procedureHeading;
    std::unique_ptr<soul::parser::Value<p::Virtual>> procVirtuality;
    std::unique_ptr<p::FunctionHeadingNode> functionHeading;
    std::unique_ptr<soul::parser::Value<p::Virtual>> funcVirtuality;
    std::unique_ptr<p::ConstructorHeadingNode> constructorHeading;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    switch (*lexer)
    {
        case PROCEDURE:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch1 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch2 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch3 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch4 = &match;
                        {
                            int64_t pos = lexer.GetPos();
                            soul::parser::Match match = SubroutineParser<LexerT>::ProcedureHeading(lexer, context);
                            procedureHeading.reset(static_cast<p::ProcedureHeadingNode*>(match.value));
                            if (match.hit)
                            {
                                procedureHeadingNode.reset(procedureHeading.release());
                            }
                            *parentMatch4 = match;
                        }
                        *parentMatch3 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch5 = &match;
                        {
                            soul::parser::Match match(true);
                            int64_t save = lexer.GetPos();
                            soul::parser::Match* parentMatch6 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch7 = &match;
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch8 = &match;
                                    {
                                        soul::parser::Match match(false);
                                        if (*lexer == SEMICOLON)
                                        {
                                            ++lexer;
                                            match.hit = true;
                                        }
                                        *parentMatch8 = match;
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
                                                soul::parser::Match match = SubroutineParser<LexerT>::Virtual(lexer);
                                                procVirtuality.reset(static_cast<soul::parser::Value<p::Virtual>*>(match.value));
                                                if (match.hit)
                                                {
                                                    procedureHeadingNode->SetVirtual(procVirtuality->value);
                                                }
                                                *parentMatch10 = match;
                                            }
                                            *parentMatch9 = match;
                                        }
                                        *parentMatch8 = match;
                                    }
                                    *parentMatch7 = match;
                                }
                                if (match.hit)
                                {
                                    *parentMatch6 = match;
                                }
                                else
                                {
                                    lexer.SetPos(save);
                                }
                            }
                            *parentMatch5 = match;
                        }
                        *parentMatch3 = match;
                    }
                    *parentMatch2 = match;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "MethodHeading");
                        #endif
                        return soul::parser::Match(true, procedureHeadingNode.release());
                    }
                }
                *parentMatch1 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case FUNCTION:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch11 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch12 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch13 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch14 = &match;
                        {
                            int64_t pos = lexer.GetPos();
                            soul::parser::Match match = SubroutineParser<LexerT>::FunctionHeading(lexer, context);
                            functionHeading.reset(static_cast<p::FunctionHeadingNode*>(match.value));
                            if (match.hit)
                            {
                                functionHeadingNode.reset(functionHeading.release());
                            }
                            *parentMatch14 = match;
                        }
                        *parentMatch13 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch15 = &match;
                        {
                            soul::parser::Match match(true);
                            int64_t save = lexer.GetPos();
                            soul::parser::Match* parentMatch16 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch17 = &match;
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch18 = &match;
                                    {
                                        soul::parser::Match match(false);
                                        if (*lexer == SEMICOLON)
                                        {
                                            ++lexer;
                                            match.hit = true;
                                        }
                                        *parentMatch18 = match;
                                    }
                                    if (match.hit)
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch19 = &match;
                                        {
                                            soul::parser::Match match(false);
                                            soul::parser::Match* parentMatch20 = &match;
                                            {
                                                int64_t pos = lexer.GetPos();
                                                soul::parser::Match match = SubroutineParser<LexerT>::Virtual(lexer);
                                                funcVirtuality.reset(static_cast<soul::parser::Value<p::Virtual>*>(match.value));
                                                if (match.hit)
                                                {
                                                    functionHeadingNode->SetVirtual(funcVirtuality->value);
                                                }
                                                *parentMatch20 = match;
                                            }
                                            *parentMatch19 = match;
                                        }
                                        *parentMatch18 = match;
                                    }
                                    *parentMatch17 = match;
                                }
                                if (match.hit)
                                {
                                    *parentMatch16 = match;
                                }
                                else
                                {
                                    lexer.SetPos(save);
                                }
                            }
                            *parentMatch15 = match;
                        }
                        *parentMatch13 = match;
                    }
                    *parentMatch12 = match;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "MethodHeading");
                        #endif
                        return soul::parser::Match(true, functionHeadingNode.release());
                    }
                }
                *parentMatch11 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case CONSTRUCTOR:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch21 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch22 = &match;
                {
                    int64_t pos = lexer.GetPos();
                    soul::parser::Match match = SubroutineParser<LexerT>::ConstructorHeading(lexer, context);
                    constructorHeading.reset(static_cast<p::ConstructorHeadingNode*>(match.value));
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "MethodHeading");
                            #endif
                            return soul::parser::Match(true, constructorHeading.release());
                        }
                    }
                    *parentMatch22 = match;
                }
                *parentMatch21 = match;
            }
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "MethodHeading");
        else soul::lexer::WriteFailureToLog(lexer, "MethodHeading");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match SubroutineParser<LexerT>::SubroutineId(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "SubroutineId");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 8014466136055742471);
    soul::ast::Span span = soul::ast::Span();
    std::unique_ptr<p::IdentifierNode> objectId;
    std::unique_ptr<p::IdentifierNode> subroutineId1;
    std::unique_ptr<p::IdentifierNode> subroutineId2;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t save = lexer.GetPos();
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    int64_t pos = lexer.GetPos();
                    soul::parser::Match match = CommonParser<LexerT>::Identifier(lexer);
                    objectId.reset(static_cast<p::IdentifierNode*>(match.value));
                    if (match.hit)
                    {
                        span = lexer.GetSpan(pos);
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch4 = &match;
                {
                    soul::parser::Match match(false);
                    if (*lexer == DOT)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    *parentMatch4 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch5 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch6 = &match;
                {
                    int64_t pos = lexer.GetPos();
                    soul::parser::Match match = CommonParser<LexerT>::Identifier(lexer);
                    subroutineId1.reset(static_cast<p::IdentifierNode*>(match.value));
                    if (match.hit)
                    {
                        span.Union(lexer.GetSpan(pos));
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "SubroutineId");
                            #endif
                            return soul::parser::Match(true, new p::SubroutineIdNode(span, objectId.release(), subroutineId1.release()));
                        }
                    }
                    *parentMatch6 = match;
                }
                *parentMatch5 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
        if (!match.hit)
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch7 = &match;
            lexer.SetPos(save);
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch8 = &match;
                {
                    int64_t pos = lexer.GetPos();
                    soul::parser::Match match = CommonParser<LexerT>::Identifier(lexer);
                    subroutineId2.reset(static_cast<p::IdentifierNode*>(match.value));
                    if (match.hit)
                    {
                        span = lexer.GetSpan(pos);
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "SubroutineId");
                            #endif
                            return soul::parser::Match(true, new p::SubroutineIdNode(span, nullptr, subroutineId2.release()));
                        }
                    }
                    *parentMatch8 = match;
                }
                *parentMatch7 = match;
            }
            *parentMatch0 = match;
        }
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "SubroutineId");
        else soul::lexer::WriteFailureToLog(lexer, "SubroutineId");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match SubroutineParser<LexerT>::ProcedureHeading(LexerT& lexer, ParsingContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ProcedureHeading");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 8014466136055742472);
    soul::ast::Span span = soul::ast::Span();
    std::unique_ptr<p::ProcedureHeadingNode> procedureHeading = std::unique_ptr<p::ProcedureHeadingNode>();
    std::unique_ptr<p::SubroutineIdNode> subroutineId;
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
                        int64_t pos = lexer.GetPos();
                        soul::parser::Match match(false);
                        if (*lexer == PROCEDURE)
                        {
                            ++lexer;
                            match.hit = true;
                        }
                        if (match.hit)
                        {
                            span = lexer.GetSpan(pos);
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch5 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch6 = &match;
                        {
                            int64_t pos = lexer.GetPos();
                            soul::parser::Match match = SubroutineParser<LexerT>::SubroutineId(lexer);
                            subroutineId.reset(static_cast<p::SubroutineIdNode*>(match.value));
                            if (match.hit)
                            {
                                span.Union(lexer.GetSpan(pos));
                                procedureHeading.reset(new p::ProcedureHeadingNode(span, subroutineId.release()));
                            }
                            *parentMatch6 = match;
                        }
                        *parentMatch5 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch7 = &match;
                {
                    soul::parser::Match match = ParameterParser<LexerT>::ParameterList(lexer, context, procedureHeading.get());
                    *parentMatch7 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ProcedureHeading");
                #endif
                return soul::parser::Match(true, procedureHeading.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ProcedureHeading");
        else soul::lexer::WriteFailureToLog(lexer, "ProcedureHeading");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match SubroutineParser<LexerT>::FunctionHeading(LexerT& lexer, ParsingContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "FunctionHeading");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 8014466136055742473);
    soul::ast::Span span = soul::ast::Span();
    std::unique_ptr<p::FunctionHeadingNode> functionHeading = std::unique_ptr<p::FunctionHeadingNode>();
    std::unique_ptr<p::SubroutineIdNode> subroutineId;
    std::unique_ptr<p::Node> resultType;
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
                                int64_t pos = lexer.GetPos();
                                soul::parser::Match match(false);
                                if (*lexer == FUNCTION)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                if (match.hit)
                                {
                                    span = lexer.GetSpan(pos);
                                }
                                *parentMatch6 = match;
                            }
                            *parentMatch5 = match;
                        }
                        if (match.hit)
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch7 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch8 = &match;
                                {
                                    int64_t pos = lexer.GetPos();
                                    soul::parser::Match match = SubroutineParser<LexerT>::SubroutineId(lexer);
                                    subroutineId.reset(static_cast<p::SubroutineIdNode*>(match.value));
                                    if (match.hit)
                                    {
                                        span.Union(lexer.GetSpan(pos));
                                        functionHeading.reset(new p::FunctionHeadingNode(span, subroutineId.release()));
                                    }
                                    *parentMatch8 = match;
                                }
                                *parentMatch7 = match;
                            }
                            *parentMatch5 = match;
                        }
                        *parentMatch4 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch9 = &match;
                        {
                            soul::parser::Match match = ParameterParser<LexerT>::ParameterList(lexer, context, functionHeading.get());
                            *parentMatch9 = match;
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch10 = &match;
                    {
                        soul::parser::Match match(false);
                        if (*lexer == COLON)
                        {
                            ++lexer;
                            match.hit = true;
                        }
                        *parentMatch10 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch11 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch12 = &match;
                    {
                        int64_t pos = lexer.GetPos();
                        soul::parser::Match match = TypeParser<LexerT>::TypeName(lexer, context);
                        resultType.reset(static_cast<p::Node*>(match.value));
                        if (match.hit)
                        {
                            functionHeading->SetResultType(resultType.release());
                        }
                        *parentMatch12 = match;
                    }
                    *parentMatch11 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "FunctionHeading");
                #endif
                return soul::parser::Match(true, functionHeading.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "FunctionHeading");
        else soul::lexer::WriteFailureToLog(lexer, "FunctionHeading");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match SubroutineParser<LexerT>::ConstructorHeading(LexerT& lexer, ParsingContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ConstructorHeading");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 8014466136055742474);
    std::unique_ptr<p::ConstructorHeadingNode> constructorHeading = std::unique_ptr<p::ConstructorHeadingNode>();
    std::unique_ptr<p::IdentifierNode> objectId;
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
                        int64_t pos = lexer.GetPos();
                        soul::parser::Match match(false);
                        if (*lexer == CONSTRUCTOR)
                        {
                            ++lexer;
                            match.hit = true;
                        }
                        if (match.hit)
                        {
                            constructorHeading.reset(new p::ConstructorHeadingNode(lexer.GetSpan(pos)));
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch5 = &match;
                    {
                        soul::parser::Match match(true);
                        int64_t save = lexer.GetPos();
                        soul::parser::Match* parentMatch6 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch7 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch8 = &match;
                                {
                                    int64_t pos = lexer.GetPos();
                                    soul::parser::Match match = CommonParser<LexerT>::Identifier(lexer);
                                    objectId.reset(static_cast<p::IdentifierNode*>(match.value));
                                    if (match.hit)
                                    {
                                        constructorHeading->SetObjectId(objectId.release());
                                    }
                                    *parentMatch8 = match;
                                }
                                *parentMatch7 = match;
                            }
                            if (match.hit)
                            {
                                *parentMatch6 = match;
                            }
                            else
                            {
                                lexer.SetPos(save);
                            }
                        }
                        *parentMatch5 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch9 = &match;
                {
                    soul::parser::Match match = ParameterParser<LexerT>::ParameterList(lexer, context, constructorHeading.get());
                    *parentMatch9 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ConstructorHeading");
                #endif
                return soul::parser::Match(true, constructorHeading.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ConstructorHeading");
        else soul::lexer::WriteFailureToLog(lexer, "ConstructorHeading");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template struct SubroutineParser<soul::lexer::Lexer<p::lexer::PLexer<char>, char>>;

} // namespace p::subroutine::parser
