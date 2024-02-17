// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.binder.bound_node_visitor;

import std.core;

export namespace p {

class BoundExpressionStatementNode;
class BoundEmptyStatementNode;
class BoundAssignmentStatementNode;
class BoundCompoundStatementNode;
class BoundIfStatementNode;
class BoundReturnFunctionResultStatementNode;
class BoundRangeNode;
class BoundCaseNode;
class BoundCaseStatementNode;
class BoundRepeatStatementNode;
class BoundWhileStatementNode;
class BoundBinaryExpressionNode;
class BoundUnaryExpressionNode;
class BoundParameterNode;
class BoundLiteralNode;
class BoundConstantNode;
class BoundVariableNode;
class BoundProcedureNode;
class BoundFunctionNode;
class BoundMethodNode;
class BoundSubroutineCallNode;
class BoundConstructorCallNode;
class BoundIndexExprNode;
class BoundMemberExprNode;
class BoundNewExpressionNode;
class BoundNewArrayExpressionNode;
class BoundArrayLengthNode;
class BoundStringLengthNode;
class BoundTypeNode;
class BoundConversionNode;
class BoundValueConversionNode;
class BoundFunctionResultNode;

class BoundNodeVisitor
{
public:
    BoundNodeVisitor();
    virtual ~BoundNodeVisitor();
    virtual void Visit(BoundExpressionStatementNode& node) {}
    virtual void Visit(BoundEmptyStatementNode& node) {}
    virtual void Visit(BoundAssignmentStatementNode& node) {}
    virtual void Visit(BoundCompoundStatementNode& node) {}
    virtual void Visit(BoundIfStatementNode& node) {}
    virtual void Visit(BoundReturnFunctionResultStatementNode& node) {}
    virtual void Visit(BoundRangeNode& node) {}
    virtual void Visit(BoundCaseNode& node) {}
    virtual void Visit(BoundCaseStatementNode& node) {}
    virtual void Visit(BoundRepeatStatementNode& node) {}
    virtual void Visit(BoundWhileStatementNode& node) {}
    virtual void Visit(BoundBinaryExpressionNode& node) {}
    virtual void Visit(BoundUnaryExpressionNode& node) {}
    virtual void Visit(BoundParameterNode& node) {}
    virtual void Visit(BoundLiteralNode& node) {}
    virtual void Visit(BoundConstantNode& node) {}
    virtual void Visit(BoundVariableNode& node) {}
    virtual void Visit(BoundProcedureNode& node) {}
    virtual void Visit(BoundFunctionNode& node) {}
    virtual void Visit(BoundMethodNode& node) {}
    virtual void Visit(BoundSubroutineCallNode& node) {}
    virtual void Visit(BoundConstructorCallNode& node) {}
    virtual void Visit(BoundIndexExprNode& node) {}
    virtual void Visit(BoundMemberExprNode& node) {}
    virtual void Visit(BoundNewExpressionNode& node) {}
    virtual void Visit(BoundNewArrayExpressionNode& node) {}
    virtual void Visit(BoundArrayLengthNode& node) {}
    virtual void Visit(BoundStringLengthNode& node) {}
    virtual void Visit(BoundTypeNode& node) {}
    virtual void Visit(BoundConversionNode& node) {}
    virtual void Visit(BoundValueConversionNode& node) {}
    virtual void Visit(BoundFunctionResultNode& node) {}
};

} // namespace p
