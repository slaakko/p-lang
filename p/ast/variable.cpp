// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.ast.variable;

import p.ast.visitor;

namespace p {

VariableDeclarationNode::VariableDeclarationNode(const soul::ast::Span& span_) : Node(NodeKind::variableDeclarationNode, span_)
{
}

VariableDeclarationNode::VariableDeclarationNode(const soul::ast::Span& span_, IdentifierListNode* identifierList_, Node* typeName_) : 
    Node(NodeKind::variableDeclarationNode, span_), identifierList(identifierList_), typeName(typeName_)
{
    identifierList->SetParent(this);
    typeName->SetParent(this);
}

Node* VariableDeclarationNode::Clone() const
{
    return new VariableDeclarationNode(Span(), static_cast<IdentifierListNode*>(identifierList->Clone()), typeName->Clone());
}

void VariableDeclarationNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void VariableDeclarationNode::Write(AstWriter& writer)
{
    writer.WriteNode(identifierList.get());
    writer.WriteNode(typeName.get());
}

void VariableDeclarationNode::Read(AstReader& reader)
{
    identifierList.reset(static_cast<IdentifierListNode*>(reader.ReadNode()));
    identifierList->SetParent(this);
    typeName.reset(reader.ReadNode());
    typeName->SetParent(this);
}

} // namespace p
