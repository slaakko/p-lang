// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.ast.common;

import p.ast.node;
import p.ast.node_list;
import std.core;

export namespace p {

class UnitNameNode : public Node
{
public:
    UnitNameNode(const soul::ast::Span& span_);
    UnitNameNode(const soul::ast::Span& span_, const std::string& name_);
    const std::string& Name() const { return name; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
private:
    std::string name;
};

class UnitNameListNode : public Node
{
public:
    UnitNameListNode(const soul::ast::Span& span_);
    const NodeList<UnitNameNode>& UnitNameNodes() const { return unitNameNodes; }
    void AddUnitName(UnitNameNode* unitName);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
private:
    NodeList<UnitNameNode> unitNameNodes;
};

class IdentifierNode : public Node
{
public:
    IdentifierNode(const soul::ast::Span& span_);
    IdentifierNode(const soul::ast::Span& span_, const std::string& str_);
    const std::string& Str() const { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
private:
    std::string str;
};

class IdentifierListNode : public Node
{
public:
    IdentifierListNode(const soul::ast::Span& span_);
    const NodeList<IdentifierNode>& IdentifierNodes() const { return identifierNodes; }
    void AddIdentifier(IdentifierNode* identifier);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
private:
    NodeList<IdentifierNode> identifierNodes;
};

} // namespace p
