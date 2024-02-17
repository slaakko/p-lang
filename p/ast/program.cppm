// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.ast.program;

import p.ast.node;
import p.ast.block;
import p.ast.common;
import std.core;

export namespace p {

class ProgramNode : public Node
{
public:
    ProgramNode(const soul::ast::Span& span_);
    ProgramNode(const soul::ast::Span& span_, const std::string& filePath_, UnitNameNode* unitName_);
    const std::string& FilePath() const override { return filePath; }
    UnitNameNode* UnitName() const { return unitName.get(); }
    void SetProgramParameters(IdentifierListNode* programParameters_);
    IdentifierListNode* ProgramParameters() const { return programParameters.get(); }
    void SetUsesList(UnitNameListNode* uses_);
    UnitNameListNode* Uses() const { return uses.get(); }
    void SetBlock(BlockNode* block_);
    BlockNode* Block() const { return block.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
private:
    std::string filePath;
    std::unique_ptr<UnitNameNode> unitName;
    std::unique_ptr<IdentifierListNode> programParameters;
    std::unique_ptr<UnitNameListNode> uses;
    std::unique_ptr<BlockNode> block;
};

} // namespace p
