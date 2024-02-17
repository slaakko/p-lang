// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.core.parsing_context;

import p.core.context;
import std.core;

export namespace p::core {

    namespace parsing_context {}
};

export namespace p {

class ParsingContext 
{
public:
    ParsingContext();
    bool ParsingLValue() const { return parsingLValue; }
    void PushParsingLValue();
    void PopParsingLValue();
    bool ParsingExpressionStatement() const { return parsingExpressionStatement; }
    void PushParsingExpressionStatement();
    void PopParsingExpressionStatement();
    bool ParsingArguments() const { return parsingArguments; }
    void PushParsingArguments();
    void PopParsingArguments();
    bool ParsingLValueOrExpressionStatementAndNotParsingArgs() const { return (parsingLValue || parsingExpressionStatement) && !parsingArguments; }
private:
    bool parsingLValue;
    std::stack<bool> parsingLValueStack;
    bool parsingExpressionStatement;
    std::stack<bool> parsingExpressionStatementStack;
    bool parsingArguments;
    std::stack<bool> parsingArgumentsStack;
};

} // namespace p
