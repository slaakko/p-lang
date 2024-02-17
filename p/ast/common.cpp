// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.ast.common;

import p.ast.visitor;

namespace p {

UnitNameNode::UnitNameNode(const soul::ast::Span& span_) : Node(NodeKind::unitNameNode, span_)
{
}

UnitNameNode::UnitNameNode(const soul::ast::Span& span_, const std::string& name_) : Node(NodeKind::unitNameNode, span_), name(name_)
{
}

Node* UnitNameNode::Clone() const
{
    return new UnitNameNode(Span(), name);
}

void UnitNameNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void UnitNameNode::Write(AstWriter& writer)
{
    writer.GetBinaryWriter().Write(name);
}

void UnitNameNode::Read(AstReader& reader)
{
    name = reader.GetBinaryReader().ReadUtf8String();
}

UnitNameListNode::UnitNameListNode(const soul::ast::Span& span_) : Node(NodeKind::unitNameListNode, span_)
{
}

void UnitNameListNode::AddUnitName(UnitNameNode* unitName)
{
    unitName->SetParent(this);
    unitNameNodes.Add(unitName);
}

Node* UnitNameListNode::Clone() const
{
    UnitNameListNode* clone = new UnitNameListNode(Span());
    unitNameNodes.CloneTo(clone->unitNameNodes, clone);
    return clone;
}

void UnitNameListNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void UnitNameListNode::Write(AstWriter& writer)
{
    unitNameNodes.Write(writer);
}

void UnitNameListNode::Read(AstReader& reader)
{
    unitNameNodes.Read(reader);
    unitNameNodes.SetParent(this);
}

IdentifierNode::IdentifierNode(const soul::ast::Span& span_) : Node(NodeKind::identifierNode, span_)
{
}

IdentifierNode::IdentifierNode(const soul::ast::Span& span_, const std::string& str_) : Node(NodeKind::identifierNode, span_), str(str_)
{
}

Node* IdentifierNode::Clone() const
{
    return new IdentifierNode(Span(), str);
}

void IdentifierNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void IdentifierNode::Write(AstWriter& writer)
{
    writer.GetBinaryWriter().Write(str);
}

void IdentifierNode::Read(AstReader& reader)
{
    str = reader.GetBinaryReader().ReadUtf8String();
}

IdentifierListNode::IdentifierListNode(const soul::ast::Span& span_) : Node(NodeKind::identifierListNode, span_)
{
}

void IdentifierListNode::AddIdentifier(IdentifierNode* identifier) 
{ 
    identifier->SetParent(this);
    identifierNodes.Add(identifier); 
}

Node* IdentifierListNode::Clone() const
{
    IdentifierListNode* clone = new IdentifierListNode(Span());
    identifierNodes.CloneTo(clone->identifierNodes, clone);
    return clone;
}

void IdentifierListNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void IdentifierListNode::Write(AstWriter& writer)
{
    identifierNodes.Write(writer);
}

void IdentifierListNode::Read(AstReader& reader)
{
    identifierNodes.Read(reader);
    identifierNodes.SetParent(this);
}

} // namespace p
