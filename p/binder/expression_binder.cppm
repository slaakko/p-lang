// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.binder.expression_binder;

import p.binder.bound_expression;
import p.core;
import p.ast;
import std.core;

export namespace p {

class ExpressionBinder : public Visitor
{
public:
    ExpressionBinder(Context* context_, SubroutineSymbol* currentSubroutine_, const std::vector<TypeSymbol*>& argumentTypes_);
    BoundExpressionNode* GetBoundExpression();
    void Visit(IntegerNode& node) override;
    void Visit(RealNode& node) override;
    void Visit(CharNode& node) override;
    void Visit(BooleanNode& node) override;
    void Visit(StringNode& node) override;
    void Visit(PointerNode& node) override;
    void Visit(BinaryExprNode& node) override;
    void Visit(UnaryExprNode& node) override;
    void Visit(ParenthesizedExprNode& node) override;
    void Visit(IntegerLiteralNode& node) override;
    void Visit(RealLiteralNode& node) override;
    void Visit(BooleanLiteralNode& node) override;
    void Visit(CharLiteralNode& node) override;
    void Visit(StringLiteralNode& node) override;
    void Visit(IdentifierNode& node) override;
    void Visit(ThisNode& node) override;
    void Visit(BaseNode& node) override;
    void Visit(NilNode& node) override;
    void Visit(NewExprNode& node) override;
    void Visit(NewArrayExprNode& node) override;
    void Visit(InvokeExprNode& node) override;
    void Visit(IndexExprNode& node) override;
    void Visit(DotNode& node) override;
private:
    Context* context;
    SubroutineSymbol* currentSubroutine; 
    std::vector<TypeSymbol*> argumentTypes;
    std::unique_ptr<BoundExpressionNode> boundExpression;
};

BoundExpressionNode* BindExpression(Node* node, SubroutineSymbol* currentSubroutine, const std::vector<TypeSymbol*>& argumentTypes, Context* context);

} // namespace p
