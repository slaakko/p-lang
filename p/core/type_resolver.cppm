// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.core.type_resolver;

import p.ast;
import soul.ast.span;
import std.core;

export namespace p {

class TypeSymbol;
class ContainerSymbol;
class Context;

TypeSymbol* GetType(Node* node, ContainerSymbol* container, Context* context);

} // namespace p
