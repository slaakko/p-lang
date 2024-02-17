// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.ast.writer;

import soul.ast.span;
import p.ast.node;
import util;
import std.core;

export namespace p {

class AstWriter
{
public:
    AstWriter(const std::string& filePath_);
    const std::string& FilePath() const { return filePath; }
    util::BinaryStreamWriter& GetBinaryWriter() { return binaryWriter; }
    void WriteNode(Node* node);
    void WriteSpan(const soul::ast::Span& span);
private:
    std::string filePath;
    util::FileStream file;
    util::BufferedStream buffered;
    util::BinaryStreamWriter binaryWriter;
};

} // namespace p
