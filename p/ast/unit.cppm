// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.ast.unit;

import p.ast.node;
import p.ast.block;
import p.ast.common;
import std.core;

export namespace p {

enum class UnitPartKind
{
    none, program, interfacePart, implementationPart, initializationPart
};

std::string UnitPartKindStr(UnitPartKind kind);

class UnitPartNode : public Node
{
public:
    UnitPartNode(const soul::ast::Span& span_);
    UnitPartNode(const soul::ast::Span& span_, UnitPartKind partKind_);
    void SetBlock(p::BlockNode* block_);
    void SetUsesList(UnitNameListNode* uses_);
    UnitPartKind PartKind() const { return partKind; }
    UnitNameListNode* Uses() const { return uses.get(); }
    BlockNode* Block() const { return block.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
private:
    UnitPartKind partKind;
    std::unique_ptr<UnitNameListNode> uses;
    std::unique_ptr<BlockNode> block;
};

class UnitNode : public Node
{
public:
    UnitNode(const soul::ast::Span& span_);
    UnitNode(const soul::ast::Span& span_, const std::string& filePath_, UnitNameNode* unitName_);
    const std::string& FilePath() const override { return filePath; }
    UnitNameNode* UnitName() const { return unitName.get(); }
    void SetInterfacePart(UnitPartNode* interfacePart_);
    UnitPartNode* InterfacePart() const { return interfacePart.get(); }
    void SetImplementationPart(UnitPartNode* implementationPart_);
    UnitPartNode* ImplementationPart() const { return implementationPart.get(); }
    void SetInitializationPart(UnitPartNode* initializationPart_);
    UnitPartNode* InitializationPart() const { return initializationPart.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
private:
    std::string filePath;
    std::unique_ptr<UnitNameNode> unitName;
    std::unique_ptr<UnitPartNode> interfacePart;
    std::unique_ptr<UnitPartNode> implementationPart;
    std::unique_ptr<UnitPartNode> initializationPart;
};

} // namespace p
