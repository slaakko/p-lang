// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.ast.program;

import p.ast.visitor;

namespace p {

ProgramNode::ProgramNode(const soul::ast::Span& span_) : Node(NodeKind::programNode, span_)
{
}

ProgramNode::ProgramNode(const soul::ast::Span& span_, const std::string& filePath_, UnitNameNode* unitName_) : 
    Node(NodeKind::programNode, span_), filePath(filePath_), unitName(unitName_)
{
    unitName->SetParent(this);
}

void ProgramNode::SetProgramParameters(IdentifierListNode* programParameters_)
{
    programParameters.reset(programParameters_);
    if (programParameters)
    {
        programParameters->SetParent(this);
    }
}

void ProgramNode::SetUsesList(UnitNameListNode* uses_)
{
    uses.reset(uses_);
    if (uses)
    {
        uses->SetParent(this);
    }
}

void ProgramNode::SetBlock(BlockNode* block_)
{
    block.reset(block_);
    block->SetParent(this);
}

Node* ProgramNode::Clone() const
{
    ProgramNode* clone = new ProgramNode(Span(), filePath, static_cast<UnitNameNode*>(unitName->Clone()));
    if (programParameters)
    {
        clone->SetProgramParameters(static_cast<IdentifierListNode*>(programParameters->Clone()));
    }
    if (uses)
    {
        clone->SetUsesList(static_cast<UnitNameListNode*>(uses->Clone()));
    }
    clone->SetBlock(static_cast<BlockNode*>(block->Clone()));
    return clone;
}

void ProgramNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ProgramNode::Write(AstWriter& writer)
{
    writer.GetBinaryWriter().Write(filePath);
    writer.WriteNode(unitName.get());
    writer.WriteNode(programParameters.get());
    writer.WriteNode(uses.get());
    writer.WriteNode(block.get());
}

void ProgramNode::Read(AstReader& reader)
{
    filePath = reader.GetBinaryReader().ReadUtf8String();
    unitName.reset(static_cast<UnitNameNode*>(reader.ReadNode()));
    unitName->SetParent(this);
    SetProgramParameters(static_cast<IdentifierListNode*>(reader.ReadNode()));
    SetUsesList(static_cast<UnitNameListNode*>(reader.ReadNode()));
    SetBlock(static_cast<BlockNode*>(reader.ReadNode()));
}

} // namespace p
