// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.ast.span;

import std.core;

export namespace soul::ast {

export namespace span {}

struct Span
{
    Span() : pos(-1), len(-1) {}
    Span(int pos_, int len_) : pos(pos_), len(len_) {}
    bool IsValid() const { return pos != -1; }
    bool Contains(int pos_) const { return pos_ >= pos && pos_ < pos + len; }
    int pos;
    int len;
};

} // namespace soul::ast

