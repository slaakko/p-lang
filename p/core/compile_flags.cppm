// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.core.compile_flags;

import std.core;

export namespace p {

enum class CompileFlags
{
    none = 0, rebuild = 1 << 0, update = 1 << 1
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
bool GetCompileFlag(CompileFlags flag);

} // namespace p
