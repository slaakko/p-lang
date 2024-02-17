// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.ast.block;

import p.ast.visitor;
import p.ast.writer;
import p.ast.reader;

namespace p {

BlockNode::BlockNode(const soul::ast::Span& span_) : Node(NodeKind::blockNode, span_)
{
}

void BlockNode::AddDeclaration(Node* declaration)
{
    declaration->SetParent(this);
    declarations.push_back(std::unique_ptr<Node>(declaration));
}

void BlockNode::SetCompoundStatement(CompoundStatementNode* compoundStatement_)
{
    compoundStatement.reset(compoundStatement_);
    if (compoundStatement)
    {
        compoundStatement->SetParent(this);
    }
}

Node* BlockNode::Clone() const
{
    BlockNode* clone = new BlockNode(Span());
    for (const auto& declaration : declarations)
    {
        clone->AddDeclaration(declaration->Clone());
    }
    clone->SetCompoundStatement(static_cast<CompoundStatementNode*>(compoundStatement->Clone()));
    return clone;
}

void BlockNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void BlockNode::Write(AstWriter& writer)
{
    int32_t n = declarations.size();
    writer.GetBinaryWriter().Write(n);
    for (const auto& declaration : declarations)
    {
        writer.WriteNode(declaration.get());
    }
    writer.WriteNode(compoundStatement.get());
}

void BlockNode::Read(AstReader& reader)
{
    int32_t n = reader.GetBinaryReader().ReadInt();
    for (int32_t i = 0; i < n; ++i)
    {
        Node* declaration = reader.ReadNode();
        AddDeclaration(declaration);
    }
    SetCompoundStatement(static_cast<CompoundStatementNode*>(reader.ReadNode()));
}

ForwardNode::ForwardNode(const soul::ast::Span& span_) : Node(NodeKind::forwardNode, span_)
{
}

Node* ForwardNode::Clone() const
{
    return new ForwardNode(Span());
}

void ForwardNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ForwardNode::Write(AstWriter& writer)
{
}

void ForwardNode::Read(AstReader& reader)
{
}

ExternalNode::ExternalNode(const soul::ast::Span& span_) : Node(NodeKind::externalNode, span_)
{
}

Node* ExternalNode::Clone() const
{
    return new ExternalNode(Span());
}

void ExternalNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ExternalNode::Write(AstWriter& writer)
{
}

void ExternalNode::Read(AstReader& reader)
{
}

} // namespace p
