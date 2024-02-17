// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.ast.constant;

import p.ast.visitor;

namespace p {

ConstantDeclarationNode::ConstantDeclarationNode(const soul::ast::Span& span_) : Node(NodeKind::constantDeclarationNode, span_)
{
}

ConstantDeclarationNode::ConstantDeclarationNode(const soul::ast::Span& span_, IdentifierNode* constantId_) :
    Node(NodeKind::constantDeclarationNode, span_), constantId(constantId_)
{
    constantId->SetParent(this);
}

void ConstantDeclarationNode::SetTypeName(Node* typeName_)
{
    typeName.reset(typeName_);
    typeName->SetParent(this);
}

void ConstantDeclarationNode::SetConstant(Node* constant_)
{
    constant.reset(constant_);
    constant->SetParent(this);
}

Node* ConstantDeclarationNode::Clone() const
{
    ConstantDeclarationNode* clone = new ConstantDeclarationNode(Span(), static_cast<IdentifierNode*>(constantId->Clone()));
    clone->SetTypeName(typeName->Clone());
    clone->SetConstant(constant->Clone());
    return clone;
}

void ConstantDeclarationNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ConstantDeclarationNode::Write(AstWriter& writer)
{
    writer.WriteNode(constantId.get());
    writer.WriteNode(typeName.get());
    writer.WriteNode(constant.get());
}

void ConstantDeclarationNode::Read(AstReader& reader)
{
    constantId.reset(static_cast<IdentifierNode*>(reader.ReadNode()));
    constantId->SetParent(this);
    typeName.reset(reader.ReadNode());
    typeName->SetParent(this);
    constant.reset(reader.ReadNode());
    constant->SetParent(this);
}

ArrayConstantNode::ArrayConstantNode(const soul::ast::Span& span_) : Node(NodeKind::arrayConstantNode, span_)
{
}

void ArrayConstantNode::AddElement(Node* element)
{
    element->SetParent(this);
    elements.push_back(std::unique_ptr<Node>(element));
}

Node* ArrayConstantNode::Clone() const
{
    ArrayConstantNode* clone = new ArrayConstantNode(Span());
    for (const auto& element : elements)
    {
        clone->AddElement(element->Clone());
    }
    return clone;
}

void ArrayConstantNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ArrayConstantNode::Write(AstWriter& writer)
{
    int32_t n = elements.size();
    writer.GetBinaryWriter().Write(n);
    for (const auto& element : elements)
    {
        writer.WriteNode(element.get());
    }
}

void ArrayConstantNode::Read(AstReader& reader)
{
    int32_t n = reader.GetBinaryReader().ReadInt();
    for (int32_t i = 0; i < n; ++i)
    {
        Node* element = reader.ReadNode();
        AddElement(element);
    }
}

FieldNode::FieldNode(const soul::ast::Span& span_) : Node(NodeKind::fieldNode, span_)
{
}

FieldNode::FieldNode(IdentifierNode* fieldName_, Node* constant_) : Node(NodeKind::fieldNode, fieldName_->Span()), fieldName(fieldName_), constant(constant_)
{
    fieldName->SetParent(this);
    constant->SetParent(this);
}

Node* FieldNode::Clone() const
{
    return new FieldNode(static_cast<IdentifierNode*>(fieldName->Clone()), constant->Clone());
}

void FieldNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void FieldNode::Write(AstWriter& writer)
{
    writer.WriteNode(fieldName.get());
    writer.WriteNode(constant.get());
}

void FieldNode::Read(AstReader& reader)
{
    fieldName.reset(static_cast<IdentifierNode*>(reader.ReadNode()));
    fieldName->SetParent(this);
    constant.reset(reader.ReadNode());
    constant->SetParent(this);
}

ObjectConstantNode::ObjectConstantNode(const soul::ast::Span& span_) : Node(NodeKind::objectConstantNode, span_)
{
}

void ObjectConstantNode::AddField(FieldNode* field)
{
    field->SetParent(this);
    fields.push_back(std::unique_ptr<FieldNode>(field));
}

Node* ObjectConstantNode::Clone() const
{
    ObjectConstantNode* clone = new ObjectConstantNode(Span());
    for (const auto& field : fields)
    {
        clone->AddField(static_cast<FieldNode*>(field->Clone()));
    }
    return clone;
}

void ObjectConstantNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ObjectConstantNode::Write(AstWriter& writer)
{
    int32_t n = fields.size();
    writer.GetBinaryWriter().Write(n);
    for (const auto& field : fields)
    {
        writer.WriteNode(field.get());
    }
}

void ObjectConstantNode::Read(AstReader& reader)
{
    int32_t n = reader.GetBinaryReader().ReadInt();
    for (int32_t i = 0; i < n; ++i)
    {
        FieldNode* field = static_cast<FieldNode*>(reader.ReadNode());
        AddField(field);
    }
}

} // namespace p
