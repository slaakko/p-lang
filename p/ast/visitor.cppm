// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.ast.visitor;

import std.core;

export namespace p {

class UnitNameNode;
class UnitNameListNode;
class IdentifierNode;
class IdentifierListNode;
class ProgramNode;
class UnitNode;
class UnitPartNode;
class BinaryExprNode;
class UnaryExprNode;
class ParenthesizedExprNode;
class IntegerLiteralNode;
class RealLiteralNode;
class BooleanLiteralNode;
class StringLiteralNode;
class CharLiteralNode;
class ThisNode;
class BaseNode;
class NilNode;
class NewExprNode;
class NewArrayExprNode;
class InvokeExprNode;
class IndexExprNode;
class DotNode;
class AssignmentStatementNode;
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
class IntegerNode;
class RealNode;
class CharNode;
class BooleanNode;
class StringNode;
class PointerNode;
class BlockNode;
class ConstantDeclarationNode;
class ArrayConstantNode;
class ObjectConstantNode;
class FieldNode;
class TypeDeclarationNode;
class VariableDeclarationNode;
class SubrangeTypeNode;
class EnumeratedTypeNode;
class FieldListNode;
class SubroutineIdNode;
class ParameterListNode;
class ProcedureHeadingNode;
class FunctionHeadingNode;
class ConstructorHeadingNode;
class ProcedureDeclarationNode;
class FunctionDeclarationNode;
class ConstructorDeclarationNode;
class ConstructorCallNode;
class ForwardNode;
class ExternalNode;
class ObjectTypeNode;
class ArrayTypeNode;
class SpecializationNode;

class Visitor
{
public:
    virtual ~Visitor();
    virtual void Visit(UnitNameNode& node) {}
    virtual void Visit(UnitNameListNode& node) {}
    virtual void Visit(IdentifierNode& node) {}
    virtual void Visit(IdentifierListNode& node) {}
    virtual void Visit(ProgramNode& node) {}
    virtual void Visit(UnitNode& node) {}
    virtual void Visit(UnitPartNode& node) {}
    virtual void Visit(BinaryExprNode& node) {}
    virtual void Visit(UnaryExprNode& node) {}
    virtual void Visit(ParenthesizedExprNode& node) {}
    virtual void Visit(IntegerLiteralNode& node) {}
    virtual void Visit(RealLiteralNode& node) {}
    virtual void Visit(BooleanLiteralNode& node) {}
    virtual void Visit(StringLiteralNode& node) {}
    virtual void Visit(CharLiteralNode& node) {}
    virtual void Visit(ThisNode& node) {}
    virtual void Visit(BaseNode& node) {}
    virtual void Visit(NilNode& node) {}
    virtual void Visit(NewExprNode& node) {}
    virtual void Visit(NewArrayExprNode& node) {}
    virtual void Visit(InvokeExprNode& node) {}
    virtual void Visit(IndexExprNode& node) {}
    virtual void Visit(DotNode& node) {}
    virtual void Visit(AssignmentStatementNode& node) {}
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
    virtual void Visit(IntegerNode& node) {}
    virtual void Visit(RealNode& node) {}
    virtual void Visit(CharNode& node) {}
    virtual void Visit(BooleanNode& node) {}
    virtual void Visit(StringNode& node) {}
    virtual void Visit(PointerNode& node) {}
    virtual void Visit(BlockNode& node) {}
    virtual void Visit(ConstantDeclarationNode& node) {}
    virtual void Visit(ArrayConstantNode& node) {}
    virtual void Visit(ObjectConstantNode& node) {}
    virtual void Visit(FieldNode& node) {}
    virtual void Visit(TypeDeclarationNode& node) {}
    virtual void Visit(VariableDeclarationNode& node) {}
    virtual void Visit(SubrangeTypeNode& node) {}
    virtual void Visit(EnumeratedTypeNode& node) {}
    virtual void Visit(FieldListNode& node) {}
    virtual void Visit(SubroutineIdNode& node) {}
    virtual void Visit(ParameterListNode& node) {}
    virtual void Visit(ProcedureHeadingNode& node) {}
    virtual void Visit(FunctionHeadingNode& node) {}
    virtual void Visit(ConstructorHeadingNode& node) {}
    virtual void Visit(ProcedureDeclarationNode& node) {}
    virtual void Visit(FunctionDeclarationNode& node) {}
    virtual void Visit(ConstructorDeclarationNode& node) {}
    virtual void Visit(ConstructorCallNode& node) {}
    virtual void Visit(ForwardNode& node) {}
    virtual void Visit(ExternalNode& node) {}
    virtual void Visit(ObjectTypeNode& node) {}
    virtual void Visit(ArrayTypeNode& node) {}
    virtual void Visit(SpecializationNode& node) {}
};

} // namespace p
