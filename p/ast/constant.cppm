// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.ast.constant;

import p.ast.node;
import p.ast.common;
import soul.ast.span;
import std.core;

export namespace p {

class ConstantDeclarationNode: public Node
{
public:    
    ConstantDeclarationNode(const soul::ast::Span& span_);
    ConstantDeclarationNode(const soul::ast::Span& span_, IdentifierNode* constantId_);
    IdentifierNode* ConstantId() const { return constantId.get(); }
    Node* TypeName() const { return typeName.get(); }
    void SetTypeName(Node* typeName_);
    Node* Constant() const { return constant.get(); }
    void SetConstant(Node* constant_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
private:
    std::unique_ptr<IdentifierNode> constantId;
    std::unique_ptr<Node> typeName;
    std::unique_ptr<Node> constant;
};

class ArrayConstantNode : public Node
{
public:
    ArrayConstantNode(const soul::ast::Span& span_);
    void AddElement(Node* element);
    const std::vector<std::unique_ptr<Node>>& Elements() const { return elements; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
private:
    std::vector<std::unique_ptr<Node>> elements;
};

class FieldNode : public Node
{
public:
    FieldNode(const soul::ast::Span& span_);
    FieldNode(IdentifierNode* fieldName_, Node* constant_);
    IdentifierNode* FieldName() const { return fieldName.get(); }
    Node* Constant() const { return constant.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
private:
    std::unique_ptr<IdentifierNode> fieldName;
    std::unique_ptr<Node> constant;
};

class ObjectConstantNode : public Node
{
public:
    ObjectConstantNode(const soul::ast::Span& span_);
    void AddField(FieldNode* field);
    const std::vector<std::unique_ptr<FieldNode>>& Fields() const { return fields; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
private:
    std::vector<std::unique_ptr<FieldNode>> fields;
};

} // namespace p
