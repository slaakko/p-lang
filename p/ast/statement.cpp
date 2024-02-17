// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.ast.statement;

import p.ast.visitor;
import p.ast.writer;
import p.ast.reader;
import p.ast.common;
import p.ast.error;

namespace p {

StatementNode::StatementNode(NodeKind kind_, const soul::ast::Span& span_) : Node(kind_, span_)
{
}

AssignmentStatementNode::AssignmentStatementNode(const soul::ast::Span& span_) : StatementNode(NodeKind::assignmentStatementNode, span_)
{
}

AssignmentStatementNode::AssignmentStatementNode(const soul::ast::Span& span_, Node* target_, Node* source_) : 
    StatementNode(NodeKind::assignmentStatementNode, span_), target(target_), source(source_)
{
    target->SetParent(this);
    source->SetParent(this);
}

Node* AssignmentStatementNode::Clone() const
{
    return new AssignmentStatementNode(Span(), target->Clone(), source->Clone());
}

void AssignmentStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void AssignmentStatementNode::Write(AstWriter& writer)
{
    writer.WriteNode(target.get());
    writer.WriteNode(source.get());
}

void AssignmentStatementNode::Read(AstReader& reader)
{
    target.reset(reader.ReadNode());
    target->SetParent(this);
    source.reset(reader.ReadNode());
    source->SetParent(this);
}

ExpressionStatementNode::ExpressionStatementNode(const soul::ast::Span& span_) : StatementNode(NodeKind::expressionStatementNode, span_)
{
}

ExpressionStatementNode::ExpressionStatementNode(const soul::ast::Span& span_, Node* expr_) : StatementNode(NodeKind::expressionStatementNode, span_), expr(expr_)
{
    expr->SetParent(this);
}

Node* ExpressionStatementNode::Clone() const
{
    return new ExpressionStatementNode(Span(), expr->Clone());
}

void ExpressionStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ExpressionStatementNode::Write(AstWriter& writer)
{
    writer.WriteNode(expr.get());
}

void ExpressionStatementNode::Read(AstReader& reader)
{
    expr.reset(reader.ReadNode());
    expr->SetParent(this);
}

EmptyStatementNode::EmptyStatementNode(const soul::ast::Span& span_) : StatementNode(NodeKind::emptyStatementNode, span_)
{
}

Node* EmptyStatementNode::Clone() const
{
    return new EmptyStatementNode(Span());
}

void EmptyStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void EmptyStatementNode::Write(AstWriter& writer)
{
}

void EmptyStatementNode::Read(AstReader& reader)
{
}

CompoundStatementNode::CompoundStatementNode(const soul::ast::Span& span_) : StatementNode(NodeKind::compoundStatementNode, span_)
{
}

void CompoundStatementNode::AddStatement(StatementNode* statement)
{
    statement->SetParent(this);
    statements.push_back(std::unique_ptr<StatementNode>(statement));
}

Node* CompoundStatementNode::Clone() const
{
    CompoundStatementNode* clone = new CompoundStatementNode(Span());
    for (const auto& statement : statements)
    {
        clone->AddStatement(static_cast<StatementNode*>(statement->Clone()));
    }
    return clone;
}

void CompoundStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void CompoundStatementNode::Write(AstWriter& writer)
{
    int32_t n = statements.size();
    writer.GetBinaryWriter().Write(n);
    for (const auto& statement : statements)
    {
        writer.WriteNode(statement.get());
    }
}

void CompoundStatementNode::Read(AstReader& reader)
{
    int32_t n = reader.GetBinaryReader().ReadInt();
    for (int32_t i = 0; i < n; ++i)
    {
        StatementNode* statement = static_cast<StatementNode*>(reader.ReadNode());
        AddStatement(statement);
    }
}

const std::string& CompoundStatementNode::FilePath() const
{
    if (!filePath.empty()) return filePath;
    return StatementNode::FilePath();
}

void CompoundStatementNode::SetFilePath(const std::string& filePath_)
{
    filePath = filePath_;
}

IfStatementNode::IfStatementNode(const soul::ast::Span& span_) : StatementNode(NodeKind::ifStatementNode, span_)
{
}

IfStatementNode::IfStatementNode(const soul::ast::Span& span_, Node* condition_, StatementNode* thenStatement_) : 
    StatementNode(NodeKind::ifStatementNode, span_), condition(condition_), thenStatement(thenStatement_)
{
    condition->SetParent(this);
    thenStatement->SetParent(this);
}

void IfStatementNode::SetElseStatement(StatementNode* elseStatement_)
{
    elseStatement.reset(elseStatement_);
    elseStatement->SetParent(this);
}

Node* IfStatementNode::Clone() const
{
    IfStatementNode* clone = new IfStatementNode(Span(), condition->Clone(), static_cast<StatementNode*>(thenStatement->Clone()));
    if (elseStatement)
    {
        clone->SetElseStatement(static_cast<StatementNode*>(elseStatement->Clone()));
    }
    return clone;
}

void IfStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void IfStatementNode::Write(AstWriter& writer)
{
    writer.WriteNode(condition.get());
    writer.WriteNode(thenStatement.get());
    writer.WriteNode(elseStatement.get());
}

void IfStatementNode::Read(AstReader& reader)
{
    condition.reset(reader.ReadNode());
    thenStatement.reset(static_cast<StatementNode*>(reader.ReadNode()));
    thenStatement->SetParent(this);
    elseStatement.reset(static_cast<StatementNode*>(reader.ReadNode()));
    if (elseStatement)
    {
        elseStatement->SetParent(this);
    }
}

CaseNode::CaseNode(const soul::ast::Span& span_) : Node(NodeKind::caseNode, span_)
{
}

void CaseNode::AddRange(Node* range)
{
    range->SetParent(this);
    ranges.push_back(std::unique_ptr<Node>(range));
}

void CaseNode::SetStatement(StatementNode* statement_)
{
    statement.reset(statement_);
    statement->SetParent(this);
}

Node* CaseNode::Clone() const
{
    CaseNode* clone = new CaseNode(Span());
    for (const auto& range : ranges)
    {
        clone->AddRange(range->Clone());
    }
    clone->SetStatement(static_cast<StatementNode*>(statement->Clone()));
    return clone;
}

void CaseNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void CaseNode::Write(AstWriter& writer)
{
    int32_t n = ranges.size();
    writer.GetBinaryWriter().Write(n);
    for (const auto& range : ranges)
    {
        writer.WriteNode(range.get());
    }
    writer.WriteNode(statement.get());
}

void CaseNode::Read(AstReader& reader)
{
    int32_t n = reader.GetBinaryReader().ReadInt();
    for (int32_t i = 0; i < n; ++i)
    {
        Node* range = reader.ReadNode();
        AddRange(range);
    }
    statement.reset(static_cast<StatementNode*>(reader.ReadNode()));
    statement->SetParent(this);
}

ConstantRangeNode::ConstantRangeNode(const soul::ast::Span& span_) : Node(NodeKind::constantRangeNode, span_)
{
}

ConstantRangeNode::ConstantRangeNode(const soul::ast::Span& span_, Node* start_, Node* end_) : Node(NodeKind::constantRangeNode, span_), rangeStart(start_), rangeEnd(end_)
{
    rangeStart->SetParent(this);
    if (rangeEnd)
    {
        rangeEnd->SetParent(this);
    }
}

Node* ConstantRangeNode::Clone() const
{
    Node* clonedEnd = nullptr;
    if (rangeEnd)
    {
        clonedEnd = rangeEnd->Clone();
    }
    return new ConstantRangeNode(Span(), rangeStart->Clone(), clonedEnd);
}

void ConstantRangeNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ConstantRangeNode::Write(AstWriter& writer)
{
    writer.WriteNode(rangeStart.get());
    writer.WriteNode(rangeEnd.get());
}

void ConstantRangeNode::Read(AstReader& reader)
{
    rangeStart.reset(reader.ReadNode());
    rangeStart->SetParent(this);
    rangeEnd.reset(reader.ReadNode());
    if (rangeEnd)
    {
        rangeEnd->SetParent(this);
    }
}

CaseStatementNode::CaseStatementNode(const soul::ast::Span& span_) : StatementNode(NodeKind::caseStatementNode, span_)
{
}

CaseStatementNode::CaseStatementNode(const soul::ast::Span& span_, Node* condition_) : StatementNode(NodeKind::caseStatementNode, span_), condition(condition_)
{
    condition->SetParent(this);
}

void CaseStatementNode::AddCase(CaseNode* caseNode)
{
    caseNode->SetParent(this);
    cases.push_back(std::unique_ptr<CaseNode>(caseNode));
}

void CaseStatementNode::SetElsePart(StatementNode* elsePart_)
{
    elsePart.reset(elsePart_);
    elsePart->SetParent(this);
}

Node* CaseStatementNode::Clone() const
{
    CaseStatementNode* clone = new CaseStatementNode(Span(), condition->Clone());
    for (const auto& caseNode : cases)
    {
        clone->AddCase(static_cast<CaseNode*>(caseNode->Clone()));
    }
    if (elsePart)
    {
        clone->SetElsePart(static_cast<StatementNode*>(elsePart->Clone()));
    }
    return clone;
}

void CaseStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void CaseStatementNode::Write(AstWriter& writer)
{
    writer.WriteNode(condition.get());
    int32_t n = cases.size();
    writer.GetBinaryWriter().Write(n);
    for (const auto& caseNode : cases)
    {
        writer.WriteNode(caseNode.get());
    }
    writer.WriteNode(elsePart.get());
}

void CaseStatementNode::Read(AstReader& reader)
{
    condition.reset(reader.ReadNode());
    condition->SetParent(this);
    int32_t n = reader.GetBinaryReader().ReadInt();
    for (int32_t i = 0; i < n; ++i)
    {
        CaseNode* caseNode = static_cast<CaseNode*>(reader.ReadNode());
        AddCase(caseNode);
    }
    elsePart.reset(static_cast<StatementNode*>(reader.ReadNode()));
    if (elsePart)
    {
        elsePart->SetParent(this);
    }
}

RepeatStatementNode::RepeatStatementNode(const soul::ast::Span& span_) : StatementNode(NodeKind::repeatStatementNode, span_)
{
}

void RepeatStatementNode::AddStatement(StatementNode* statement)
{
    statement->SetParent(this);
    statements.push_back(std::unique_ptr<StatementNode>(statement));
}

void RepeatStatementNode::SetCondition(Node* condition_)
{
    condition.reset(condition_);
    condition->SetParent(this);
}

Node* RepeatStatementNode::Clone() const 
{
    RepeatStatementNode* clone = new RepeatStatementNode(Span());
    for (const auto& statement : statements)
    {
        clone->AddStatement(static_cast<StatementNode*>(statement->Clone()));
    }
    clone->SetCondition(condition->Clone());
    return clone;
}

void RepeatStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void RepeatStatementNode::Write(AstWriter& writer)
{
    int32_t n = statements.size();
    writer.GetBinaryWriter().Write(n);
    for (const auto& statement : statements)
    {
        writer.WriteNode(statement.get());
    }
    writer.WriteNode(condition.get());
}

void RepeatStatementNode::Read(AstReader& reader)
{
    int32_t n = reader.GetBinaryReader().ReadInt();
    for (int32_t i = 0; i < n; ++i)
    {
        AddStatement(static_cast<StatementNode*>(reader.ReadNode()));
    }
    SetCondition(reader.ReadNode());
}

WhileStatementNode::WhileStatementNode(const soul::ast::Span& span_) : StatementNode(NodeKind::whileStatementNode, span_)
{
}

WhileStatementNode::WhileStatementNode(const soul::ast::Span& span_, Node* condition_, StatementNode* statement_) : 
    StatementNode(NodeKind::whileStatementNode, span_), condition(condition_), statement(statement_)
{
    condition->SetParent(this);
    statement->SetParent(this);
}

Node* WhileStatementNode::Clone() const
{
    return new WhileStatementNode(Span(), condition->Clone(), static_cast<StatementNode*>(statement->Clone()));
}

void WhileStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void WhileStatementNode::Write(AstWriter& writer)
{
    writer.WriteNode(condition.get());
    writer.WriteNode(statement.get());
}

void WhileStatementNode::Read(AstReader& reader)
{
    condition.reset(reader.ReadNode());
    condition->SetParent(this);
    statement.reset(static_cast<StatementNode*>(reader.ReadNode()));
    statement->SetParent(this);
}

ForStatementNode::ForStatementNode(const soul::ast::Span& span_) : StatementNode(NodeKind::forStatementNode, span_)
{
}

ForStatementNode::ForStatementNode(const soul::ast::Span& span_, IdentifierNode* controlVariable_, Node* initialValue_, Direction direction_, Node* finalValue_, 
    StatementNode* statement_) : 
    StatementNode(NodeKind::forStatementNode, span_), controlVariable(controlVariable_), initialValue(initialValue_), direction(direction_), finalValue(finalValue_),
    statement(statement_)
{
    controlVariable->SetParent(this);
    initialValue->SetParent(this);
    finalValue->SetParent(this);
    statement->SetParent(this);
}

Node* ForStatementNode::Clone() const
{
    return new ForStatementNode(Span(), static_cast<IdentifierNode*>(controlVariable->Clone()), initialValue->Clone(), direction, finalValue->Clone(),
        static_cast<StatementNode*>(statement->Clone()));
}

void ForStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ForStatementNode::Write(AstWriter& writer)
{
    writer.WriteNode(controlVariable.get());
    writer.WriteNode(initialValue.get());
    writer.GetBinaryWriter().Write(static_cast<uint8_t>(direction));
    writer.WriteNode(finalValue.get());
    writer.WriteNode(statement.get());
}

void ForStatementNode::Read(AstReader& reader)
{
    controlVariable.reset(static_cast<IdentifierNode*>(reader.ReadNode()));
    controlVariable->SetParent(this);
    initialValue.reset(reader.ReadNode());
    initialValue->SetParent(this);
    direction = static_cast<Direction>(reader.GetBinaryReader().ReadByte());
    finalValue.reset(reader.ReadNode());
    finalValue->SetParent(this);
    statement.reset(static_cast<StatementNode*>(reader.ReadNode()));
    statement->SetParent(this);
}

} // namespace p
