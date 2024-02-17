// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.ast.writer;

namespace p {

AstWriter::AstWriter(const std::string& filePath_) :
    filePath(filePath_),
    file(filePath, util::OpenMode::binary | util::OpenMode::write),
    buffered(file),
    binaryWriter(buffered)
{
}

void AstWriter::WriteSpan(const soul::ast::Span& span)
{
    binaryWriter.Write(static_cast<int32_t>(span.pos));
    binaryWriter.Write(static_cast<int32_t>(span.len));
}

void AstWriter::WriteNode(Node* node)
{
    if (node)
    {
        binaryWriter.Write(static_cast<uint8_t>(node->Kind()));
        WriteSpan(node->Span());
        node->Write(*this);
    }
    else
    {
        binaryWriter.Write(static_cast<uint8_t>(NodeKind::none));
    }
}

} // namespace p
