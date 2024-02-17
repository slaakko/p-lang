// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.parser.lib;

import p.main_parser;
import p.lexer;
import p.core;
import util;

namespace p {

void ParserLibInit()
{
    SetParseFunc(&Parse);
}

std::unique_ptr<Node> Parse(const std::string& plangSourceFile)
{
    std::string content = util::ReadFile(plangSourceFile);
    auto lexer = p::lexer::MakeLexer(content.c_str(), content.c_str() + content.size(), plangSourceFile);
    using LexerType = decltype(lexer);
    ParsingContext context;
    std::unique_ptr<p::Node> node = main_parser::MainParser<LexerType>::Parse(lexer, &context);
    return node;
}

} // namespace p
