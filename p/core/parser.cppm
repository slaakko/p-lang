// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.core.parser;

import p.ast;
import std.core;

export namespace p {

using ParseFunc = std::unique_ptr<Node> (*)(const std::string& plangSourceFile);

void SetParseFunc(ParseFunc parseFunc_);

std::unique_ptr<Node> ParsePLangSourceFile(const std::string& plangSourceFile);

} // namespace p
