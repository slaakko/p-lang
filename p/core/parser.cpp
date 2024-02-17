// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.core.parser;

namespace p {

ParseFunc parseFunc;

void SetParseFunc(ParseFunc parseFunc_)
{
    parseFunc = parseFunc_;
}

std::unique_ptr<Node> ParsePLangSourceFile(const std::string& plangSourceFile)
{
    if (parseFunc)
    {
        return parseFunc(plangSourceFile);
    }
    else
    {
        throw std::runtime_error("p core: parse function not set");
    }
}

} // namespace p
