
// this file has been automatically generated from 'C:/work/p-lang/p/parser/block.parser' using soul parser generator spg version 5.0.0

module p.block.parser;

import util;
import soul.ast.spg;
import p.token;
import p.constant.parser;
import p.type.parser;
import p.variable.parser;
import p.subroutine.parser;
import p.statement.parser;

using namespace p::token;
using namespace p::constant::parser;
using namespace p::type::parser;
using namespace p::variable::parser;
using namespace p::subroutine::parser;
using namespace p::statement::parser;

namespace p::block::parser {

template<typename LexerT>
soul::parser::Match BlockParser<LexerT>::Block(LexerT& lexer, ParsingContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Block");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 7470045152585711617);
    std::unique_ptr<p::BlockNode> block = std::unique_ptr<p::BlockNode>();
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
                        soul::parser::Match match(true);
                        if (match.hit)
                        {
                            block.reset(new p::BlockNode(lexer.GetSpan(pos)));
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
                        soul::parser::Match match = BlockParser<LexerT>::DeclarationPart(lexer, context, block.get());
                        *parentMatch5 = match;
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
                    soul::parser::Match match = BlockParser<LexerT>::StatementPart(lexer, context, block.get());
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
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Block");
                #endif
                return soul::parser::Match(true, block.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Block");
        else soul::lexer::WriteFailureToLog(lexer, "Block");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match BlockParser<LexerT>::SubroutineBlock(LexerT& lexer, ParsingContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "SubroutineBlock");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 7470045152585711618);
    std::unique_ptr<p::BlockNode> block;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t save = lexer.GetPos();
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        switch (*lexer)
        {
            case FORWARD:
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch2 = &match;
                {
                    int64_t pos = lexer.GetPos();
                    soul::parser::Match match(false);
                    if (*lexer == FORWARD)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "SubroutineBlock");
                            #endif
                            return soul::parser::Match(true, new p::ForwardNode(lexer.GetSpan(pos)));
                        }
                    }
                    *parentMatch2 = match;
                }
                if (match.hit)
                {
                    *parentMatch1 = match;
                }
                break;
            }
            case EXTERNAL:
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    int64_t pos = lexer.GetPos();
                    soul::parser::Match match(false);
                    if (*lexer == EXTERNAL)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "SubroutineBlock");
                            #endif
                            return soul::parser::Match(true, new p::ExternalNode(lexer.GetSpan(pos)));
                        }
                    }
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    *parentMatch1 = match;
                }
                break;
            }
        }
        *parentMatch0 = match;
        if (!match.hit)
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch4 = &match;
            lexer.SetPos(save);
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch5 = &match;
                {
                    int64_t pos = lexer.GetPos();
                    soul::parser::Match match = BlockParser<LexerT>::Block(lexer, context);
                    block.reset(static_cast<p::BlockNode*>(match.value));
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "SubroutineBlock");
                            #endif
                            return soul::parser::Match(true, block.release());
                        }
                    }
                    *parentMatch5 = match;
                }
                *parentMatch4 = match;
            }
            *parentMatch0 = match;
        }
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "SubroutineBlock");
        else soul::lexer::WriteFailureToLog(lexer, "SubroutineBlock");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match BlockParser<LexerT>::DeclarationPart(LexerT& lexer, ParsingContext* context, p::BlockNode* block)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "DeclarationPart");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 7470045152585711619);
    soul::parser::Match match(true);
    soul::parser::Match* parentMatch0 = &match;
    {
        while (true)
        {
            int64_t save = lexer.GetPos();
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch1 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch2 = &match;
                    switch (*lexer)
                    {
                        case CONST:
                        {
                            soul::parser::Match match = BlockParser<LexerT>::ConstantDeclarationPart(lexer, context, block);
                            if (match.hit)
                            {
                                *parentMatch2 = match;
                            }
                            break;
                        }
                        case TYPE:
                        {
                            soul::parser::Match match = BlockParser<LexerT>::TypeDeclarationPart(lexer, context, block);
                            if (match.hit)
                            {
                                *parentMatch2 = match;
                            }
                            break;
                        }
                        case VAR:
                        {
                            soul::parser::Match match = BlockParser<LexerT>::VariableDeclarationPart(lexer, context, block);
                            if (match.hit)
                            {
                                *parentMatch2 = match;
                            }
                            break;
                        }
                        case CONSTRUCTOR:
                        case FUNCTION:
                        case PROCEDURE:
                        {
                            soul::parser::Match match = BlockParser<LexerT>::SubroutineDeclarationPart(lexer, context, block);
                            if (match.hit)
                            {
                                *parentMatch2 = match;
                            }
                            break;
                        }
                    }
                    *parentMatch1 = match;
                }
                if (match.hit)
                {
                    *parentMatch0 = match;
                }
                else
                {
                    lexer.SetPos(save);
                    break;
                }
            }
        }
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "DeclarationPart");
        else soul::lexer::WriteFailureToLog(lexer, "DeclarationPart");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match BlockParser<LexerT>::ConstantDeclarationPart(LexerT& lexer, ParsingContext* context, p::BlockNode* block)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ConstantDeclarationPart");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 7470045152585711620);
    std::unique_ptr<p::Node> constantDeclaration;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            if (*lexer == CONST)
            {
                ++lexer;
                match.hit = true;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
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
                            soul::parser::Match match = ConstantParser<LexerT>::ConstantDeclaration(lexer, context);
                            constantDeclaration.reset(static_cast<p::Node*>(match.value));
                            if (match.hit)
                            {
                                block->AddDeclaration(constantDeclaration.release());
                            }
                            *parentMatch5 = match;
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    soul::parser::Match match(true);
                    soul::parser::Match* parentMatch6 = &match;
                    while (true)
                    {
                        int64_t save = lexer.GetPos();
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch7 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch8 = &match;
                                {
                                    int64_t pos = lexer.GetPos();
                                    soul::parser::Match match = ConstantParser<LexerT>::ConstantDeclaration(lexer, context);
                                    constantDeclaration.reset(static_cast<p::Node*>(match.value));
                                    if (match.hit)
                                    {
                                        block->AddDeclaration(constantDeclaration.release());
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
                                break;
                            }
                        }
                    }
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ConstantDeclarationPart");
        else soul::lexer::WriteFailureToLog(lexer, "ConstantDeclarationPart");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match BlockParser<LexerT>::TypeDeclarationPart(LexerT& lexer, ParsingContext* context, p::BlockNode* block)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "TypeDeclarationPart");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 7470045152585711621);
    std::unique_ptr<p::Node> typeDeclaration;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            if (*lexer == TYPE)
            {
                ++lexer;
                match.hit = true;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
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
                            soul::parser::Match match = TypeParser<LexerT>::TypeDeclaration(lexer, context);
                            typeDeclaration.reset(static_cast<p::Node*>(match.value));
                            if (match.hit)
                            {
                                block->AddDeclaration(typeDeclaration.release());
                            }
                            *parentMatch5 = match;
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    soul::parser::Match match(true);
                    soul::parser::Match* parentMatch6 = &match;
                    while (true)
                    {
                        int64_t save = lexer.GetPos();
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch7 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch8 = &match;
                                {
                                    int64_t pos = lexer.GetPos();
                                    soul::parser::Match match = TypeParser<LexerT>::TypeDeclaration(lexer, context);
                                    typeDeclaration.reset(static_cast<p::Node*>(match.value));
                                    if (match.hit)
                                    {
                                        block->AddDeclaration(typeDeclaration.release());
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
                                break;
                            }
                        }
                    }
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TypeDeclarationPart");
        else soul::lexer::WriteFailureToLog(lexer, "TypeDeclarationPart");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match BlockParser<LexerT>::VariableDeclarationPart(LexerT& lexer, ParsingContext* context, p::BlockNode* block)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "VariableDeclarationPart");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 7470045152585711622);
    std::unique_ptr<p::Node> variableDeclaration;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            if (*lexer == VAR)
            {
                ++lexer;
                match.hit = true;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
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
                            soul::parser::Match match = VariableParser<LexerT>::VariableDeclaration(lexer, context);
                            variableDeclaration.reset(static_cast<p::Node*>(match.value));
                            if (match.hit)
                            {
                                block->AddDeclaration(variableDeclaration.release());
                            }
                            *parentMatch5 = match;
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    soul::parser::Match match(true);
                    soul::parser::Match* parentMatch6 = &match;
                    while (true)
                    {
                        int64_t save = lexer.GetPos();
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch7 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch8 = &match;
                                {
                                    int64_t pos = lexer.GetPos();
                                    soul::parser::Match match = VariableParser<LexerT>::VariableDeclaration(lexer, context);
                                    variableDeclaration.reset(static_cast<p::Node*>(match.value));
                                    if (match.hit)
                                    {
                                        block->AddDeclaration(variableDeclaration.release());
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
                                break;
                            }
                        }
                    }
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "VariableDeclarationPart");
        else soul::lexer::WriteFailureToLog(lexer, "VariableDeclarationPart");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match BlockParser<LexerT>::SubroutineDeclarationPart(LexerT& lexer, ParsingContext* context, p::BlockNode* block)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "SubroutineDeclarationPart");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 7470045152585711623);
    std::unique_ptr<p::Node> procedureDeclaration;
    std::unique_ptr<p::Node> functionDeclaration;
    std::unique_ptr<p::Node> constructorDeclaration;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            switch (*lexer)
            {
                case PROCEDURE:
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch3 = &match;
                    {
                        int64_t pos = lexer.GetPos();
                        soul::parser::Match match = SubroutineParser<LexerT>::ProcedureDeclaration(lexer, context);
                        procedureDeclaration.reset(static_cast<p::Node*>(match.value));
                        if (match.hit)
                        {
                            block->AddDeclaration(procedureDeclaration.release());
                        }
                        *parentMatch3 = match;
                    }
                    if (match.hit)
                    {
                        *parentMatch2 = match;
                    }
                    break;
                }
                case FUNCTION:
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch4 = &match;
                    {
                        int64_t pos = lexer.GetPos();
                        soul::parser::Match match = SubroutineParser<LexerT>::FunctionDeclaration(lexer, context);
                        functionDeclaration.reset(static_cast<p::Node*>(match.value));
                        if (match.hit)
                        {
                            block->AddDeclaration(functionDeclaration.release());
                        }
                        *parentMatch4 = match;
                    }
                    if (match.hit)
                    {
                        *parentMatch2 = match;
                    }
                    break;
                }
                case CONSTRUCTOR:
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch5 = &match;
                    {
                        int64_t pos = lexer.GetPos();
                        soul::parser::Match match = SubroutineParser<LexerT>::ConstructorDeclaration(lexer, context);
                        constructorDeclaration.reset(static_cast<p::Node*>(match.value));
                        if (match.hit)
                        {
                            block->AddDeclaration(constructorDeclaration.release());
                        }
                        *parentMatch5 = match;
                    }
                    if (match.hit)
                    {
                        *parentMatch2 = match;
                    }
                    break;
                }
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(true);
        soul::parser::Match* parentMatch6 = &match;
        while (true)
        {
            int64_t save = lexer.GetPos();
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch7 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch8 = &match;
                    switch (*lexer)
                    {
                        case PROCEDURE:
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch9 = &match;
                            {
                                int64_t pos = lexer.GetPos();
                                soul::parser::Match match = SubroutineParser<LexerT>::ProcedureDeclaration(lexer, context);
                                procedureDeclaration.reset(static_cast<p::Node*>(match.value));
                                if (match.hit)
                                {
                                    block->AddDeclaration(procedureDeclaration.release());
                                }
                                *parentMatch9 = match;
                            }
                            if (match.hit)
                            {
                                *parentMatch8 = match;
                            }
                            break;
                        }
                        case FUNCTION:
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch10 = &match;
                            {
                                int64_t pos = lexer.GetPos();
                                soul::parser::Match match = SubroutineParser<LexerT>::FunctionDeclaration(lexer, context);
                                functionDeclaration.reset(static_cast<p::Node*>(match.value));
                                if (match.hit)
                                {
                                    block->AddDeclaration(functionDeclaration.release());
                                }
                                *parentMatch10 = match;
                            }
                            if (match.hit)
                            {
                                *parentMatch8 = match;
                            }
                            break;
                        }
                        case CONSTRUCTOR:
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch11 = &match;
                            {
                                int64_t pos = lexer.GetPos();
                                soul::parser::Match match = SubroutineParser<LexerT>::ConstructorDeclaration(lexer, context);
                                constructorDeclaration.reset(static_cast<p::Node*>(match.value));
                                if (match.hit)
                                {
                                    block->AddDeclaration(constructorDeclaration.release());
                                }
                                *parentMatch11 = match;
                            }
                            if (match.hit)
                            {
                                *parentMatch8 = match;
                            }
                            break;
                        }
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
                    break;
                }
            }
        }
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "SubroutineDeclarationPart");
        else soul::lexer::WriteFailureToLog(lexer, "SubroutineDeclarationPart");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match BlockParser<LexerT>::StatementPart(LexerT& lexer, ParsingContext* context, p::BlockNode* block)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "StatementPart");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 7470045152585711624);
    std::unique_ptr<p::CompoundStatementNode> compoundStatement;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            int64_t pos = lexer.GetPos();
            soul::parser::Match match = StatementParser<LexerT>::CompoundStatement(lexer, context);
            compoundStatement.reset(static_cast<p::CompoundStatementNode*>(match.value));
            if (match.hit)
            {
                block->SetCompoundStatement(compoundStatement.release());
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "StatementPart");
        else soul::lexer::WriteFailureToLog(lexer, "StatementPart");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template struct BlockParser<soul::lexer::Lexer<p::lexer::PLexer<char>, char>>;

} // namespace p::block::parser
