// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.binder.bound_node;

import p.core;
import p.ast;
import soul.ast.span;
import std.core;

export namespace p {

class BoundNodeVisitor;

enum class BoundNodeKind
{
    boundCompoundStatementNode, boundEmptyStatementNode, boundAssignmentStatementNode, boundExpressionStatementNode,
    boundIfStatementNode, boundWhileStatementNode, boundRepeatStatementNode, boundRangeNode, boundCaseNode, boundCaseStatementNode, boundReturnFunctionResultStatementNode,
    boundBinaryExprNode, boundUnaryExprNode, boundLiteralNode, boundParameterNode, boundConstantNode, boundVariableNode, 
    boundProcedureNode, boundFunctionNode, boundMethodNode, 
    boundSubroutineCallNode, boundConstructorCallNode, boundIndexExprNode, boundMemberExprNode, boundNewExpressionNode, boundNewArrayExpressionNode,
    boundArrayLengthNode, boundStringLengthNode, boundTypeNode, boundConversionNode, boundValueConversionNode, boundFunctionResultNode,
};

class BoundNode
{
public:
    BoundNode(BoundNodeKind kind_, const soul::ast::Span& span_);
    virtual ~BoundNode();
    BoundNodeKind Kind() const { return kind; }
    const soul::ast::Span& Span() const { return span; }
    bool IsBoundProcedureNode() const { return kind == BoundNodeKind::boundProcedureNode; }
    bool IsBoundFunctionNode() const { return kind == BoundNodeKind::boundFunctionNode; }
    bool IsBoundMethodNode() const { return kind == BoundNodeKind::boundMethodNode; }
    bool IsBoundTypeNode() const { return kind == BoundNodeKind::boundTypeNode; }
    bool IsBoundNewExpressionNode() const { return kind == BoundNodeKind::boundNewExpressionNode; }
    bool IsBoundArrayLengthNode() const { return kind == BoundNodeKind::boundArrayLengthNode; }
    bool IsBoundStringLengthNode() const { return kind == BoundNodeKind::boundStringLengthNode; }
    bool IsBoundSubroutineCallNode() const { return kind == BoundNodeKind::boundSubroutineCallNode; }
    void SetParent(BoundNode* parent_) { parent = parent_; }
    BoundNode* Parent() const { return parent; }
    virtual const std::string& FilePath() const;
    virtual void Accept(BoundNodeVisitor& visitor) = 0;
private:
    BoundNodeKind kind;
    soul::ast::Span span;
    BoundNode* parent;
};

} // namespace p
