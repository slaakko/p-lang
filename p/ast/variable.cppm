// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.ast.variable;

import p.ast.node;
import p.ast.common;
import soul.ast.span;
import std.core;

export namespace p {

class VariableDeclarationNode : public Node
{
public:
    VariableDeclarationNode(const soul::ast::Span& span_);
    VariableDeclarationNode(const soul::ast::Span& span_, IdentifierListNode* identifierList_, Node* typeName_);
    IdentifierListNode* IdentifierList() const { return identifierList.get(); }
    Node* TypeName() const { return typeName.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
private:
    std::unique_ptr<IdentifierListNode> identifierList;
    std::unique_ptr<Node> typeName;
};

} // namespace p
