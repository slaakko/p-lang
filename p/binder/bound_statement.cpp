// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.binder.bound_statement;

import p.binder.bound_node_visitor;

namespace p {

BoundStatementNode::BoundStatementNode(BoundNodeKind kind_, const soul::ast::Span& span_) : BoundNode(kind_, span_)
{
}

BoundExpressionStatementNode::BoundExpressionStatementNode(const soul::ast::Span& span_, BoundExpressionNode* expression_) : 
    BoundStatementNode(BoundNodeKind::boundExpressionStatementNode, span_), expression(expression_), pop(true)
{
    expression->SetParent(this);
}

void BoundExpressionStatementNode::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundEmptyStatementNode::BoundEmptyStatementNode(const soul::ast::Span& span_) : BoundStatementNode(BoundNodeKind::boundEmptyStatementNode, span_)
{
}

void BoundEmptyStatementNode::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundAssignmentStatementNode::BoundAssignmentStatementNode(const soul::ast::Span& span_, BoundExpressionNode* target_, BoundExpressionNode* source_) : 
    BoundStatementNode(BoundNodeKind::boundAssignmentStatementNode, span_), target(target_), source(source_)
{
    source->SetParent(this);
    target->SetParent(this);
}

void BoundAssignmentStatementNode::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundCompoundStatementNode::BoundCompoundStatementNode(const soul::ast::Span& span_) : BoundStatementNode(BoundNodeKind::boundCompoundStatementNode, span_)
{
}

void BoundCompoundStatementNode::AddStatement(BoundStatementNode* statement)
{
    statement->SetParent(this);
    statements.push_back(std::unique_ptr<BoundStatementNode>(statement));
}

void BoundCompoundStatementNode::SetFilePath(const std::string& filePath_)
{
    filePath = filePath_;
}

const std::string& BoundCompoundStatementNode::FilePath() const
{
    BoundNode* parent = Parent();
    if (parent)
    {
        return parent->FilePath();
    }
    else
    {
        return filePath;
    }
}

void BoundCompoundStatementNode::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundIfStatementNode::BoundIfStatementNode(const soul::ast::Span& span_, BoundExpressionNode* condition_, BoundStatementNode* thenStatement_, BoundStatementNode* elseStatement_) : 
    BoundStatementNode(BoundNodeKind::boundIfStatementNode, span_), condition(condition_), thenStatement(thenStatement_), elseStatement(elseStatement_)
{
    condition->SetParent(this);
    thenStatement->SetParent(this);
    if (elseStatement)
    {
        elseStatement->SetParent(this);
    }
}

void BoundIfStatementNode::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundReturnFunctionResultStatementNode::BoundReturnFunctionResultStatementNode(const soul::ast::Span& span_, BoundFunctionResultNode* functionResult_) : 
    BoundStatementNode(BoundNodeKind::boundReturnFunctionResultStatementNode, span_), functionResult(functionResult_)
{
    functionResult->SetParent(this);
}

void BoundReturnFunctionResultStatementNode::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundRangeNode::BoundRangeNode(const soul::ast::Span& span_, Value* rangeStart_, Value* rangeEnd_) :
    BoundNode(BoundNodeKind::boundRangeNode, span_), rangeStart(rangeStart_), rangeEnd(rangeEnd_)
{
}

void BoundRangeNode::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundCaseNode::BoundCaseNode(const soul::ast::Span& span_) : BoundNode(BoundNodeKind::boundCaseNode, span_)
{
}

void BoundCaseNode::AddRange(BoundRangeNode* range)
{
    range->SetParent(this);
    ranges.push_back(std::unique_ptr<BoundRangeNode>(range));
}

void BoundCaseNode::SetStatement(BoundStatementNode* statement_)
{
    statement.reset(statement_);
    statement->SetParent(this);
}

void BoundCaseNode::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundCaseStatementNode::BoundCaseStatementNode(const soul::ast::Span& span_) : BoundStatementNode(BoundNodeKind::boundCaseStatementNode, span_)
{
}

void BoundCaseStatementNode::SetCondition(BoundExpressionNode* condition_)
{
    condition.reset(condition_);
    condition->SetParent(this);
}

void BoundCaseStatementNode::AddCase(BoundCaseNode* caseNode)
{
    caseNode->SetParent(this);
    cases.push_back(std::unique_ptr<BoundCaseNode>(caseNode));
}

void BoundCaseStatementNode::SetElsePart(BoundStatementNode* elsePart_)
{
    elsePart.reset(elsePart_);
    elsePart->SetParent(this);
}

void BoundCaseStatementNode::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundRepeatStatementNode::BoundRepeatStatementNode(const soul::ast::Span& span_) : BoundStatementNode(BoundNodeKind::boundRepeatStatementNode, span_)
{
}

void BoundRepeatStatementNode::AddStatement(BoundStatementNode* statement)
{
    statement->SetParent(this);
    statements.push_back(std::unique_ptr<BoundStatementNode>(statement));
}

void BoundRepeatStatementNode::SetCondition(BoundExpressionNode* condition_)
{
    condition.reset(condition_);
    condition->SetParent(this);
}

void BoundRepeatStatementNode::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundWhileStatementNode::BoundWhileStatementNode(const soul::ast::Span& span_, BoundExpressionNode* condition_, BoundStatementNode* statement_) : 
    BoundStatementNode(BoundNodeKind::boundWhileStatementNode, span_), condition(condition_), statement(statement_)
{
    condition->SetParent(this);
    statement->SetParent(this);
}

void BoundWhileStatementNode::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

} // namespace p
