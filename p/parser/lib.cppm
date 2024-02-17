// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.parser.lib;

import p.ast;
import std.core;

export namespace p {

void ParserLibInit();

std::unique_ptr<Node> Parse(const std::string& plangSourceFile);

} // namespace p
