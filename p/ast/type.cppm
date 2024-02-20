// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.ast.type;

import p.ast.node;
import p.ast.common;
import p.ast.subroutine;
import soul.ast.span;
import std.core;

export namespace p {

class IntegerNode : public Node
{
public:
    IntegerNode(const soul::ast::Span& span_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
};

class RealNode : public Node
{
public:
    RealNode(const soul::ast::Span& span_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
};

class CharNode : public Node
{
public:
    CharNode(const soul::ast::Span& span_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
};

class BooleanNode : public Node
{
public:
    BooleanNode(const soul::ast::Span& span_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
};

class StringNode : public Node
{
public:
    StringNode(const soul::ast::Span& span_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
};

class PointerNode : public Node
{
public:
    PointerNode(const soul::ast::Span& span_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
};

class TypeDeclarationNode : public Node
{
public:
    TypeDeclarationNode(const soul::ast::Span& span_);
    TypeDeclarationNode(const soul::ast::Span& span_, IdentifierNode* typeId_, Node* type_);
    IdentifierNode* TypeId() const { return typeId.get(); }
    Node* Type() const { return type.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
private:
    std::unique_ptr<IdentifierNode> typeId;
    std::unique_ptr<Node> type;
};

class SubrangeTypeNode : public Node
{
public:
    SubrangeTypeNode(const soul::ast::Span& span_);
    SubrangeTypeNode(const soul::ast::Span& span_, Node* rangeStart_, Node* rangeEnd_);
    Node* RangeStart() const { return rangeStart.get(); }
    Node* RangeEnd() const { return rangeEnd.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
private:
    std::unique_ptr<Node> rangeStart;
    std::unique_ptr<Node> rangeEnd;
};

class EnumeratedTypeNode : public Node
{
public:
    EnumeratedTypeNode(const soul::ast::Span& span_);
    EnumeratedTypeNode(const soul::ast::Span& span_, IdentifierListNode* enumerationConstants_);
    IdentifierListNode* EnumerationConstants() const { return enumerationConstants.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
private:
    std::unique_ptr<IdentifierListNode> enumerationConstants;
};

class FieldListNode : public Node
{
public:
    FieldListNode(const soul::ast::Span& span_);
    FieldListNode(const soul::ast::Span& span_, IdentifierListNode* fieldNames_, Node* typeName_);
    IdentifierListNode* FieldNames() const { return fieldNames.get(); }
    Node* TypeName() const { return typeName.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
private:
    std::unique_ptr<IdentifierListNode> fieldNames;
    std::unique_ptr<Node> typeName;
};

class ObjectTypeNode : public Node
{
public:
    ObjectTypeNode(const soul::ast::Span& span_);
    void SetHeritage(IdentifierNode* baseObjectTypeName_);
    IdentifierNode* BaseObjectTypeName() const { return baseObjectTypeName.get(); }
    void SetTypeParamId(IdentifierNode* typeParamId_);
    IdentifierNode* TypeParamId() const { return typeParamId.get(); }
    void AddFields(const soul::ast::Span& span, IdentifierListNode* fieldNames, Node* type);
    const std::vector<std::unique_ptr<FieldListNode>>& FieldLists() const { return fieldLists; }
    void AddMethod(SubroutineHeadingNode* method);
    const std::vector<std::unique_ptr<SubroutineHeadingNode>>& Methods() const { return methods; }
    Node* Clone() const override;
    void SetFilePath(const std::string& filePath_);
    const std::string& FilePath() const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
private:
    std::unique_ptr<IdentifierNode> baseObjectTypeName;
    std::unique_ptr<IdentifierNode> typeParamId;
    std::vector<std::unique_ptr<FieldListNode>> fieldLists;
    std::vector<std::unique_ptr<SubroutineHeadingNode>> methods;
    std::string filePath;
};

class ArrayTypeNode : public Node
{
public:
    ArrayTypeNode(const soul::ast::Span& span_);
    ArrayTypeNode(const soul::ast::Span& span_, Node* elementType_);
    Node* ElementType() const { return elementType.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
private:
    std::unique_ptr<Node> elementType;
};

class SpecializationNode : public Node
{
public:
    SpecializationNode(const soul::ast::Span& span_);
    SpecializationNode(const soul::ast::Span& span_, IdentifierNode* genericId_, Node* type_);
    IdentifierNode* GenericId() const { return genericId.get(); }
    Node* Type() const { return type.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
private:
    std::unique_ptr<IdentifierNode> genericId;
    std::unique_ptr<Node> type;
};

} // namespace p
