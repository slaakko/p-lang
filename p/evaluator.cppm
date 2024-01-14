// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.evaluator;

import soul.lexer;
import std.core;

export namespace p {

class Node;
class Value;
class Block;

Value* Evaluate(Node* node, soul::lexer::LexerBase<char>& lexer, Block* block);

} // namespace p
