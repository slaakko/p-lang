// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.core.compile_flags;

namespace p {

CompileFlags compileFlags = CompileFlags::none;

void SetCompileFlags(CompileFlags flags)
{
    compileFlags = flags;
}

CompileFlags GetCompileFlags()
{
    return compileFlags;
}

bool GetCompileFlag(CompileFlags flag)
{
    return (compileFlags & flag) != CompileFlags::none;
}

} // namespace p

