// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.binder.code_generator;

import p.binder.bound_node_visitor;
import p.core;
import std.core;

export namespace p {

class CodeGenerator : public BoundNodeVisitor
{
public:
    CodeGenerator(Context* context_, SubroutineSymbol* currentSubroutine_, const std::string& filePath_);
    void Visit(BoundExpressionStatementNode& node) override;
    void Visit(BoundEmptyStatementNode& node) override;
    void Visit(BoundAssignmentStatementNode& node) override;
    void Visit(BoundCompoundStatementNode& node) override;
    void Visit(BoundIfStatementNode& node) override;
    void Visit(BoundReturnFunctionResultStatementNode& node) override;
    void Visit(BoundCaseStatementNode& node) override;
    void Visit(BoundRepeatStatementNode& node) override;
    void Visit(BoundWhileStatementNode& node) override;
    void Visit(BoundBinaryExpressionNode& node) override;
    void Visit(BoundUnaryExpressionNode& node) override;
    void Visit(BoundParameterNode& node) override;
    void Visit(BoundLiteralNode& node) override;
    void Visit(BoundConstantNode& node) override;
    void Visit(BoundVariableNode& node) override;
    void Visit(BoundProcedureNode& node) override;
    void Visit(BoundFunctionNode& node) override;
    void Visit(BoundMethodNode& node) override;
    void Visit(BoundSubroutineCallNode& node) override;
    void Visit(BoundConstructorCallNode& node) override;
    void Visit(BoundIndexExprNode& node) override;
    void Visit(BoundMemberExprNode& node) override;
    void Visit(BoundNewExpressionNode& node) override;
    void Visit(BoundNewArrayExpressionNode& node) override;
    void Visit(BoundArrayLengthNode& node) override;
    void Visit(BoundStringLengthNode& node) override;
    void Visit(BoundTypeNode& node) override;
    void Visit(BoundConversionNode& node) override;
    void Visit(BoundValueConversionNode& node) override;
    void Visit(BoundFunctionResultNode& node) override;
private:
    Context* context;
    SubroutineSymbol* currentSubroutine;
    std::string filePath;
    Emitter emitter;
    int level;
};

} // namespace p
