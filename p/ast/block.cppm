// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.ast.block;

import p.ast.node;
import p.ast.statement;
import std.core;

export namespace p {

class BlockNode : public Node
{
public:
    BlockNode(const soul::ast::Span& span_);
    void AddDeclaration(Node* declaration);
    const std::vector<std::unique_ptr<Node>>& Declarations() const { return declarations; }
    void SetCompoundStatement(CompoundStatementNode* compoundStatement_);
    CompoundStatementNode* CompoundStatement() const { return compoundStatement.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
private:
    std::vector<std::unique_ptr<Node>> declarations;
    std::unique_ptr<CompoundStatementNode> compoundStatement;
};

class ForwardNode : public Node
{
public:
    ForwardNode(const soul::ast::Span& span_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
};

class ExternalNode : public Node
{
public:
    ExternalNode(const soul::ast::Span& span_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
};

} // namespace p
