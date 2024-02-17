// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.ast.unit;

import p.ast.visitor;

namespace p {

std::string UnitPartKindStr(UnitPartKind kind)
{
    switch (kind)
    {
        case UnitPartKind::none: return "none";
        case UnitPartKind::program: return "program";
        case UnitPartKind::interfacePart: return "inteface";
        case UnitPartKind::implementationPart: return "implementation";
        case UnitPartKind::initializationPart: return "initialization";
    }
    return "unknown";
}

UnitPartNode::UnitPartNode(const soul::ast::Span& span_) : Node(NodeKind::unitPartNode, span_)
{
}

UnitPartNode::UnitPartNode(const soul::ast::Span& span_, UnitPartKind partKind_) : Node(NodeKind::unitPartNode, span_), partKind(partKind_)
{
}

void UnitPartNode::SetBlock(p::BlockNode* block_)
{
    block.reset(block_);
    block->SetParent(this);
}

void UnitPartNode::SetUsesList(UnitNameListNode* uses_)
{
    uses.reset(uses_);
    uses->SetParent(this);
}

Node* UnitPartNode::Clone() const
{
    UnitPartNode* clone = new UnitPartNode(Span(), partKind);
    clone->SetBlock(static_cast<BlockNode*>(block->Clone()));
    return clone;
}

void UnitPartNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void UnitPartNode::Write(AstWriter& writer)
{
    writer.GetBinaryWriter().Write(static_cast<uint8_t>(partKind));
    writer.WriteNode(uses.get());
    writer.WriteNode(block.get());
}

void UnitPartNode::Read(AstReader& reader)
{
    partKind = static_cast<UnitPartKind>(reader.GetBinaryReader().ReadByte());
    uses.reset(static_cast<UnitNameListNode*>(reader.ReadNode()));
    if (uses)
    {
        uses->SetParent(this);
    }
    block.reset(static_cast<BlockNode*>(reader.ReadNode()));
    block->SetParent(this);
}

UnitNode::UnitNode(const soul::ast::Span& span_) : Node(NodeKind::unitNode, span_)
{
}

UnitNode::UnitNode(const soul::ast::Span& span_, const std::string& filePath_, UnitNameNode* unitName_) :
    Node(NodeKind::unitNode, span_), filePath(filePath_), unitName(unitName_)
{
    unitName->SetParent(this);
}

void UnitNode::SetInterfacePart(UnitPartNode* interfacePart_)
{
    interfacePart.reset(interfacePart_);
    interfacePart->SetParent(this);
}

void UnitNode::SetImplementationPart(UnitPartNode* implementationPart_)
{
    implementationPart.reset(implementationPart_);
    implementationPart->SetParent(this);
}

void UnitNode::SetInitializationPart(UnitPartNode* initializationPart_)
{
    initializationPart.reset(initializationPart_);
    initializationPart->SetParent(this);
}

Node* UnitNode::Clone() const
{
    UnitNode* clone = new UnitNode(Span(), filePath, static_cast<UnitNameNode*>(unitName->Clone()));
    if (interfacePart)
    {
        clone->SetInterfacePart(static_cast<UnitPartNode*>(interfacePart->Clone()));
    }
    if (implementationPart)
    {
        clone->SetImplementationPart(static_cast<UnitPartNode*>(implementationPart->Clone()));
    }
    if (initializationPart)
    {
        clone->SetInitializationPart(static_cast<UnitPartNode*>(initializationPart->Clone()));
    }
    return clone;
}

void UnitNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void UnitNode::Write(AstWriter& writer)
{
    writer.GetBinaryWriter().Write(filePath);
    writer.WriteNode(unitName.get());
    writer.WriteNode(interfacePart.get());
    writer.WriteNode(implementationPart.get());
    writer.WriteNode(initializationPart.get());
}

void UnitNode::Read(AstReader& reader)
{
    filePath = reader.GetBinaryReader().ReadUtf8String();
    unitName.reset(static_cast<UnitNameNode*>(reader.ReadNode()));
    unitName->SetParent(this);
    interfacePart.reset(static_cast<UnitPartNode*>(reader.ReadNode()));
    if (interfacePart)
    {
        interfacePart->SetParent(this);
    }
    implementationPart.reset(static_cast<UnitPartNode*>(reader.ReadNode()));
    if (implementationPart)
    {
        implementationPart->SetParent(this);
    }
    initializationPart.reset(static_cast<UnitPartNode*>(reader.ReadNode()));
    if (initializationPart)
    {
        initializationPart->SetParent(this);
    }
}

} // namespace p
