// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.binder.evaluator;

import p.core;
import p.ast;
import std.core;

export namespace p {

Value* Evaluate(Node* node, Context* context, BlockSymbol* currentBlock);

} // namespace p
