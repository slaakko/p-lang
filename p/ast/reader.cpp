// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.ast.reader;

namespace p {

AstReader::AstReader(const std::string& filePath_) : 
    filePath(filePath_),
    file(filePath, util::OpenMode::binary | util::OpenMode::read),
    buffered(file),
    binaryReader(buffered)
{
}

soul::ast::Span AstReader::ReadSpan()
{
    int pos = binaryReader.ReadInt();
    int len = binaryReader.ReadInt();
    return soul::ast::Span(pos, len);
}

Node* AstReader::ReadNode()
{
    NodeKind kind = static_cast<NodeKind>(binaryReader.ReadByte());
    if (kind == NodeKind::none)
    {
        return nullptr;
    }
    else
    {
        soul::ast::Span span = ReadSpan();
        Node* node = CreateNode(kind, span);
        node->Read(*this);
        return node;
    }
}

} // namespace p
