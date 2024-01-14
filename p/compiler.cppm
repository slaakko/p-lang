// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.compiler;

import soul.lexer;
import std.core;

export namespace p {

enum class CompileFlags
{
    none = 0, update = 1 << 0
};

constexpr CompileFlags operator|(CompileFlags left, CompileFlags right)
{
    return CompileFlags(int(left) | int(right));
}

constexpr CompileFlags operator&(CompileFlags left, CompileFlags right)
{
    return CompileFlags(int(left) & int(right));
}

constexpr CompileFlags operator~(CompileFlags flags)
{
    return CompileFlags(~int(flags));
}

void SetCompileFlags(CompileFlags flags);

CompileFlags GetCompileFlags();

constexpr bool GetCompileFlag(CompileFlags flag) { return (GetCompileFlags() & flag) != CompileFlags::none; }

void Compile(const std::string& filePath, bool verbose);

class Context;

void Compile(const std::string& filePath, Context* context);

} // namespace p
