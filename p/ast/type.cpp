// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.ast.type;

import p.ast.visitor;

namespace p {

IntegerNode::IntegerNode(const soul::ast::Span& span_) : Node(NodeKind::integerNode, span_)
{
}

Node* IntegerNode::Clone() const
{
    return new IntegerNode(Span());
}

void IntegerNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void IntegerNode::Write(AstWriter& writer)
{
}

void IntegerNode::Read(AstReader& reader)
{
}

RealNode::RealNode(const soul::ast::Span& span_) : Node(NodeKind::realNode, span_)
{
}

Node* RealNode::Clone() const
{
    return new RealNode(Span());
}

void RealNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void RealNode::Write(AstWriter& writer)
{
}

void RealNode::Read(AstReader& reader)
{
}

CharNode::CharNode(const soul::ast::Span& span_) : Node(NodeKind::charNode, span_)
{
}

Node* CharNode::Clone() const
{
    return new CharNode(Span());
}

void CharNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void CharNode::Write(AstWriter& writer)
{
}

void CharNode::Read(AstReader& reader)
{
}

BooleanNode::BooleanNode(const soul::ast::Span& span_) : Node(NodeKind::booleanNode, span_)
{
}

Node* BooleanNode::Clone() const
{
    return new BooleanNode(Span());
}

void BooleanNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void BooleanNode::Write(AstWriter& writer)
{
}

void BooleanNode::Read(AstReader& reader)
{
}

StringNode::StringNode(const soul::ast::Span& span_) : Node(NodeKind::stringNode, span_)
{
}

Node* StringNode::Clone() const
{
    return new StringNode(Span());
}

void StringNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void StringNode::Write(AstWriter& writer)
{
}

void StringNode::Read(AstReader& reader)
{
}

PointerNode::PointerNode(const soul::ast::Span& span_) : Node(NodeKind::pointerNode, span_)
{
}

Node* PointerNode::Clone() const
{
    return new PointerNode(Span());
}

void PointerNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void PointerNode::Write(AstWriter& writer)
{
}

void PointerNode::Read(AstReader& reader)
{
}

TypeDeclarationNode::TypeDeclarationNode(const soul::ast::Span& span_) : Node(NodeKind::typeDeclarationNode, span_)
{
}

TypeDeclarationNode::TypeDeclarationNode(const soul::ast::Span& span_, IdentifierNode* typeId_, Node* type_) : 
    Node(NodeKind::typeDeclarationNode, span_), typeId(typeId_), type(type_)
{
    typeId->SetParent(this);
    type->SetParent(this);
}

Node* TypeDeclarationNode::Clone() const
{
    return new TypeDeclarationNode(Span(), static_cast<IdentifierNode*>(typeId->Clone()), type->Clone());
}

void TypeDeclarationNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void TypeDeclarationNode::Write(AstWriter& writer)
{
    writer.WriteNode(typeId.get());
    writer.WriteNode(type.get());
}

void TypeDeclarationNode::Read(AstReader& reader)
{
    typeId.reset(static_cast<IdentifierNode*>(reader.ReadNode()));
    typeId->SetParent(this);
    type.reset(reader.ReadNode());
    type->SetParent(this);
}

SubrangeTypeNode::SubrangeTypeNode(const soul::ast::Span& span_) : Node(NodeKind::subrangeTypeNode, span_)
{
}

SubrangeTypeNode::SubrangeTypeNode(const soul::ast::Span& span_, Node* rangeStart_, Node* rangeEnd_) : 
    Node(NodeKind::subrangeTypeNode, span_), rangeStart(rangeStart_), rangeEnd(rangeEnd_)
{
    rangeStart->SetParent(this);
    rangeEnd->SetParent(this);
}

Node* SubrangeTypeNode::Clone() const
{
    return new SubrangeTypeNode(Span(), rangeStart->Clone(), rangeEnd->Clone());
}

void SubrangeTypeNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void SubrangeTypeNode::Write(AstWriter& writer)
{
    writer.WriteNode(rangeStart.get());
    writer.WriteNode(rangeEnd.get());
}

void SubrangeTypeNode::Read(AstReader& reader)
{
    rangeStart.reset(reader.ReadNode());
    rangeStart->SetParent(this);
    rangeEnd.reset(reader.ReadNode());
    rangeEnd->SetParent(this);
}

EnumeratedTypeNode::EnumeratedTypeNode(const soul::ast::Span& span_) : Node(NodeKind::enumeratedTypeNode, span_)
{
}

EnumeratedTypeNode::EnumeratedTypeNode(const soul::ast::Span& span_, IdentifierListNode* enumerationConstants_) :
    Node(NodeKind::enumeratedTypeNode, span_), enumerationConstants(enumerationConstants_)
{
    enumerationConstants->SetParent(this);
}

Node* EnumeratedTypeNode::Clone() const
{
    return new EnumeratedTypeNode(Span(), static_cast<IdentifierListNode*>(enumerationConstants->Clone()));
}

void EnumeratedTypeNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void EnumeratedTypeNode::Write(AstWriter& writer)
{
    writer.WriteNode(enumerationConstants.get());
}

void EnumeratedTypeNode::Read(AstReader& reader)
{
    enumerationConstants.reset(static_cast<IdentifierListNode*>(reader.ReadNode()));
    enumerationConstants->SetParent(this);
}

FieldListNode::FieldListNode(const soul::ast::Span& span_) : Node(NodeKind::fieldListNode, span_)
{
}

FieldListNode::FieldListNode(const soul::ast::Span& span_, IdentifierListNode* fieldNames_, Node* typeName_) : 
    Node(NodeKind::fieldListNode, span_), fieldNames(fieldNames_), typeName(typeName_)
{
    fieldNames->SetParent(this);
    typeName->SetParent(this);
}

Node* FieldListNode::Clone() const
{
    return new FieldListNode(Span(), static_cast<IdentifierListNode*>(fieldNames->Clone()), typeName->Clone());
}

void FieldListNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void FieldListNode::Write(AstWriter& writer)
{
    writer.WriteNode(fieldNames.get());
    writer.WriteNode(typeName.get());
}

void FieldListNode::Read(AstReader& reader)
{
    fieldNames.reset(static_cast<IdentifierListNode*>(reader.ReadNode()));
    fieldNames->SetParent(this);
    typeName.reset(reader.ReadNode());
    typeName->SetParent(this);
}

ObjectTypeNode::ObjectTypeNode(const soul::ast::Span& span_) : Node(NodeKind::objectTypeNode, span_)
{
}

void ObjectTypeNode::SetHeritage(IdentifierNode* baseObjectTypeName_)
{
    baseObjectTypeName.reset(baseObjectTypeName_);
    baseObjectTypeName->SetParent(this);
}

void ObjectTypeNode::SetTypeParamId(IdentifierNode* typeParamId_)
{
    typeParamId.reset(typeParamId_);
    typeParamId->SetParent(this);
}

void ObjectTypeNode::AddFields(const soul::ast::Span& span, IdentifierListNode* fieldNames, Node* type)
{
    std::unique_ptr<FieldListNode> fieldList(new FieldListNode(span, fieldNames, type));
    fieldList->SetParent(this);
    fieldLists.push_back(std::unique_ptr<FieldListNode>(fieldList.release()));
}

void ObjectTypeNode::AddMethod(SubroutineHeadingNode* method)
{
    method->SetParent(this);
    methods.push_back(std::unique_ptr<SubroutineHeadingNode>(method));
}

void ObjectTypeNode::SetFilePath(const std::string& filePath_)
{
    filePath = filePath_;
}

const std::string& ObjectTypeNode::FilePath() const
{
    if (!filePath.empty())
    {
        return filePath;
    }
    else
    {
        return Node::FilePath();
    }
}

Node* ObjectTypeNode::Clone() const
{
    ObjectTypeNode* clone = new ObjectTypeNode(Span());
    if (baseObjectTypeName)
    {
        clone->SetHeritage(static_cast<IdentifierNode*>(baseObjectTypeName->Clone()));
    }
    if (typeParamId)
    {
        clone->SetTypeParamId(static_cast<IdentifierNode*>(typeParamId->Clone()));
    }
    for (const auto& fieldList : fieldLists)
    {
        clone->AddFields(fieldList->Span(), static_cast<IdentifierListNode*>(fieldList->FieldNames()->Clone()), fieldList->TypeName()->Clone());
    }
    for (const auto& method : methods)
    {
        clone->AddMethod(static_cast<SubroutineHeadingNode*>(method->Clone()));
    }
    clone->SetFilePath(filePath);
    return clone;
}

void ObjectTypeNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ObjectTypeNode::Write(AstWriter& writer)
{
    writer.WriteNode(baseObjectTypeName.get());
    writer.WriteNode(typeParamId.get());
    int32_t nf = fieldLists.size();
    writer.GetBinaryWriter().Write(nf);
    for (const auto& fieldList : fieldLists)
    {
        writer.WriteNode(fieldList.get());
    }
    int32_t n = methods.size();
    writer.GetBinaryWriter().Write(n);
    for (const auto& method : methods)
    {
        writer.WriteNode(method.get());
    }
    writer.GetBinaryWriter().Write(filePath);
}

void ObjectTypeNode::Read(AstReader& reader)
{
    baseObjectTypeName.reset(static_cast<IdentifierNode*>(reader.ReadNode()));
    if (baseObjectTypeName)
    {
        baseObjectTypeName->SetParent(this);
    }
    typeParamId.reset(static_cast<IdentifierNode*>(reader.ReadNode()));
    if (typeParamId)
    {
        typeParamId->SetParent(this);
    }
    int32_t nf = reader.GetBinaryReader().ReadInt();
    for (int32_t i = 0; i < nf; ++i)
    {
        std::unique_ptr<FieldListNode> fieldList(static_cast<FieldListNode*>(reader.ReadNode()));
        fieldList->SetParent(this);
        fieldLists.push_back(std::unique_ptr<FieldListNode>(fieldList.release()));
    }
    int32_t n = reader.GetBinaryReader().ReadInt();
    for (int32_t i = 0; i < n; ++i)
    {
        SubroutineHeadingNode* headingNode = static_cast<SubroutineHeadingNode*>(reader.ReadNode());
        AddMethod(headingNode);
    }
    filePath = reader.GetBinaryReader().ReadUtf8String();
}

ArrayTypeNode::ArrayTypeNode(const soul::ast::Span& span_) : Node(NodeKind::arrayTypeNode, span_)
{
}

ArrayTypeNode::ArrayTypeNode(const soul::ast::Span& span_, Node* elementType_) : Node(NodeKind::arrayTypeNode, span_), elementType(elementType_)
{
    elementType->SetParent(this);
}

Node* ArrayTypeNode::Clone() const
{
    return new ArrayTypeNode(Span(), elementType->Clone());
}

void ArrayTypeNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ArrayTypeNode::Write(AstWriter& writer)
{
    writer.WriteNode(elementType.get());
}

void ArrayTypeNode::Read(AstReader& reader)
{
    elementType.reset(reader.ReadNode());
    elementType->SetParent(this);
}

SpecializationNode::SpecializationNode(const soul::ast::Span& span_) : Node(NodeKind::specializationNode, span_)
{
}

SpecializationNode::SpecializationNode(const soul::ast::Span& span_, IdentifierNode* genericId_, Node* type_) : 
    Node(NodeKind::specializationNode, span_), genericId(genericId_), type(type_)
{
    genericId->SetParent(this);
    type->SetParent(this);
}

Node* SpecializationNode::Clone() const
{
    return new SpecializationNode(Span(), static_cast<IdentifierNode*>(genericId->Clone()), type->Clone());
}

void SpecializationNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void SpecializationNode::Write(AstWriter& writer)
{
    writer.WriteNode(genericId.get());
    writer.WriteNode(type.get());
}

void SpecializationNode::Read(AstReader& reader)
{
    genericId.reset(static_cast<IdentifierNode*>(reader.ReadNode()));
    genericId->SetParent(this);
    type.reset(reader.ReadNode());
    type->SetParent(this);
}

} // namespace p
