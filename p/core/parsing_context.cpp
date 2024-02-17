// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.core.parsing_context;

namespace p {

ParsingContext::ParsingContext() : parsingLValue(false), parsingExpressionStatement(false), parsingArguments(false)
{
}

void ParsingContext::PushParsingLValue()
{
    parsingLValueStack.push(parsingLValue);
    parsingLValue = true;
}

void ParsingContext::PopParsingLValue()
{
    parsingLValue = parsingLValueStack.top();
    parsingLValueStack.pop();
}

void ParsingContext::PushParsingExpressionStatement()
{
    parsingExpressionStatementStack.push(parsingExpressionStatement);
    parsingExpressionStatement = true;
}

void ParsingContext::PopParsingExpressionStatement()
{
    parsingExpressionStatement = parsingExpressionStatementStack.top();
    parsingExpressionStatementStack.pop();
}

void ParsingContext::PushParsingArguments()
{
    parsingArgumentsStack.push(parsingArguments);
    parsingArguments = true;
}

void ParsingContext::PopParsingArguments()
{
    parsingArguments = parsingArgumentsStack.top();
    parsingArgumentsStack.pop();
}

} // namespace p
