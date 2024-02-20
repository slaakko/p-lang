// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.ast.subroutine;

import p.ast.visitor;

namespace p {

std::string VirtualStr(Virtual virtual_)
{
    switch (virtual_)
    {
        case Virtual::none: return "none";
        case Virtual::virtual_: return "virtual";
        case Virtual::override_: return "override";
    }
    return "unknown";
}

std::string ParameterQualifierStr(ParameterQualifier qualifier)
{
    switch (qualifier)
    {
        case ParameterQualifier::valueParam: return "value";
        case ParameterQualifier::varParam: return "var";
        case ParameterQualifier::constParam: return "const";
    }
    return std::string();
}

SubroutineIdNode::SubroutineIdNode(const soul::ast::Span& span_) : Node(NodeKind::subroutineIdNode, span_)
{
}

SubroutineIdNode::SubroutineIdNode(const soul::ast::Span& span_, IdentifierNode* objectId_, IdentifierNode* subroutineId_) : 
    Node(NodeKind::subroutineIdNode, span_), objectId(objectId_), subroutineId(subroutineId_)
{
    if (objectId)
    {
        objectId->SetParent(this);
    }
    subroutineId->SetParent(this);
}

Node* SubroutineIdNode::Clone() const
{
    IdentifierNode* clonedObjectId = nullptr;
    if (objectId)
    {
        clonedObjectId = static_cast<IdentifierNode*>(objectId->Clone());
    }
    return new SubroutineIdNode(Span(), clonedObjectId, static_cast<IdentifierNode*>(subroutineId->Clone()));
}

void SubroutineIdNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void SubroutineIdNode::Write(AstWriter& writer)
{
    writer.WriteNode(objectId.get());
    writer.WriteNode(subroutineId.get());
}

void SubroutineIdNode::Read(AstReader& reader)
{
    objectId.reset(static_cast<IdentifierNode*>(reader.ReadNode()));
    if (objectId)
    {
        objectId->SetParent(this);
    }
    subroutineId.reset(static_cast<IdentifierNode*>(reader.ReadNode()));
    subroutineId->SetParent(this);
}

ParameterListNode::ParameterListNode(const soul::ast::Span& span_) : Node(NodeKind::parameterListNode, span_)
{
}

ParameterListNode::ParameterListNode(const soul::ast::Span& span_, ParameterQualifier qualifier_, IdentifierListNode* identifierList_, Node* type_) :
    Node(NodeKind::parameterListNode, span_), qualifier(qualifier_), identifierList(identifierList_), type(type_)
{
    identifierList->SetParent(this);
    type->SetParent(this);
}

Node* ParameterListNode::Clone() const
{
    return new ParameterListNode(Span(), qualifier, static_cast<IdentifierListNode*>(identifierList->Clone()), type->Clone());
}

void ParameterListNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ParameterListNode::Write(AstWriter& writer)
{
    writer.GetBinaryWriter().Write(static_cast<uint8_t>(qualifier));
    writer.WriteNode(identifierList.get());
    writer.WriteNode(type.get());
}

void ParameterListNode::Read(AstReader& reader)
{
    qualifier = static_cast<ParameterQualifier>(reader.GetBinaryReader().ReadByte());
    identifierList.reset(static_cast<IdentifierListNode*>(reader.ReadNode()));
    identifierList->SetParent(this);
    type.reset(reader.ReadNode());
    type->SetParent(this);
}

SubroutineHeadingNode::SubroutineHeadingNode(NodeKind kind_, const soul::ast::Span& span_) : Node(kind_, span_)
{
}

SubroutineHeadingNode::SubroutineHeadingNode(NodeKind kind_, const soul::ast::Span& span_, SubroutineIdNode* subroutineId_) : Node(kind_, span_), subroutineId(subroutineId_)
{
    subroutineId->SetParent(this);
}

void SubroutineHeadingNode::SetVirtual(Virtual virtual__)
{
    virtual_ = virtual__;
}

void SubroutineHeadingNode::AddParameters(const soul::ast::Span& span, ParameterQualifier qualifier, IdentifierListNode* identifierList, Node* type)
{
    std::unique_ptr<ParameterListNode> parameterList(new ParameterListNode(span, qualifier, identifierList, type));
    parameterList->SetParent(this);
    parameterLists.push_back(std::unique_ptr<ParameterListNode>(parameterList.release()));
}

void SubroutineHeadingNode::Write(AstWriter& writer)
{
    writer.WriteNode(subroutineId.get());
    writer.GetBinaryWriter().Write(static_cast<uint8_t>(virtual_));
    int32_t n = parameterLists.size();
    writer.GetBinaryWriter().Write(n);
    for (const auto& parameterList : parameterLists)
    {
        writer.WriteNode(parameterList.get());
    }
}

void SubroutineHeadingNode::Read(AstReader& reader)
{
    subroutineId.reset(static_cast<SubroutineIdNode*>(reader.ReadNode()));
    if (subroutineId)
    {
        subroutineId->SetParent(this);
    }
    virtual_ = static_cast<Virtual>(reader.GetBinaryReader().ReadByte());
    int32_t n = reader.GetBinaryReader().ReadInt();
    for (int32_t i = 0; i < n; ++i)
    {
        ParameterListNode* parameterList = static_cast<ParameterListNode*>(reader.ReadNode());
        parameterList->SetParent(this);
        parameterLists.push_back(std::unique_ptr<ParameterListNode>(parameterList));
    }
}

ProcedureHeadingNode::ProcedureHeadingNode(const soul::ast::Span& span_) : SubroutineHeadingNode(NodeKind::procedureHeadingNode, span_)
{
}

ProcedureHeadingNode::ProcedureHeadingNode(const soul::ast::Span& span_, SubroutineIdNode* subroutineId_) : 
    SubroutineHeadingNode(NodeKind::procedureHeadingNode, span_, subroutineId_)
{
}

Node* ProcedureHeadingNode::Clone() const
{
    ProcedureHeadingNode* clone = new ProcedureHeadingNode(Span(), static_cast<SubroutineIdNode*>(SubroutineId()->Clone()));
    clone->SetVirtual(GetVirtual());
    for (const auto& parameterList : ParameterLists())
    {
        clone->AddParameters(parameterList->Span(), parameterList->Qualifier(), static_cast<IdentifierListNode*>(parameterList->IdentifierList()->Clone()),
            parameterList->Type()->Clone());
    }
    return clone;
}

void ProcedureHeadingNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

FunctionHeadingNode::FunctionHeadingNode(const soul::ast::Span& span_) : SubroutineHeadingNode(NodeKind::functionHeadingNode, span_)
{
}

FunctionHeadingNode::FunctionHeadingNode(const soul::ast::Span& span_, SubroutineIdNode* subroutineId_) : SubroutineHeadingNode(NodeKind::functionHeadingNode, span_, subroutineId_)
{
}

void FunctionHeadingNode::SetResultType(Node* resultType_)
{
    resultType.reset(resultType_);
    resultType->SetParent(this);
}

Node* FunctionHeadingNode::Clone() const
{
    FunctionHeadingNode* clone = new FunctionHeadingNode(Span(), static_cast<SubroutineIdNode*>(SubroutineId()->Clone()));
    clone->SetVirtual(GetVirtual());
    for (const auto& parameterList : ParameterLists())
    {
        clone->AddParameters(parameterList->Span(), parameterList->Qualifier(), static_cast<IdentifierListNode*>(parameterList->IdentifierList()->Clone()),
            parameterList->Type()->Clone());
    }
    clone->SetResultType(resultType->Clone());
    return clone;
}

void FunctionHeadingNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void FunctionHeadingNode::Write(AstWriter& writer)
{
    SubroutineHeadingNode::Write(writer);
    writer.WriteNode(resultType.get());
}

void FunctionHeadingNode::Read(AstReader& reader)
{
    SubroutineHeadingNode::Read(reader);
    resultType.reset(reader.ReadNode());
    resultType->SetParent(this);
}

ConstructorHeadingNode::ConstructorHeadingNode(const soul::ast::Span& span_) : SubroutineHeadingNode(NodeKind::constructorHeadingNode, span_)
{
}

void ConstructorHeadingNode::SetObjectId(IdentifierNode* objectId_)
{
    objectId.reset(objectId_);
    objectId->SetParent(this);
}

Node* ConstructorHeadingNode::Clone() const
{
    ConstructorHeadingNode* clone = new ConstructorHeadingNode(Span());
    for (const auto& parameterList : ParameterLists())
    {
        clone->AddParameters(parameterList->Span(), parameterList->Qualifier(), static_cast<IdentifierListNode*>(parameterList->IdentifierList()->Clone()),
            parameterList->Type()->Clone());
    }
    if (objectId)
    {
        clone->SetObjectId(static_cast<IdentifierNode*>(objectId->Clone()));
    }
    return clone;
}

void ConstructorHeadingNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ConstructorHeadingNode::Write(AstWriter& writer)
{
    SubroutineHeadingNode::Write(writer);
    writer.WriteNode(objectId.get());
}

void ConstructorHeadingNode::Read(AstReader& reader)
{
    SubroutineHeadingNode::Read(reader);
    objectId.reset(static_cast<IdentifierNode*>(reader.ReadNode()));
    if (objectId)
    {
        objectId->SetParent(this);
    }
}

ProcedureDeclarationNode::ProcedureDeclarationNode(const soul::ast::Span& span_) : Node(NodeKind::procedureDeclarationNode, span_)
{
}

ProcedureDeclarationNode::ProcedureDeclarationNode(const soul::ast::Span& span_, ProcedureHeadingNode* heading_, Node* subroutineBlock_) : 
    Node(NodeKind::procedureDeclarationNode, span_), heading(heading_), subroutineBlock(subroutineBlock_)
{
    heading->SetParent(this);
    subroutineBlock->SetParent(this);
}

void ProcedureDeclarationNode::SetFilePath(const std::string& filePath_)
{
    filePath = filePath_;
}

const std::string& ProcedureDeclarationNode::FilePath() const
{
    if (!filePath.empty())
    {
        return filePath;
    }
    else
    {
        return Node::FilePath();
    }
}

Node* ProcedureDeclarationNode::Clone() const
{
    ProcedureDeclarationNode* clone = new ProcedureDeclarationNode(Span(), static_cast<ProcedureHeadingNode*>(heading->Clone()), subroutineBlock->Clone());
    clone->SetFilePath(filePath);
    return clone;
}

void ProcedureDeclarationNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ProcedureDeclarationNode::Write(AstWriter& writer)
{
    writer.WriteNode(heading.get());
    writer.WriteNode(subroutineBlock.get());
    writer.GetBinaryWriter().Write(filePath);
}

void ProcedureDeclarationNode::Read(AstReader& reader)
{
    heading.reset(static_cast<ProcedureHeadingNode*>(reader.ReadNode()));
    heading->SetParent(this);
    subroutineBlock.reset(reader.ReadNode());
    subroutineBlock->SetParent(this);
    filePath = reader.GetBinaryReader().ReadUtf8String();
}

FunctionDeclarationNode::FunctionDeclarationNode(const soul::ast::Span& span_) : Node(NodeKind::functionDeclarationNode, span_)
{
}

FunctionDeclarationNode::FunctionDeclarationNode(const soul::ast::Span& span_, FunctionHeadingNode* heading_, Node* subroutineBlock_) : 
    Node(NodeKind::functionDeclarationNode, span_), heading(heading_), subroutineBlock(subroutineBlock_)
{
    heading->SetParent(this);
    subroutineBlock->SetParent(this);
}

void FunctionDeclarationNode::SetFilePath(const std::string& filePath_)
{
    filePath = filePath_;
}

const std::string& FunctionDeclarationNode::FilePath() const
{
    if (!filePath.empty())
    {
        return filePath;
    }
    else
    {
        return Node::FilePath();
    }
}

Node* FunctionDeclarationNode::Clone() const
{
    FunctionDeclarationNode* clone = new FunctionDeclarationNode(Span(), static_cast<FunctionHeadingNode*>(heading->Clone()), subroutineBlock->Clone());
    clone->SetFilePath(filePath);
    return clone;
}

void FunctionDeclarationNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void FunctionDeclarationNode::Write(AstWriter& writer)
{
    writer.WriteNode(heading.get());
    writer.WriteNode(subroutineBlock.get());
    writer.GetBinaryWriter().Write(filePath);
}

void FunctionDeclarationNode::Read(AstReader& reader)
{
    heading.reset(static_cast<FunctionHeadingNode*>(reader.ReadNode()));
    heading->SetParent(this);
    subroutineBlock.reset(reader.ReadNode());
    subroutineBlock->SetParent(this);
    filePath = reader.GetBinaryReader().ReadUtf8String();
}

ConstructorCallNode::ConstructorCallNode(const soul::ast::Span& span_) : Node(NodeKind::constructorCallNode, span_)
{
}

ConstructorCallNode::ConstructorCallNode(const soul::ast::Span& span_, Node* kindNode_) : Node(NodeKind::constructorCallNode, span_), kindNode(kindNode_)
{
    kindNode->SetParent(this);
}

void ConstructorCallNode::AddArgument(Node* argument)
{
    argument->SetParent(this);
    arguments.push_back(std::unique_ptr<Node>(argument));
}

Node* ConstructorCallNode::Clone() const
{
    ConstructorCallNode* clone = new ConstructorCallNode(Span(), kindNode->Clone());
    for (const auto& argument : arguments)
    {
        clone->AddArgument(argument->Clone());
    }
    return clone;
}

void ConstructorCallNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ConstructorCallNode::Write(AstWriter& writer)
{
    writer.WriteNode(kindNode.get());
    int32_t n = arguments.size();
    writer.GetBinaryWriter().Write(n);
    for (const auto& argument : arguments)
    {
        writer.WriteNode(argument.get());
    }
}

void ConstructorCallNode::Read(AstReader& reader)
{
    kindNode.reset(reader.ReadNode());
    kindNode->SetParent(this);
    int32_t n = reader.GetBinaryReader().ReadInt();
    for (int32_t i = 0; i < n; ++i)
    {
        Node* argument = reader.ReadNode();
        AddArgument(argument);
    }
}

ConstructorDeclarationNode::ConstructorDeclarationNode(const soul::ast::Span& span_) : Node(NodeKind::constructorDeclarationNode, span_)
{
}

ConstructorDeclarationNode::ConstructorDeclarationNode(const soul::ast::Span& span_, ConstructorHeadingNode* heading_, ConstructorCallNode* constructorCall_, Node* subroutineBlock_) :
    Node(NodeKind::constructorDeclarationNode, span_), heading(heading_), constructorCall(constructorCall_), subroutineBlock(subroutineBlock_)
{
    heading->SetParent(this);
    if (constructorCall)
    {
        constructorCall->SetParent(this);
    }
    subroutineBlock->SetParent(this);
}

Node* ConstructorDeclarationNode::Clone() const
{
    ConstructorCallNode* clonedConstructorCall = nullptr;
    if (constructorCall)
    {
        clonedConstructorCall = static_cast<ConstructorCallNode*>(constructorCall->Clone());
    }
    ConstructorDeclarationNode* clone = new ConstructorDeclarationNode(Span(), static_cast<ConstructorHeadingNode*>(heading->Clone()), clonedConstructorCall, 
        subroutineBlock->Clone());
    clone->SetFilePath(filePath);
    return clone;
}

void ConstructorDeclarationNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ConstructorDeclarationNode::Write(AstWriter& writer)
{
    writer.WriteNode(heading.get());
    writer.WriteNode(constructorCall.get());
    writer.WriteNode(subroutineBlock.get());
    writer.GetBinaryWriter().Write(filePath);
}

void ConstructorDeclarationNode::Read(AstReader& reader)
{
    heading.reset(static_cast<ConstructorHeadingNode*>(reader.ReadNode()));
    heading->SetParent(this);
    constructorCall.reset(static_cast<ConstructorCallNode*>(reader.ReadNode()));
    if (constructorCall)
    {
        constructorCall->SetParent(this);
    }
    subroutineBlock.reset(reader.ReadNode());
    subroutineBlock->SetParent(this);
    filePath = reader.GetBinaryReader().ReadUtf8String();
}

void ConstructorDeclarationNode::SetFilePath(const std::string& filePath_)
{
    filePath = filePath_;
}

const std::string& ConstructorDeclarationNode::FilePath() const
{
    if (!filePath.empty())
    {
        return filePath;
    }
    else
    {
        return Node::FilePath();
    }
}

} // namespace p
