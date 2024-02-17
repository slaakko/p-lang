
// this file has been automatically generated from 'C:/work/p-lang/p/parser/type.parser' using soul parser generator spg version 5.0.0

module p.type.parser;

import util;
import soul.ast.spg;
import p.token;
import p.common.parser;
import p.expression.parser;
import p.subroutine.parser;

using namespace p::token;
using namespace p::common::parser;
using namespace p::expression::parser;
using namespace p::subroutine::parser;

namespace p::type::parser {

template<typename LexerT>
soul::parser::Match TypeParser<LexerT>::TypeName(LexerT& lexer, ParsingContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "TypeName");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 1060958820211097601);
    std::unique_ptr<p::IdentifierNode> identifier;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    switch (*lexer)
    {
        case INTEGER:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch1 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == INTEGER)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TypeName");
                        #endif
                        return soul::parser::Match(true, new p::IntegerNode(lexer.GetSpan(pos)));
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
        case REAL:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == REAL)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TypeName");
                        #endif
                        return soul::parser::Match(true, new p::RealNode(lexer.GetSpan(pos)));
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
        case CHAR:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch3 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == CHAR)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TypeName");
                        #endif
                        return soul::parser::Match(true, new p::CharNode(lexer.GetSpan(pos)));
                    }
                }
                *parentMatch3 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case BOOLEAN:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch4 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == BOOLEAN)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TypeName");
                        #endif
                        return soul::parser::Match(true, new p::BooleanNode(lexer.GetSpan(pos)));
                    }
                }
                *parentMatch4 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case STRING:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch5 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == STRING)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TypeName");
                        #endif
                        return soul::parser::Match(true, new p::StringNode(lexer.GetSpan(pos)));
                    }
                }
                *parentMatch5 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case POINTER:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch6 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == POINTER)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TypeName");
                        #endif
                        return soul::parser::Match(true, new p::PointerNode(lexer.GetSpan(pos)));
                    }
                }
                *parentMatch6 = match;
            }
            if (match.hit)
            {
                *parentMatch0 = match;
            }
            break;
        }
        case ID:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch7 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match = CommonParser<LexerT>::Identifier(lexer);
                identifier.reset(static_cast<p::IdentifierNode*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TypeName");
                        #endif
                        return soul::parser::Match(true, identifier.release());
                    }
                }
                *parentMatch7 = match;
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TypeName");
        else soul::lexer::WriteFailureToLog(lexer, "TypeName");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match TypeParser<LexerT>::TypeDeclaration(LexerT& lexer, ParsingContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "TypeDeclaration");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 1060958820211097602);
    soul::ast::Span span = soul::ast::Span();
    std::unique_ptr<p::IdentifierNode> typeId;
    std::unique_ptr<p::Node> type;
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
                            soul::parser::Match match = CommonParser<LexerT>::Identifier(lexer);
                            typeId.reset(static_cast<p::IdentifierNode*>(match.value));
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
                            if (*lexer == EQUAL)
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
                        soul::parser::Match match = TypeParser<LexerT>::Type(lexer, context);
                        type.reset(static_cast<p::Node*>(match.value));
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
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TypeDeclaration");
                #endif
                return soul::parser::Match(true, new p::TypeDeclarationNode(span, typeId.release(), type.release()));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "TypeDeclaration");
        else soul::lexer::WriteFailureToLog(lexer, "TypeDeclaration");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match TypeParser<LexerT>::Type(LexerT& lexer, ParsingContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Type");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 1060958820211097603);
    std::unique_ptr<p::Node> simpleType;
    std::unique_ptr<p::Node> stringType;
    std::unique_ptr<p::Node> pointerType;
    std::unique_ptr<p::Node> objectType;
    std::unique_ptr<p::Node> arrayType;
    std::unique_ptr<p::Node> specialization;
    std::unique_ptr<p::IdentifierNode> typeIdentifier;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t save = lexer.GetPos();
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            int64_t save = lexer.GetPos();
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                int64_t save = lexer.GetPos();
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    int64_t save = lexer.GetPos();
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch4 = &match;
                    {
                        int64_t save = lexer.GetPos();
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch5 = &match;
                        {
                            int64_t save = lexer.GetPos();
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch6 = &match;
                            {
                                int64_t pos = lexer.GetPos();
                                soul::parser::Match match = TypeParser<LexerT>::SimpleType(lexer, context);
                                simpleType.reset(static_cast<p::Node*>(match.value));
                                if (match.hit)
                                {
                                    {
                                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Type");
                                        #endif
                                        return soul::parser::Match(true, simpleType.release());
                                    }
                                }
                                *parentMatch6 = match;
                            }
                            *parentMatch5 = match;
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
                                        soul::parser::Match match = TypeParser<LexerT>::StringType(lexer, context);
                                        stringType.reset(static_cast<p::Node*>(match.value));
                                        if (match.hit)
                                        {
                                            {
                                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Type");
                                                #endif
                                                return soul::parser::Match(true, stringType.release());
                                            }
                                        }
                                        *parentMatch8 = match;
                                    }
                                    *parentMatch7 = match;
                                }
                                *parentMatch5 = match;
                            }
                        }
                        *parentMatch4 = match;
                        if (!match.hit)
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch9 = &match;
                            lexer.SetPos(save);
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch10 = &match;
                                {
                                    int64_t pos = lexer.GetPos();
                                    soul::parser::Match match = TypeParser<LexerT>::PointerType(lexer, context);
                                    pointerType.reset(static_cast<p::Node*>(match.value));
                                    if (match.hit)
                                    {
                                        {
                                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Type");
                                            #endif
                                            return soul::parser::Match(true, pointerType.release());
                                        }
                                    }
                                    *parentMatch10 = match;
                                }
                                *parentMatch9 = match;
                            }
                            *parentMatch4 = match;
                        }
                    }
                    *parentMatch3 = match;
                    if (!match.hit)
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch11 = &match;
                        lexer.SetPos(save);
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch12 = &match;
                            {
                                int64_t pos = lexer.GetPos();
                                soul::parser::Match match = TypeParser<LexerT>::ObjectType(lexer, context);
                                objectType.reset(static_cast<p::Node*>(match.value));
                                if (match.hit)
                                {
                                    {
                                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Type");
                                        #endif
                                        return soul::parser::Match(true, objectType.release());
                                    }
                                }
                                *parentMatch12 = match;
                            }
                            *parentMatch11 = match;
                        }
                        *parentMatch3 = match;
                    }
                }
                *parentMatch2 = match;
                if (!match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch13 = &match;
                    lexer.SetPos(save);
                    {
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch14 = &match;
                        {
                            int64_t pos = lexer.GetPos();
                            soul::parser::Match match = TypeParser<LexerT>::ArrayType(lexer, context);
                            arrayType.reset(static_cast<p::Node*>(match.value));
                            if (match.hit)
                            {
                                {
                                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Type");
                                    #endif
                                    return soul::parser::Match(true, arrayType.release());
                                }
                            }
                            *parentMatch14 = match;
                        }
                        *parentMatch13 = match;
                    }
                    *parentMatch2 = match;
                }
            }
            *parentMatch1 = match;
            if (!match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch15 = &match;
                lexer.SetPos(save);
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch16 = &match;
                    {
                        int64_t pos = lexer.GetPos();
                        soul::parser::Match match = TypeParser<LexerT>::Specialization(lexer, context);
                        specialization.reset(static_cast<p::Node*>(match.value));
                        if (match.hit)
                        {
                            {
                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Type");
                                #endif
                                return soul::parser::Match(true, specialization.release());
                            }
                        }
                        *parentMatch16 = match;
                    }
                    *parentMatch15 = match;
                }
                *parentMatch1 = match;
            }
        }
        *parentMatch0 = match;
        if (!match.hit)
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch17 = &match;
            lexer.SetPos(save);
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch18 = &match;
                {
                    int64_t pos = lexer.GetPos();
                    soul::parser::Match match = CommonParser<LexerT>::Identifier(lexer);
                    typeIdentifier.reset(static_cast<p::IdentifierNode*>(match.value));
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Type");
                            #endif
                            return soul::parser::Match(true, typeIdentifier.release());
                        }
                    }
                    *parentMatch18 = match;
                }
                *parentMatch17 = match;
            }
            *parentMatch0 = match;
        }
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Type");
        else soul::lexer::WriteFailureToLog(lexer, "Type");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match TypeParser<LexerT>::SimpleType(LexerT& lexer, ParsingContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "SimpleType");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 1060958820211097604);
    std::unique_ptr<p::Node> ordinalType;
    std::unique_ptr<p::Node> realType;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t save = lexer.GetPos();
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            int64_t pos = lexer.GetPos();
            soul::parser::Match match = TypeParser<LexerT>::OrdinalType(lexer, context);
            ordinalType.reset(static_cast<p::Node*>(match.value));
            if (match.hit)
            {
                {
                    #ifdef SOUL_PARSER_DEBUG_SUPPORT
                    if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "SimpleType");
                    #endif
                    return soul::parser::Match(true, ordinalType.release());
                }
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
        if (!match.hit)
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            lexer.SetPos(save);
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    int64_t pos = lexer.GetPos();
                    soul::parser::Match match = TypeParser<LexerT>::RealType(lexer, context);
                    realType.reset(static_cast<p::Node*>(match.value));
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "SimpleType");
                            #endif
                            return soul::parser::Match(true, realType.release());
                        }
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch0 = match;
        }
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "SimpleType");
        else soul::lexer::WriteFailureToLog(lexer, "SimpleType");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match TypeParser<LexerT>::OrdinalType(LexerT& lexer, ParsingContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "OrdinalType");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 1060958820211097605);
    std::unique_ptr<p::Node> subrangeType;
    std::unique_ptr<p::Node> enumeratedType;
    std::unique_ptr<p::Node> ordinalTypeName;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t save = lexer.GetPos();
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            int64_t save = lexer.GetPos();
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match = TypeParser<LexerT>::SubrangeType(lexer, context);
                subrangeType.reset(static_cast<p::Node*>(match.value));
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OrdinalType");
                        #endif
                        return soul::parser::Match(true, subrangeType.release());
                    }
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
            if (!match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                lexer.SetPos(save);
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch4 = &match;
                    {
                        int64_t pos = lexer.GetPos();
                        soul::parser::Match match = TypeParser<LexerT>::EnumeratedType(lexer, context);
                        enumeratedType.reset(static_cast<p::Node*>(match.value));
                        if (match.hit)
                        {
                            {
                                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OrdinalType");
                                #endif
                                return soul::parser::Match(true, enumeratedType.release());
                            }
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch1 = match;
            }
        }
        *parentMatch0 = match;
        if (!match.hit)
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch5 = &match;
            lexer.SetPos(save);
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch6 = &match;
                {
                    int64_t pos = lexer.GetPos();
                    soul::parser::Match match = TypeParser<LexerT>::OrdinalTypeName(lexer, context);
                    ordinalTypeName.reset(static_cast<p::Node*>(match.value));
                    if (match.hit)
                    {
                        {
                            #ifdef SOUL_PARSER_DEBUG_SUPPORT
                            if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OrdinalType");
                            #endif
                            return soul::parser::Match(true, ordinalTypeName.release());
                        }
                    }
                    *parentMatch6 = match;
                }
                *parentMatch5 = match;
            }
            *parentMatch0 = match;
        }
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OrdinalType");
        else soul::lexer::WriteFailureToLog(lexer, "OrdinalType");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match TypeParser<LexerT>::SubrangeType(LexerT& lexer, ParsingContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "SubrangeType");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 1060958820211097606);
    soul::ast::Span span = soul::ast::Span();
    std::unique_ptr<p::Node> rangeStart;
    std::unique_ptr<p::Node> rangeEnd;
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
                        soul::parser::Match match = ExpressionParser<LexerT>::ConstantExpression(lexer, context);
                        rangeStart.reset(static_cast<p::Node*>(match.value));
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
                        if (*lexer == DOTDOT)
                        {
                            ++lexer;
                            match.hit = true;
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
                soul::parser::Match* parentMatch6 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch7 = &match;
                    {
                        int64_t pos = lexer.GetPos();
                        soul::parser::Match match = ExpressionParser<LexerT>::ConstantExpression(lexer, context);
                        rangeEnd.reset(static_cast<p::Node*>(match.value));
                        if (match.hit)
                        {
                            span.Union(lexer.GetSpan(pos));
                        }
                        *parentMatch7 = match;
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
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "SubrangeType");
                #endif
                return soul::parser::Match(true, new p::SubrangeTypeNode(span, rangeStart.release(), rangeEnd.release()));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "SubrangeType");
        else soul::lexer::WriteFailureToLog(lexer, "SubrangeType");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match TypeParser<LexerT>::EnumeratedType(LexerT& lexer, ParsingContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "EnumeratedType");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 1060958820211097607);
    soul::ast::Span span = soul::ast::Span();
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
                    soul::parser::Match* parentMatch4 = &match;
                    {
                        int64_t pos = lexer.GetPos();
                        soul::parser::Match match(false);
                        if (*lexer == LPAREN)
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
                        soul::parser::Match match = CommonParser<LexerT>::IdentifierList(lexer);
                        identifierList.reset(static_cast<p::IdentifierListNode*>(match.value));
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
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch7 = &match;
                    {
                        int64_t pos = lexer.GetPos();
                        soul::parser::Match match(false);
                        if (*lexer == RPAREN)
                        {
                            ++lexer;
                            match.hit = true;
                        }
                        if (match.hit)
                        {
                            span.Union(lexer.GetSpan(pos));
                        }
                        *parentMatch7 = match;
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
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "EnumeratedType");
                #endif
                return soul::parser::Match(true, new p::EnumeratedTypeNode(span, identifierList.release()));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "EnumeratedType");
        else soul::lexer::WriteFailureToLog(lexer, "EnumeratedType");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match TypeParser<LexerT>::OrdinalTypeName(LexerT& lexer, ParsingContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "OrdinalTypeName");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 1060958820211097608);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    switch (*lexer)
    {
        case INTEGER:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch1 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == INTEGER)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OrdinalTypeName");
                        #endif
                        return soul::parser::Match(true, new p::IntegerNode(lexer.GetSpan(pos)));
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
        case BOOLEAN:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == BOOLEAN)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OrdinalTypeName");
                        #endif
                        return soul::parser::Match(true, new p::BooleanNode(lexer.GetSpan(pos)));
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
        case CHAR:
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch3 = &match;
            {
                int64_t pos = lexer.GetPos();
                soul::parser::Match match(false);
                if (*lexer == CHAR)
                {
                    ++lexer;
                    match.hit = true;
                }
                if (match.hit)
                {
                    {
                        #ifdef SOUL_PARSER_DEBUG_SUPPORT
                        if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OrdinalTypeName");
                        #endif
                        return soul::parser::Match(true, new p::CharNode(lexer.GetSpan(pos)));
                    }
                }
                *parentMatch3 = match;
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "OrdinalTypeName");
        else soul::lexer::WriteFailureToLog(lexer, "OrdinalTypeName");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match TypeParser<LexerT>::RealType(LexerT& lexer, ParsingContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "RealType");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 1060958820211097609);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        if (*lexer == REAL)
        {
            ++lexer;
            match.hit = true;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "RealType");
                #endif
                return soul::parser::Match(true, new p::RealNode(lexer.GetSpan(pos)));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "RealType");
        else soul::lexer::WriteFailureToLog(lexer, "RealType");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match TypeParser<LexerT>::StringType(LexerT& lexer, ParsingContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "StringType");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 1060958820211097610);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        if (*lexer == STRING)
        {
            ++lexer;
            match.hit = true;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "StringType");
                #endif
                return soul::parser::Match(true, new p::StringNode(lexer.GetSpan(pos)));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "StringType");
        else soul::lexer::WriteFailureToLog(lexer, "StringType");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match TypeParser<LexerT>::PointerType(LexerT& lexer, ParsingContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "PointerType");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 1060958820211097611);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        int64_t pos = lexer.GetPos();
        soul::parser::Match match(false);
        if (*lexer == POINTER)
        {
            ++lexer;
            match.hit = true;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "PointerType");
                #endif
                return soul::parser::Match(true, new p::PointerNode(lexer.GetSpan(pos)));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "PointerType");
        else soul::lexer::WriteFailureToLog(lexer, "PointerType");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match TypeParser<LexerT>::ObjectType(LexerT& lexer, ParsingContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ObjectType");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 1060958820211097612);
    std::unique_ptr<p::ObjectTypeNode> node = std::unique_ptr<p::ObjectTypeNode>();
    std::unique_ptr<p::IdentifierNode> heritage;
    std::unique_ptr<p::IdentifierNode> typeParamId;
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
                                if (*lexer == OBJECT)
                                {
                                    ++lexer;
                                    match.hit = true;
                                }
                                if (match.hit)
                                {
                                    node.reset(new p::ObjectTypeNode(lexer.GetSpan(pos)));
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
                                soul::parser::Match match(true);
                                int64_t save = lexer.GetPos();
                                soul::parser::Match* parentMatch8 = &match;
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch9 = &match;
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch10 = &match;
                                        {
                                            int64_t pos = lexer.GetPos();
                                            soul::parser::Match match = TypeParser<LexerT>::Heritage(lexer);
                                            heritage.reset(static_cast<p::IdentifierNode*>(match.value));
                                            if (match.hit)
                                            {
                                                node->SetHeritage(heritage.release());
                                            }
                                            *parentMatch10 = match;
                                        }
                                        *parentMatch9 = match;
                                    }
                                    if (match.hit)
                                    {
                                        *parentMatch8 = match;
                                    }
                                    else
                                    {
                                        lexer.SetPos(save);
                                    }
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
                                        soul::parser::Match match = TypeParser<LexerT>::GenericPart(lexer);
                                        typeParamId.reset(static_cast<p::IdentifierNode*>(match.value));
                                        if (match.hit)
                                        {
                                            node->SetTypeParamId(typeParamId.release());
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
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch15 = &match;
                    {
                        soul::parser::Match match = TypeParser<LexerT>::ComponentList(lexer, context, node.get());
                        *parentMatch15 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch16 = &match;
                {
                    soul::parser::Match match(false);
                    if (*lexer == END)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    *parentMatch16 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ObjectType");
                #endif
                return soul::parser::Match(true, node.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ObjectType");
        else soul::lexer::WriteFailureToLog(lexer, "ObjectType");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match TypeParser<LexerT>::Heritage(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Heritage");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 1060958820211097613);
    std::unique_ptr<p::IdentifierNode> objectTypeIdentifier;
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
                        soul::parser::Match match = CommonParser<LexerT>::Identifier(lexer);
                        objectTypeIdentifier.reset(static_cast<p::IdentifierNode*>(match.value));
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch5 = &match;
                {
                    soul::parser::Match match(false);
                    if (*lexer == RPAREN)
                    {
                        ++lexer;
                        match.hit = true;
                    }
                    *parentMatch5 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Heritage");
                #endif
                return soul::parser::Match(true, objectTypeIdentifier.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Heritage");
        else soul::lexer::WriteFailureToLog(lexer, "Heritage");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match TypeParser<LexerT>::GenericPart(LexerT& lexer)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "GenericPart");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 1060958820211097614);
    std::unique_ptr<p::IdentifierNode> typeParamId;
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
                if (*lexer == OF)
                {
                    ++lexer;
                    match.hit = true;
                }
                *parentMatch2 = match;
            }
            if (match.hit)
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch3 = &match;
                {
                    soul::parser::Match match = CommonParser<LexerT>::Identifier(lexer);
                    typeParamId.reset(static_cast<p::IdentifierNode*>(match.value));
                    *parentMatch3 = match;
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            {
                #ifdef SOUL_PARSER_DEBUG_SUPPORT
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "GenericPart");
                #endif
                return soul::parser::Match(true, typeParamId.release());
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "GenericPart");
        else soul::lexer::WriteFailureToLog(lexer, "GenericPart");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match TypeParser<LexerT>::ComponentList(LexerT& lexer, ParsingContext* context, ObjectTypeNode* objectType)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ComponentList");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 1060958820211097615);
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(true);
            int64_t save = lexer.GetPos();
            soul::parser::Match* parentMatch2 = &match;
            {
                soul::parser::Match match = TypeParser<LexerT>::ObjectFieldList(lexer, context, objectType);
                if (match.hit)
                {
                    *parentMatch2 = match;
                }
                else
                {
                    lexer.SetPos(save);
                }
            }
            *parentMatch1 = match;
        }
        if (match.hit)
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch3 = &match;
            {
                soul::parser::Match match(true);
                int64_t save = lexer.GetPos();
                soul::parser::Match* parentMatch4 = &match;
                {
                    soul::parser::Match match = TypeParser<LexerT>::MethodList(lexer, context, objectType);
                    if (match.hit)
                    {
                        *parentMatch4 = match;
                    }
                    else
                    {
                        lexer.SetPos(save);
                    }
                }
                *parentMatch3 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ComponentList");
        else soul::lexer::WriteFailureToLog(lexer, "ComponentList");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match TypeParser<LexerT>::ObjectFieldList(LexerT& lexer, ParsingContext* context, ObjectTypeNode* objectType)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ObjectFieldList");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 1060958820211097616);
    soul::ast::Span span = soul::ast::Span();
    std::unique_ptr<p::IdentifierListNode> identifierList;
    std::unique_ptr<p::Node> typeName;
    soul::parser::Match match(false);
    soul::parser::Match* parentMatch0 = &match;
    {
        soul::parser::Match match(false);
        soul::parser::Match* parentMatch1 = &match;
        {
            soul::parser::Match match(false);
            soul::parser::Match* parentMatch2 = &match;
            {
                int64_t pos = lexer.GetPos();
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
                                    int64_t pos = lexer.GetPos();
                                    soul::parser::Match match = CommonParser<LexerT>::IdentifierList(lexer);
                                    identifierList.reset(static_cast<p::IdentifierListNode*>(match.value));
                                    if (match.hit)
                                    {
                                        span = lexer.GetSpan(pos);
                                    }
                                    *parentMatch7 = match;
                                }
                                *parentMatch6 = match;
                            }
                            if (match.hit)
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch8 = &match;
                                {
                                    soul::parser::Match match(false);
                                    if (*lexer == COLON)
                                    {
                                        ++lexer;
                                        match.hit = true;
                                    }
                                    *parentMatch8 = match;
                                }
                                *parentMatch6 = match;
                            }
                            *parentMatch5 = match;
                        }
                        if (match.hit)
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch9 = &match;
                            {
                                soul::parser::Match match = TypeParser<LexerT>::TypeName(lexer, context);
                                typeName.reset(static_cast<p::Node*>(match.value));
                                *parentMatch9 = match;
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
                            soul::parser::Match* parentMatch11 = &match;
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
                                *parentMatch11 = match;
                            }
                            *parentMatch10 = match;
                        }
                        *parentMatch4 = match;
                    }
                    *parentMatch3 = match;
                }
                if (match.hit)
                {
                    objectType->AddFields(span, identifierList.release(), typeName.release());
                }
                *parentMatch2 = match;
            }
            *parentMatch1 = match;
        }
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(true);
        soul::parser::Match* parentMatch12 = &match;
        while (true)
        {
            int64_t save = lexer.GetPos();
            {
                soul::parser::Match match(false);
                soul::parser::Match* parentMatch13 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch14 = &match;
                    {
                        int64_t pos = lexer.GetPos();
                        soul::parser::Match match(false);
                        soul::parser::Match* parentMatch15 = &match;
                        {
                            soul::parser::Match match(false);
                            soul::parser::Match* parentMatch16 = &match;
                            {
                                soul::parser::Match match(false);
                                soul::parser::Match* parentMatch17 = &match;
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch18 = &match;
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch19 = &match;
                                        {
                                            int64_t pos = lexer.GetPos();
                                            soul::parser::Match match = CommonParser<LexerT>::IdentifierList(lexer);
                                            identifierList.reset(static_cast<p::IdentifierListNode*>(match.value));
                                            if (match.hit)
                                            {
                                                span = lexer.GetSpan(pos);
                                            }
                                            *parentMatch19 = match;
                                        }
                                        *parentMatch18 = match;
                                    }
                                    if (match.hit)
                                    {
                                        soul::parser::Match match(false);
                                        soul::parser::Match* parentMatch20 = &match;
                                        {
                                            soul::parser::Match match(false);
                                            if (*lexer == COLON)
                                            {
                                                ++lexer;
                                                match.hit = true;
                                            }
                                            *parentMatch20 = match;
                                        }
                                        *parentMatch18 = match;
                                    }
                                    *parentMatch17 = match;
                                }
                                if (match.hit)
                                {
                                    soul::parser::Match match(false);
                                    soul::parser::Match* parentMatch21 = &match;
                                    {
                                        soul::parser::Match match = TypeParser<LexerT>::TypeName(lexer, context);
                                        typeName.reset(static_cast<p::Node*>(match.value));
                                        *parentMatch21 = match;
                                    }
                                    *parentMatch17 = match;
                                }
                                *parentMatch16 = match;
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
                                        *parentMatch23 = match;
                                    }
                                    *parentMatch22 = match;
                                }
                                *parentMatch16 = match;
                            }
                            *parentMatch15 = match;
                        }
                        if (match.hit)
                        {
                            objectType->AddFields(span, identifierList.release(), typeName.release());
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
                    break;
                }
            }
        }
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ObjectFieldList");
        else soul::lexer::WriteFailureToLog(lexer, "ObjectFieldList");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match TypeParser<LexerT>::MethodList(LexerT& lexer, ParsingContext* context, ObjectTypeNode* objectType)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "MethodList");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 1060958820211097617);
    std::unique_ptr<p::SubroutineHeadingNode> methodHeading;
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
                {
                    int64_t pos = lexer.GetPos();
                    soul::parser::Match match = SubroutineParser<LexerT>::MethodHeading(lexer, context);
                    methodHeading.reset(static_cast<p::SubroutineHeadingNode*>(match.value));
                    if (match.hit)
                    {
                        objectType->AddMethod(methodHeading.release());
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
                    if (*lexer == SEMICOLON)
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
        *parentMatch0 = match;
    }
    if (match.hit)
    {
        soul::parser::Match match(true);
        soul::parser::Match* parentMatch5 = &match;
        while (true)
        {
            int64_t save = lexer.GetPos();
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
                            soul::parser::Match match = SubroutineParser<LexerT>::MethodHeading(lexer, context);
                            methodHeading.reset(static_cast<p::SubroutineHeadingNode*>(match.value));
                            if (match.hit)
                            {
                                objectType->AddMethod(methodHeading.release());
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
                            if (*lexer == SEMICOLON)
                            {
                                ++lexer;
                                match.hit = true;
                            }
                            *parentMatch9 = match;
                        }
                        *parentMatch7 = match;
                    }
                    *parentMatch6 = match;
                }
                if (match.hit)
                {
                    *parentMatch5 = match;
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
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "MethodList");
        else soul::lexer::WriteFailureToLog(lexer, "MethodList");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match TypeParser<LexerT>::ArrayType(LexerT& lexer, ParsingContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "ArrayType");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 1060958820211097618);
    soul::ast::Span span = soul::ast::Span();
    std::unique_ptr<p::Node> elementType;
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
                        if (*lexer == ARRAY)
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
                        if (*lexer == OF)
                        {
                            ++lexer;
                            match.hit = true;
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
                soul::parser::Match* parentMatch6 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch7 = &match;
                    {
                        int64_t pos = lexer.GetPos();
                        soul::parser::Match match = TypeParser<LexerT>::TypeName(lexer, context);
                        elementType.reset(static_cast<p::Node*>(match.value));
                        if (match.hit)
                        {
                            span.Union(lexer.GetSpan(pos));
                        }
                        *parentMatch7 = match;
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
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ArrayType");
                #endif
                return soul::parser::Match(true, new p::ArrayTypeNode(span, elementType.release()));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "ArrayType");
        else soul::lexer::WriteFailureToLog(lexer, "ArrayType");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template<typename LexerT>
soul::parser::Match TypeParser<LexerT>::Specialization(LexerT& lexer, ParsingContext* context)
{
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    int64_t parser_debug_match_pos = 0;
    bool parser_debug_write_to_log = lexer.Log() != nullptr;
    if (parser_debug_write_to_log)
    {
        parser_debug_match_pos = lexer.GetPos();
        soul::lexer::WriteBeginRuleToLog(lexer, "Specialization");
    }
    #endif
    soul::lexer::RuleGuard<LexerT> ruleGuard(lexer, 1060958820211097619);
    soul::ast::Span span = soul::ast::Span();
    std::unique_ptr<p::IdentifierNode> genericId;
    std::unique_ptr<p::Node> type;
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
                        soul::parser::Match match = CommonParser<LexerT>::Identifier(lexer);
                        genericId.reset(static_cast<p::IdentifierNode*>(match.value));
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
                        if (*lexer == OF)
                        {
                            ++lexer;
                            match.hit = true;
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
                soul::parser::Match* parentMatch6 = &match;
                {
                    soul::parser::Match match(false);
                    soul::parser::Match* parentMatch7 = &match;
                    {
                        int64_t pos = lexer.GetPos();
                        soul::parser::Match match = TypeParser<LexerT>::TypeName(lexer, context);
                        type.reset(static_cast<p::Node*>(match.value));
                        if (match.hit)
                        {
                            span.Union(lexer.GetSpan(pos));
                        }
                        *parentMatch7 = match;
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
                if (parser_debug_write_to_log) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Specialization");
                #endif
                return soul::parser::Match(true, new p::SpecializationNode(span, genericId.release(), type.release()));
            }
        }
        *parentMatch0 = match;
    }
    #ifdef SOUL_PARSER_DEBUG_SUPPORT
    if (parser_debug_write_to_log)
    {
        if (match.hit) soul::lexer::WriteSuccessToLog(lexer, parser_debug_match_pos, "Specialization");
        else soul::lexer::WriteFailureToLog(lexer, "Specialization");
    }
    #endif
    if (!match.hit)
    {
        match.value = nullptr;
    }
    return match;
}

template struct TypeParser<soul::lexer::Lexer<p::lexer::PLexer<char>, char>>;

} // namespace p::type::parser
