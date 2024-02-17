
// this file has been automatically generated from 'C:/work/p-lang/p/parser/unit.parser' using soul parser generator spg version 5.0.0

module p.unit.parser;

import util;
import soul.ast.spg;
import p.token;
import p.common.parser;
import p.block.parser;
import p.subroutine.parser;

using namespace p::token;
using namespace p::common::parser;
using namespace p::block::parser;
using namespace p::subroutine::parser;

namespace p::unit::parser {

template<typename LexerT>
soul::parser::Match UnitParser<LexerT>::Unit(LexerT& lexer, ParsingContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Unit");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 109874149719015425);
    soul::ast::Span span = soul::ast::Span();
    std::unique_ptr<p::UnitNode> unitNode = std::unique_ptr<p::UnitNode>();
    std::unique_ptr<p::UnitNameNode> unitName;
    std::unique_ptr<p::UnitPartNode> interfacePart;
    std::unique_ptr<p::UnitPartNode> implementationPart;
    std::unique_ptr<p::UnitPartNode> initializationPart;
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
                                        if (*lexer == UNIT)
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
                                                unitNode.reset(new p::UnitNode(span, lexer.FileName(), unitName.release()));
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
                                    soul::parser::Match match(false);
                                    if (*lexer == SEMICOLON)
                                    {
                                        ++lexer;
                                        match.hit = true;
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
                            soul::parser::Match* parentMatch12 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch13 = &match;
                                {
                                    int64_t pos = lexer.GetPos();
                                    soul::parser::Match match = UnitParser<LexerT>::InterfacePart(lexer, context);
                                    interfacePart.reset(static_cast<p::UnitPartNode*>(match.value));
                                    if (match.hit)
                                    {
                                        unitNode->SetInterfacePart(interfacePart.release());
                                    }
                                    *parentMatch13 = match;
                                }
                                *parentMatch12 = match;
                            }
                            *parentMatch5 = match;
                        }
                        *parentMatch4 = match;
                    }
                    if (match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch14 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch15 = &match;
                            {
                                int64_t pos = lexer.GetPos();
                                soul::parser::Match match = UnitParser<LexerT>::ImplementationPart(lexer, context);
                                implementationPart.reset(static_cast<p::UnitPartNode*>(match.value));
                                if (match.hit)
                                {
                                    unitNode->SetImplementationPart(implementationPart.release());
                                }
                                *parentMatch15 = match;
                            }
                            *parentMatch14 = match;
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch16 = &match;
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch17 = &match;
                        {
                            int64_t pos = lexer.GetPos();
                            soul::parser::Match match = UnitParser<LexerT>::InitializationPart(lexer, context);
                            initializationPart.reset(static_cast<p::UnitPartNode*>(match.value));
                            if (match.hit)
                            {
                                unitNode->SetInitializationPart(initializationPart.release());
                            }
                            *parentMatch17 = match;
                        }
                        *parentMatch16 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch18 = &match;
                {
                    soul::parser::Match match(false);
                    if (*lexer == DOT)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    *parentMatch18 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Unit");
                #endif
                return soul::parser::Match(true, unitNode.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Unit");
        else soul::lexer::WriteFailureToLog(lexer, "Unit");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match UnitParser<LexerT>::InterfacePart(LexerT& lexer, ParsingContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "InterfacePart");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 109874149719015426);
    std::unique_ptr<p::UnitPartNode> interfacePart = std::unique_ptr<p::UnitPartNode>();
    std::unique_ptr<p::UnitNameListNode> usesList;
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
                        if (*lexer == INTERFACE)
                        {
                            ++lexer;
                            match.hit = true;
                        }
                        if (match.hit)
                        {
                            interfacePart.reset(new p::UnitPartNode(lexer.GetSpan(pos), p::UnitPartKind::interfacePart));
                            interfacePart->SetBlock(new p::BlockNode(lexer.GetSpan(pos)));
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
                                    soul::parser::Match match = CommonParser<LexerT>::UsesClause(lexer);
                                    usesList.reset(static_cast<p::UnitNameListNode*>(match.value));
                                    if (match.hit)
                                    {
                                        interfacePart->SetUsesList(usesList.release());
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
                    soul::parser::Match match(true);
                    soul::parser::Match* parentMatch10 = &match;
                    {
                        while (true)
                        {
                            int64_t save = lexer.GetPos();
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch11 = &match;
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch12 = &match;
                                    switch (*lexer)
                                    {
                                        case CONST:
                                        {
                                            soul::parser::Match match = BlockParser<LexerT>::ConstantDeclarationPart(lexer, context, interfacePart->Block());
                                            if (match.hit)
                                            {
                                                *parentMatch12 = match;
                                            }
                                            break;
                                        }
                                        case TYPE:
                                        {
                                            soul::parser::Match match = BlockParser<LexerT>::TypeDeclarationPart(lexer, context, interfacePart->Block());
                                            if (match.hit)
                                            {
                                                *parentMatch12 = match;
                                            }
                                            break;
                                        }
                                        case VAR:
                                        {
                                            soul::parser::Match match = BlockParser<LexerT>::VariableDeclarationPart(lexer, context, interfacePart->Block());
                                            if (match.hit)
                                            {
                                                *parentMatch12 = match;
                                            }
                                            break;
                                        }
                                        case FUNCTION:
                                        case PROCEDURE:
                                        {
                                            soul::parser::Match match = UnitParser<LexerT>::ProcedureAndFunctionHeadingPart(lexer, context, interfacePart->Block());
                                            if (match.hit)
                                            {
                                                *parentMatch12 = match;
                                            }
                                            break;
                                        }
                                    }
                                    *parentMatch11 = match;
                                }
                                if (match.hit)
                                {
                                    *parentMatch10 = match;
                                }
                                else
                                {
                                    lexer.SetPos(save);
                                    break;
                                }
                            }
                        }
                    }
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
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "InterfacePart");
                #endif
                return soul::parser::Match(true, interfacePart.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "InterfacePart");
        else soul::lexer::WriteFailureToLog(lexer, "InterfacePart");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match UnitParser<LexerT>::ProcedureAndFunctionHeadingPart(LexerT& lexer, ParsingContext* context, p::BlockNode* block)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ProcedureAndFunctionHeadingPart");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 109874149719015427);
    std::unique_ptr<p::ProcedureHeadingNode> procedureHeading;
    std::unique_ptr<p::FunctionHeadingNode> functionHeading;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                switch (*lexer)
                {
                    case PROCEDURE:
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch4 = &match;
                        {
                            int64_t pos = lexer.GetPos();
                            soul::parser::Match match = SubroutineParser<LexerT>::ProcedureHeading(lexer, context);
                            procedureHeading.reset(static_cast<p::ProcedureHeadingNode*>(match.value));
                            if (match.hit)
                            {
                                block->AddDeclaration(procedureHeading.release());
                            }
                            *parentMatch4 = match;
                        }
                        if (match.hit)
                        {
                            *parentMatch3 = match;
                        }
                        break;
                    }
                    case FUNCTION:
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch5 = &match;
                        {
                            int64_t pos = lexer.GetPos();
                            soul::parser::Match match = SubroutineParser<LexerT>::FunctionHeading(lexer, context);
                            functionHeading.reset(static_cast<p::FunctionHeadingNode*>(match.value));
                            if (match.hit)
                            {
                                block->AddDeclaration(functionHeading.release());
                            }
                            *parentMatch5 = match;
                        }
                        if (match.hit)
                        {
                            *parentMatch3 = match;
                        }
                        break;
                    }
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
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
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ProcedureAndFunctionHeadingPart");
        else soul::lexer::WriteFailureToLog(lexer, "ProcedureAndFunctionHeadingPart");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match UnitParser<LexerT>::ImplementationPart(LexerT& lexer, ParsingContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ImplementationPart");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 109874149719015428);
    std::unique_ptr<p::UnitPartNode> implementationPart = std::unique_ptr<p::UnitPartNode>();
    std::unique_ptr<p::UnitNameListNode> usesList;
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
                        if (*lexer == IMPLEMENTATION)
                        {
                            ++lexer;
                            match.hit = true;
                        }
                        if (match.hit)
                        {
                            implementationPart.reset(new p::UnitPartNode(lexer.GetSpan(pos), p::UnitPartKind::implementationPart));
                            implementationPart->SetBlock(new p::BlockNode(lexer.GetSpan(pos)));
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
                                    soul::parser::Match match = CommonParser<LexerT>::UsesClause(lexer);
                                    usesList.reset(static_cast<p::UnitNameListNode*>(match.value));
                                    if (match.hit)
                                    {
                                        implementationPart->SetUsesList(usesList.release());
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
                    soul::parser::Match match = BlockParser<LexerT>::DeclarationPart(lexer, context, implementationPart->Block());
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
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ImplementationPart");
                #endif
                return soul::parser::Match(true, implementationPart.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ImplementationPart");
        else soul::lexer::WriteFailureToLog(lexer, "ImplementationPart");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match UnitParser<LexerT>::InitializationPart(LexerT& lexer, ParsingContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "InitializationPart");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 109874149719015429);
    std::unique_ptr<p::UnitPartNode> initializationPart = std::unique_ptr<p::UnitPartNode>();
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            int64_t pos = lexer.GetPos();
            soul::parser::Match match(true);
            if (match.hit)
            {
                initializationPart.reset(new p::UnitPartNode(lexer.GetSpan(pos), p::UnitPartKind::initializationPart));
                initializationPart->SetBlock(new p::BlockNode(lexer.GetSpan(pos)));
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch2 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch3 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch4 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch5 = &match;
                    switch (*lexer)
                    {
                        case END:
                        {
                            soul::parser::Match match(false);
                            if (*lexer == END)
                            {
                                ++lexer;
                                match.hit = true;
                            }
                            if (match.hit)
                            {
                                *parentMatch5 = match;
                            }
                            break;
                        }
                        case BEGIN:
                        {
                            soul::parser::Match match = BlockParser<LexerT>::StatementPart(lexer, context, initializationPart->Block());
                            if (match.hit)
                            {
                                *parentMatch5 = match;
                            }
                            break;
                        }
                    }
                    *parentMatch4 = match;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "InitializationPart");
                        #endif
                        return soul::parser::Match(true, initializationPart.release());
                    }
                }
                *parentMatch3 = match;
            }
            *parentMatch2 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "InitializationPart");
        else soul::lexer::WriteFailureToLog(lexer, "InitializationPart");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template struct UnitParser<soul::lexer::Lexer<p::lexer::PLexer<char>, char>>;

} // namespace p::unit::parser
