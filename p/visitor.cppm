// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.visitor;

import std.core;

export namespace p {

class BinaryExprNode;
class UnaryExprNode;
class ParenthesizedExprNode;
class IntegerLiteralNode;
class RealLiteralNode;
class BooleanLiteralNode;
class StringLiteralNode;
class CharLiteralNode;
class IdentifierNode;
class QualifiedIdNode;
class FieldNode;
class MethodNode;
class ThisNode;
class BaseNode;
class NilNode;
class NewExprNode;
class NewArrayExprNode;
class ValueTypecastNode;
class VariableTypecastNode;
class InvokeExprNode;
class IndexExprNode;
class DotNode;
class AssignmentStatementNode;
class ProcedureCallStatementNode;
class ExpressionStatementNode;
class EmptyStatementNode;
class CompoundStatementNode;
class IfStatementNode;
class CaseNode;
class ConstantRangeNode;
class CaseStatementNode;
class RepeatStatementNode;
class WhileStatementNode;
class ForStatementNode;

class Visitor
{
public:
    Visitor();
    virtual ~Visitor();
    virtual void Visit(BinaryExprNode& node) {}
    virtual void Visit(UnaryExprNode& node) {}
    virtual void Visit(ParenthesizedExprNode& node) {}
    virtual void Visit(IntegerLiteralNode& node) {}
    virtual void Visit(RealLiteralNode& node) {}
    virtual void Visit(BooleanLiteralNode& node) {}
    virtual void Visit(StringLiteralNode& node) {}
    virtual void Visit(CharLiteralNode& node) {}
    virtual void Visit(IdentifierNode& node) {}
    virtual void Visit(QualifiedIdNode& node) {}
    virtual void Visit(FieldNode& node) {}
    virtual void Visit(MethodNode& node) {}
    virtual void Visit(ThisNode& node) {}
    virtual void Visit(BaseNode& node) {}
    virtual void Visit(NilNode& node) {}
    virtual void Visit(NewExprNode& node) {}
    virtual void Visit(NewArrayExprNode& node) {}
    virtual void Visit(ValueTypecastNode& node) {}
    virtual void Visit(VariableTypecastNode& node) {}
    virtual void Visit(InvokeExprNode& node) {}
    virtual void Visit(IndexExprNode& node) {}
    virtual void Visit(DotNode& node) {}
    virtual void Visit(AssignmentStatementNode& node) {}
    virtual void Visit(ProcedureCallStatementNode& node) {}
    virtual void Visit(ExpressionStatementNode& node) {}
    virtual void Visit(EmptyStatementNode& node) {}
    virtual void Visit(CompoundStatementNode& node) {}
    virtual void Visit(IfStatementNode& node) {}
    virtual void Visit(CaseNode& node) {}
    virtual void Visit(ConstantRangeNode& node) {}
    virtual void Visit(CaseStatementNode& node) {}
    virtual void Visit(RepeatStatementNode& node) {}
    virtual void Visit(WhileStatementNode& node) {}
    virtual void Visit(ForStatementNode& node) {}
};

} // namespace p
