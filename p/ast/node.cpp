// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.ast.node;

import p.ast.error;
import p.ast.program;
import p.ast.unit;
import p.ast.constant;
import p.ast.expression;
import p.ast.statement;
import p.ast.type;
import p.ast.variable;

namespace p {

Node::Node(NodeKind kind_, const soul::ast::Span& span_) : kind(kind_), span(span_), parent(nullptr)
{
}

Node::~Node()
{
}

const std::string& Node::FilePath() const
{
    if (parent)
    {
        return parent->FilePath();
    }
    else
    {
        static std::string empty;
        return empty;
    }
}

void Node::AddNode(Node* node)
{
    ThrowError("error: cannot add node", FilePath(), node->Span());
}

Node* CreateNode(NodeKind kind, const soul::ast::Span& span)
{
    switch (kind)
    {
        case NodeKind::programNode: return new ProgramNode(span);
        case NodeKind::unitNode: return new UnitNode(span);
        case NodeKind::unitPartNode: return new UnitPartNode(span);
        case NodeKind::unitNameNode: return new UnitNameNode(span);
        case NodeKind::unitNameListNode: return new UnitNameListNode(span);
        case NodeKind::identifierNode: return new IdentifierNode(span);
        case NodeKind::identifierListNode: return new IdentifierListNode(span);
        case NodeKind::constantDeclarationNode: return new ConstantDeclarationNode(span);
        case NodeKind::arrayConstantNode: return new ArrayConstantNode(span);
        case NodeKind::objectConstantNode: return new ObjectConstantNode(span);
        case NodeKind::fieldNode: return new FieldNode(span);
        case NodeKind::binaryExprNode: return new BinaryExprNode(span);
        case NodeKind::unaryExprNode: return new UnaryExprNode(span);
        case NodeKind::parenthesizedExprNode: return new ParenthesizedExprNode(span);
        case NodeKind::integerLiteralNode: return new IntegerLiteralNode(span);
        case NodeKind::realLiteralNode: return new RealLiteralNode(span);
        case NodeKind::booleanLiteralNode: return new BooleanLiteralNode(span);
        case NodeKind::stringLiteralNode: return new StringLiteralNode(span);
        case NodeKind::charLiteralNode: return new CharLiteralNode(span);
        case NodeKind::thisNode: return new ThisNode(span);
        case NodeKind::baseNode: return new BaseNode(span);
        case NodeKind::nilNode: return new NilNode(span);
        case NodeKind::newExprNode: return new NewExprNode(span);
        case NodeKind::newArrayExprNode: return new NewArrayExprNode(span);
        case NodeKind::invokeExprNode: return new InvokeExprNode(span);
        case NodeKind::indexExprNode: return new IndexExprNode(span);
        case NodeKind::dotNode: return new DotNode(span);
        case NodeKind::assignmentStatementNode: return new AssignmentStatementNode(span);
        case NodeKind::expressionStatementNode: return new ExpressionStatementNode(span);
        case NodeKind::emptyStatementNode: return new EmptyStatementNode(span);
        case NodeKind::compoundStatementNode: return new CompoundStatementNode(span);
        case NodeKind::ifStatementNode: return new IfStatementNode(span);
        case NodeKind::caseNode: return new CaseNode(span);
        case NodeKind::constantRangeNode: return new ConstantRangeNode(span);
        case NodeKind::caseStatementNode: return new CaseStatementNode(span);
        case NodeKind::repeatStatementNode: return new RepeatStatementNode(span);
        case NodeKind::whileStatementNode: return new WhileStatementNode(span);
        case NodeKind::forStatementNode: return new ForStatementNode(span);
        case NodeKind::integerNode: return new IntegerNode(span);
        case NodeKind::realNode: return new RealNode(span);
        case NodeKind::charNode: return new CharNode(span);
        case NodeKind::booleanNode: return new BooleanNode(span);
        case NodeKind::stringNode: return new StringNode(span);
        case NodeKind::pointerNode: return new PointerNode(span);
        case NodeKind::blockNode: return new BlockNode(span);
        case NodeKind::typeDeclarationNode: return new TypeDeclarationNode(span);
        case NodeKind::subrangeTypeNode: return new SubrangeTypeNode(span);
        case NodeKind::enumeratedTypeNode: return new EnumeratedTypeNode(span);
        case NodeKind::objectTypeNode: return new ObjectTypeNode(span);
        case NodeKind::fieldListNode: return new FieldListNode(span);
        case NodeKind::arrayTypeNode: return new ArrayTypeNode(span);
        case NodeKind::specializationNode: return new SpecializationNode(span);
        case NodeKind::procedureHeadingNode: return new ProcedureHeadingNode(span);
        case NodeKind::functionHeadingNode: return new FunctionHeadingNode(span);
        case NodeKind::constructorHeadingNode: return new ConstructorHeadingNode(span);
        case NodeKind::procedureDeclarationNode: return new ProcedureDeclarationNode(span);
        case NodeKind::functionDeclarationNode: return new FunctionDeclarationNode(span);
        case NodeKind::constructorDeclarationNode: return new ConstructorDeclarationNode(span);
        case NodeKind::constructorCallNode: return new ConstructorCallNode(span);
        case NodeKind::forwardNode: return new ForwardNode(span);
        case NodeKind::externalNode: return new ExternalNode(span);
        case NodeKind::subroutineIdNode: return new SubroutineIdNode(span);
        case NodeKind::parameterListNode: return new ParameterListNode(span);
        case NodeKind::variableDeclarationNode: return new VariableDeclarationNode(span);
    }
    throw std::runtime_error("error: create node: unknown node kind '" + std::to_string(static_cast<uint8_t>(kind)));
}

} // namespace p
