// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.ast.subroutine;

import p.ast.node;
import p.ast.common;
import soul.ast.span;
import std.core;

export namespace p {

enum class Virtual
{
    none, virtual_, override_
};

std::string VirtualStr(Virtual virtual_);

enum class ParameterQualifier
{
    valueParam, varParam, constParam
};

std::string ParameterQualifierStr(ParameterQualifier qualifier);

class SubroutineIdNode : public Node
{
public:
    SubroutineIdNode(const soul::ast::Span& span_);
    SubroutineIdNode(const soul::ast::Span& span_, IdentifierNode* objectId_, IdentifierNode* subroutineId_);
    IdentifierNode* ObjectId() const { return objectId.get(); }
    IdentifierNode* SubroutineId() const { return subroutineId.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
private:
    std::unique_ptr<IdentifierNode> objectId;
    std::unique_ptr<IdentifierNode> subroutineId;
};

class ParameterListNode : public Node
{
public:
    ParameterListNode(const soul::ast::Span& span_);
    ParameterListNode(const soul::ast::Span& span_, ParameterQualifier qualifier_, IdentifierListNode* identifierList_, Node* type_);
    ParameterQualifier Qualifier() const { return qualifier; }
    IdentifierListNode* IdentifierList() const { return identifierList.get(); }
    Node* Type() const { return type.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
private:
    ParameterQualifier qualifier;
    std::unique_ptr<IdentifierListNode> identifierList;
    std::unique_ptr<Node> type;
};

class SubroutineHeadingNode : public Node
{
public:
    SubroutineHeadingNode(NodeKind kind_, const soul::ast::Span& span_);
    SubroutineHeadingNode(NodeKind kind_, const soul::ast::Span& span_, SubroutineIdNode* subroutineId_);
    SubroutineIdNode* SubroutineId() const { return subroutineId.get(); }
    void SetVirtual(Virtual virtual__);
    Virtual GetVirtual() const { return virtual_; }
    void AddParameters(const soul::ast::Span& span, ParameterQualifier qualifier, IdentifierListNode* identifierList, Node* type);
    const std::vector<std::unique_ptr<ParameterListNode>>& ParameterLists() const { return parameterLists; }
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
private:
    Virtual virtual_;
    std::unique_ptr<SubroutineIdNode> subroutineId;
    std::vector<std::unique_ptr<ParameterListNode>> parameterLists;
};

class ProcedureHeadingNode : public SubroutineHeadingNode
{
public:
    ProcedureHeadingNode(const soul::ast::Span& span_);
    ProcedureHeadingNode(const soul::ast::Span& span_, SubroutineIdNode* subroutineId_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class FunctionHeadingNode : public SubroutineHeadingNode
{
public:
    FunctionHeadingNode(const soul::ast::Span& span_);
    FunctionHeadingNode(const soul::ast::Span& span_, SubroutineIdNode* subroutineId_);
    Node* ResultType() const { return resultType.get(); }
    void SetResultType(Node* resultType_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
private:
    std::unique_ptr<Node> resultType;
};

class ConstructorHeadingNode : public SubroutineHeadingNode
{
public:
    ConstructorHeadingNode(const soul::ast::Span& span_);
    IdentifierNode* ObjectId() const { return objectId.get(); }
    void SetObjectId(IdentifierNode* objectId_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
private:
    std::unique_ptr<IdentifierNode> objectId;
};

class ProcedureDeclarationNode : public Node
{
public:
    ProcedureDeclarationNode(const soul::ast::Span& span_);
    ProcedureDeclarationNode(const soul::ast::Span& span_, ProcedureHeadingNode* heading_, Node* subroutineBlock_);
    ProcedureHeadingNode* Heading() const { return heading.get(); }
    Node* SubroutineBlock() const { return subroutineBlock.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
private:
    std::unique_ptr<ProcedureHeadingNode> heading;
    std::unique_ptr<Node> subroutineBlock;
};

class FunctionDeclarationNode : public Node
{
public:
    FunctionDeclarationNode(const soul::ast::Span& span_);
    FunctionDeclarationNode(const soul::ast::Span& span_, FunctionHeadingNode* heading_, Node* subroutineBlock_);
    FunctionHeadingNode* Heading() const { return heading.get(); }
    Node* SubroutineBlock() const { return subroutineBlock.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
private:
    std::unique_ptr<FunctionHeadingNode> heading;
    std::unique_ptr<Node> subroutineBlock;
};

class ConstructorCallNode : public Node
{
public:
    ConstructorCallNode(const soul::ast::Span& span_);
    ConstructorCallNode(const soul::ast::Span& span_, Node* kindNode_);
    Node* KindNode() const { return kindNode.get(); }
    const std::vector<std::unique_ptr<Node>>& Arguments() const { return arguments; }
    void AddArgument(Node* argument);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
private:
    std::unique_ptr<Node> kindNode;
    std::vector<std::unique_ptr<Node>> arguments;
};

class ConstructorDeclarationNode : public Node
{
public:
    ConstructorDeclarationNode(const soul::ast::Span& span_);
    ConstructorDeclarationNode(const soul::ast::Span& span_, ConstructorHeadingNode* heading_, ConstructorCallNode* constructorCall_, Node* subroutineBlock_);
    ConstructorHeadingNode* Heading() const { return heading.get(); }
    Node* ConstructorCall() const { return constructorCall.get(); }
    Node* SubroutineBlock() const { return subroutineBlock.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
private:
    std::unique_ptr<ConstructorHeadingNode> heading;
    std::unique_ptr<ConstructorCallNode> constructorCall;
    std::unique_ptr<Node> subroutineBlock;
};

} // namespace p
