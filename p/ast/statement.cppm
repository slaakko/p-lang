// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.ast.statement;

import p.ast.node;
import soul.ast.span;
import std.core;

export namespace p {

class IdentifierNode;

enum class Direction
{
    to, downto
};

class StatementNode : public Node
{
public:
    StatementNode(NodeKind kind_, const soul::ast::Span& span_);
};

class AssignmentStatementNode : public StatementNode
{
public:
    AssignmentStatementNode(const soul::ast::Span& span_);
    AssignmentStatementNode(const soul::ast::Span& span_, Node* target_, Node* source_);
    Node* Target() const { return target.get(); }
    Node* Source() const { return source.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
private:
    std::unique_ptr<Node> target;
    std::unique_ptr<Node> source;
};

class ExpressionStatementNode : public StatementNode
{
public:
    ExpressionStatementNode(const soul::ast::Span& span_);
    ExpressionStatementNode(const soul::ast::Span& span_, Node* expr_);
    Node* Expr() const { return expr.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
private:
    std::unique_ptr<Node> expr;
};

class EmptyStatementNode : public StatementNode
{
public:
    EmptyStatementNode(const soul::ast::Span& span_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
};

class CompoundStatementNode : public StatementNode
{
public:
    CompoundStatementNode(const soul::ast::Span& span_);
    void AddStatement(StatementNode* statement);
    const std::vector<std::unique_ptr<StatementNode>>& Statements() const { return statements; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    const std::string& FilePath() const override;
    void SetFilePath(const std::string& filePath_);
private:
    std::vector<std::unique_ptr<StatementNode>> statements;
    std::string filePath;
};

class IfStatementNode : public StatementNode
{
public:
    IfStatementNode(const soul::ast::Span& span_);
    IfStatementNode(const soul::ast::Span& span_, Node* condition_, StatementNode* thenStatement_);
    void SetElseStatement(StatementNode* elseStatement_);
    Node* Condition() const { return condition.get(); }
    StatementNode* ThenStatement() const { return thenStatement.get(); }
    StatementNode* ElseStatement() const { return elseStatement.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
private:
    std::unique_ptr<Node> condition;
    std::unique_ptr<StatementNode> thenStatement;
    std::unique_ptr<StatementNode> elseStatement;
};

class CaseNode : public Node
{
public:
    CaseNode(const soul::ast::Span& span_);
    void AddRange(Node* range);
    const std::vector<std::unique_ptr<Node>>& Ranges() const { return ranges; }
    void SetStatement(StatementNode* statement_);
    StatementNode* Statement() const { return statement.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
private:
    std::vector<std::unique_ptr<Node>> ranges;
    std::unique_ptr<StatementNode> statement;
};

class ConstantRangeNode : public Node
{
public:
    ConstantRangeNode(const soul::ast::Span& span_);
    ConstantRangeNode(const soul::ast::Span& span_, Node* start_, Node* end_);
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

class CaseStatementNode : public StatementNode
{
public:
    CaseStatementNode(const soul::ast::Span& span_);
    CaseStatementNode(const soul::ast::Span& span_, Node* condition_);
    Node* Condition() const { return condition.get(); }
    void AddCase(CaseNode* caseNode);
    const std::vector<std::unique_ptr<CaseNode>>& Cases() const { return cases; }
    void SetElsePart(StatementNode* elsePart_);
    StatementNode* ElsePart() const { return elsePart.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
private:
    std::unique_ptr<Node> condition;
    std::vector<std::unique_ptr<CaseNode>> cases;
    std::unique_ptr<StatementNode> elsePart;
};

class RepeatStatementNode : public StatementNode
{
public:
    RepeatStatementNode(const soul::ast::Span& span_);
    void AddStatement(StatementNode* statement);
    const std::vector<std::unique_ptr<StatementNode>>& Statements() const { return statements; }
    void SetCondition(Node* condition_);
    Node* Condition() const { return condition.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
private:
    std::vector<std::unique_ptr<StatementNode>> statements;
    std::unique_ptr<Node> condition;
};

class WhileStatementNode : public StatementNode
{
public:
    WhileStatementNode(const soul::ast::Span& span_);
    WhileStatementNode(const soul::ast::Span& span_, Node* condition_, StatementNode* statement_);
    Node* Condition() const { return condition.get(); }
    StatementNode* Statement() const { return statement.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
private:
    std::unique_ptr<Node> condition;
    std::unique_ptr<StatementNode> statement;
};

class ForStatementNode : public StatementNode
{
public:
    ForStatementNode(const soul::ast::Span& span_);
    ForStatementNode(const soul::ast::Span& span_, IdentifierNode* controlVariable_, Node* initialValue_, Direction direction_, Node* finalValue_, StatementNode* statement_);
    IdentifierNode* ControlVariable() const { return controlVariable.get(); }
    Node* InitialValue() const { return initialValue.get(); }
    Direction GetDirection() const { return direction; }
    Node* FinalValue() const { return finalValue.get(); }
    StatementNode* Statement() const { return statement.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
private:
    std::unique_ptr<IdentifierNode> controlVariable;
    std::unique_ptr<Node> initialValue;
    Direction direction;
    std::unique_ptr<Node> finalValue;
    std::unique_ptr<StatementNode> statement;
};

} // namespace p
