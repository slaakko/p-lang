// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.binder.type_binder;

import p.core;
import p.ast;
import std.core;

export namespace p {

class BlockSymbol;
    
class TypeBinder : public Visitor
{
public:
    TypeBinder(Context* context_);
    SymbolTable* GetSymbolTable() const { return symbolTable.get(); }
    void Visit(ProgramNode& node) override;
    void Visit(UnitNode& node) override;
    void Visit(UnitNameNode& node) override;
    void Visit(UnitNameListNode& node) override;
    void Visit(IntegerNode& node) override;
    void Visit(RealNode& node) override;
    void Visit(CharNode& node) override;
    void Visit(BooleanNode& node) override;
    void Visit(StringNode& node) override;
    void Visit(PointerNode& node) override;
    void Visit(IdentifierNode& node) override;
    void Visit(IdentifierListNode& node) override;
    void Visit(UnitPartNode& node) override;
    void Visit(BinaryExprNode& node) override;
    void Visit(UnaryExprNode& node) override;
    void Visit(ParenthesizedExprNode& node) override;
    void Visit(IntegerLiteralNode& node) override;
    void Visit(RealLiteralNode& node) override;
    void Visit(BooleanLiteralNode& node) override;
    void Visit(StringLiteralNode& node) override;
    void Visit(CharLiteralNode& node) override;
    void Visit(ThisNode& node) override;
    void Visit(BaseNode& node) override;
    void Visit(NilNode& node) override;
    void Visit(NewExprNode& node) override;
    void Visit(NewArrayExprNode& node) override;
    void Visit(InvokeExprNode& node) override;
    void Visit(IndexExprNode& node) override;
    void Visit(DotNode& node) override;
    void Visit(AssignmentStatementNode& node) override;
    void Visit(ExpressionStatementNode& node) override;
    void Visit(EmptyStatementNode& node) override;
    void Visit(CompoundStatementNode& node) override;
    void Visit(IfStatementNode& node) override;
    void Visit(CaseNode& node) override;
    void Visit(ConstantRangeNode& node) override;
    void Visit(CaseStatementNode& node) override;
    void Visit(RepeatStatementNode& node) override;
    void Visit(WhileStatementNode& node) override;
    void Visit(ForStatementNode& node) override;
    void Visit(BlockNode& node) override;
    void Visit(ConstantDeclarationNode& node) override;
    void Visit(TypeDeclarationNode& node) override;
    void Visit(VariableDeclarationNode& node) override;
    void Visit(SubrangeTypeNode& node) override;
    void Visit(EnumeratedTypeNode& node) override;
    void Visit(FieldListNode& node) override;
    void Visit(SubroutineIdNode& node) override;
    void Visit(ParameterListNode& node) override;
    void Visit(ProcedureHeadingNode& node) override;
    void Visit(FunctionHeadingNode& node) override;
    void Visit(ConstructorHeadingNode& node) override;
    void Visit(ProcedureDeclarationNode& node) override;
    void Visit(FunctionDeclarationNode& node) override;
    void Visit(ConstructorDeclarationNode& node) override;
    void Visit(ForwardNode& node) override;
    void Visit(ExternalNode& node) override;
    void Visit(ObjectTypeNode& node) override;
    void Visit(ArrayTypeNode& node) override;
    void Visit(SpecializationNode& node) override;
private:
    std::unique_ptr<SymbolTable> symbolTable;
    std::string unitId;
    Context* context;
    BlockSymbol* currentBlock;
    ContainerSymbol* currentContainer;
    FunctionSymbol* currentFunction;
    ProcedureSymbol* currentProcedure;
    ConstructorSymbol* currentConstructor;
    SubroutineSymbol* currentSubroutine;
    IdentifierNode* typeId;
    bool makeType;
    bool external;
    bool forward;
};

} // namespace p
