// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.ast.reader;

import p.ast.node;
import soul.ast.span;
import util;
import std.core;

export namespace p {

class Node;

class AstReader
{
public:
    AstReader(const std::string& filePath_);
    const std::string& FilePath() const { return filePath; }
    util::BinaryStreamReader& GetBinaryReader() { return binaryReader; }
    Node* ReadNode();
    soul::ast::Span ReadSpan();
private:
    std::string filePath;
    util::FileStream file;
    util::BufferedStream buffered;
    util::BinaryStreamReader binaryReader;
};

} // namespace p
