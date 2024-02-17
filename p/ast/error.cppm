// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.ast.error;

import soul.ast.span;
import std.core;

export namespace p {

void ThrowError(const std::string& message, const std::string& filePath, const soul::ast::Span& span);

} // namespace p
