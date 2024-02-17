// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.binder.bound_statement;

import p.binder.bound_expression;
import p.binder.bound_node;
import p.core;
import p.ast;
import soul.ast.span;
import std.core;

export namespace p {

class BoundStatementNode : public BoundNode
{
public:
    BoundStatementNode(BoundNodeKind kind_, const soul::ast::Span& span_);
};

class BoundExpressionStatementNode : public BoundStatementNode
{
public:
    BoundExpressionStatementNode(const soul::ast::Span& span_, BoundExpressionNode* expression_);
    BoundExpressionNode* Expression() const { return expression.get(); }
    bool Pop() const { return pop; }
    void DontPop() { pop = false; }
    void Accept(BoundNodeVisitor& visitor) override;
private:
    std::unique_ptr<BoundExpressionNode> expression;
    bool pop;
};

class BoundEmptyStatementNode : public BoundStatementNode
{
public:
    BoundEmptyStatementNode(const soul::ast::Span& span_);
    void Accept(BoundNodeVisitor& visitor) override;
};

class BoundAssignmentStatementNode : public BoundStatementNode
{
public:
    BoundAssignmentStatementNode(const soul::ast::Span& span_, BoundExpressionNode* target_, BoundExpressionNode* source_);
    BoundExpressionNode* Target() const { return target.get(); }
    BoundExpressionNode* Source() const { return source.get(); }
    void Accept(BoundNodeVisitor& visitor) override;
private:
    std::unique_ptr<BoundExpressionNode> target;
    std::unique_ptr<BoundExpressionNode> source;
};

class BoundCompoundStatementNode : public BoundStatementNode
{
public:
    BoundCompoundStatementNode(const soul::ast::Span& span_);
    void AddStatement(BoundStatementNode* statement);
    const std::vector<std::unique_ptr<BoundStatementNode>>& Statements() const { return statements; }
    void SetFilePath(const std::string& filePath_);
    const std::string& FilePath() const override;
    void Accept(BoundNodeVisitor& visitor) override;
private:
    std::vector<std::unique_ptr<BoundStatementNode>> statements;
    std::string filePath;
};

class BoundIfStatementNode : public BoundStatementNode
{
public:
    BoundIfStatementNode(const soul::ast::Span& span_, BoundExpressionNode* condition_, BoundStatementNode* thenStatement_, BoundStatementNode* elseStatement_);
    BoundExpressionNode* Condition() const { return condition.get(); }
    BoundStatementNode* ThenStatement() const { return thenStatement.get(); }
    BoundStatementNode* ElseStatement() const { return elseStatement.get(); }
    void Accept(BoundNodeVisitor& visitor) override;
private:
    std::unique_ptr<BoundExpressionNode> condition;
    std::unique_ptr<BoundStatementNode> thenStatement;
    std::unique_ptr<BoundStatementNode> elseStatement;
};

class BoundReturnFunctionResultStatementNode : public BoundStatementNode
{
public:
    BoundReturnFunctionResultStatementNode(const soul::ast::Span& span_, BoundFunctionResultNode* functionResult_);
    BoundFunctionResultNode* FunctionResult() const { return functionResult.get(); }
    void Accept(BoundNodeVisitor& visitor) override;
private:
    std::unique_ptr<BoundFunctionResultNode> functionResult;
};

class BoundRangeNode : public BoundNode
{
public:
    BoundRangeNode(const soul::ast::Span& span_, Value* rangeStart_, Value* rangeEnd_);
    Value* RangeStart() const { return rangeStart.get(); }
    Value* RangeEnd() const { return rangeEnd.get(); }
    void Accept(BoundNodeVisitor& visitor) override;
private:
    std::unique_ptr<Value> rangeStart;
    std::unique_ptr<Value> rangeEnd;
};

class BoundCaseNode : public BoundNode
{
public:
    BoundCaseNode(const soul::ast::Span& span_);
    const std::vector<std::unique_ptr<BoundRangeNode>>& Ranges() const { return ranges; }
    void AddRange(BoundRangeNode* range);
    BoundStatementNode* Statement() const { return statement.get(); }
    void SetStatement(BoundStatementNode* statement_);
    void Accept(BoundNodeVisitor& visitor) override;
private:
    std::vector<std::unique_ptr<BoundRangeNode>> ranges;
    std::unique_ptr<BoundStatementNode> statement;
};

class BoundCaseStatementNode : public BoundStatementNode
{
public:
    BoundCaseStatementNode(const soul::ast::Span& span_);
    BoundExpressionNode* Condition() const { return condition.get(); }
    void SetCondition(BoundExpressionNode* condition_);
    void AddCase(BoundCaseNode* caseNode);
    const std::vector<std::unique_ptr<BoundCaseNode>>& Cases() const { return cases; }
    BoundStatementNode* ElsePart() const { return elsePart.get(); }
    void SetElsePart(BoundStatementNode* elsePart_);
    void Accept(BoundNodeVisitor& visitor) override;
private:
    std::unique_ptr<BoundExpressionNode> condition;
    std::vector<std::unique_ptr<BoundCaseNode>> cases;
    std::unique_ptr<BoundStatementNode> elsePart;
};

class BoundRepeatStatementNode : public BoundStatementNode
{
public:
    BoundRepeatStatementNode(const soul::ast::Span& span_);
    void AddStatement(BoundStatementNode* statement);
    const std::vector<std::unique_ptr<BoundStatementNode>>& Statements() const { return statements; }
    BoundExpressionNode* Condition() const { return condition.get(); }
    void SetCondition(BoundExpressionNode* condition_);
    void Accept(BoundNodeVisitor& visitor) override;
private:
    std::vector<std::unique_ptr<BoundStatementNode>> statements;
    std::unique_ptr<BoundExpressionNode> condition;
};

class BoundWhileStatementNode : public BoundStatementNode
{
public:
    BoundWhileStatementNode(const soul::ast::Span& span_, BoundExpressionNode* condition_, BoundStatementNode* statement_);
    BoundExpressionNode* Condition() const { return condition.get(); }
    BoundStatementNode* Statement() const { return statement.get(); }
    void Accept(BoundNodeVisitor& visitor) override;
private:
    std::unique_ptr<BoundExpressionNode> condition;
    std::unique_ptr<BoundStatementNode> statement;
};

} // namespace p
