// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.ast.node;

import soul.ast.span;
import std.core;

export namespace p {

export namespace ast {}

class Visitor;
class AstWriter;
class AstReader;

enum class NodeKind
{
    none, programNode, unitNode, unitPartNode, unitNameNode, unitNameListNode, identifierNode, identifierListNode, 
    constantDeclarationNode, arrayConstantNode, objectConstantNode, fieldNode,
    binaryExprNode, unaryExprNode, parenthesizedExprNode, integerLiteralNode, realLiteralNode, booleanLiteralNode, stringLiteralNode, charLiteralNode, 
    thisNode, baseNode, nilNode, newExprNode, newArrayExprNode, invokeExprNode, indexExprNode, dotNode,
    assignmentStatementNode, expressionStatementNode, emptyStatementNode, compoundStatementNode, ifStatementNode, caseNode, constantRangeNode, caseStatementNode,
    repeatStatementNode, whileStatementNode, forStatementNode,
    integerNode, realNode, charNode, booleanNode, stringNode, pointerNode,
    blockNode, typeDeclarationNode, subrangeTypeNode, enumeratedTypeNode, objectTypeNode, fieldListNode, arrayTypeNode, specializationNode,
    procedureHeadingNode, functionHeadingNode, constructorHeadingNode, procedureDeclarationNode, functionDeclarationNode, constructorDeclarationNode, constructorCallNode,
    forwardNode, externalNode, subroutineIdNode, parameterListNode, variableDeclarationNode
};

class Node
{
public:
    Node(NodeKind kind_, const soul::ast::Span& span_);
    virtual ~Node();
    NodeKind Kind() const { return kind; }
    void SetParent(Node* parent_) { parent = parent_; }
    Node* Parent() const { return parent; }
    const soul::ast::Span& Span() const { return span; }
    soul::ast::Span& Span() { return span; }
    virtual Node* Clone() const = 0;
    virtual void Accept(Visitor& visitor) = 0;
    virtual void Write(AstWriter& writer) = 0;
    virtual void Read(AstReader& reader) = 0;
    virtual const std::string& FilePath() const;
    virtual void AddNode(Node* node);
    bool IsProgramNode() const { return kind == NodeKind::programNode; }
    bool IsBaseNode() const { return kind == NodeKind::baseNode; }
    bool IsInvokeExprNode() const { return kind == NodeKind::invokeExprNode; }
    bool IsIdentifierNode() const { return kind == NodeKind::identifierNode; }
private:
    NodeKind kind;
    soul::ast::Span span;
    Node* parent;
};

Node* CreateNode(NodeKind kind, const soul::ast::Span& span);

} // namespace p
