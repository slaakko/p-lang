// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.binder.statement_binder;

import p.binder.bound_statement;
import p.core;
import p.ast;
import std.core;

export namespace p {

class StatementBinder : public Visitor
{
public:
    StatementBinder(Context* context_, SubroutineSymbol* currentSubroutine_);
    BoundStatementNode* GetBoundStatement();
    void Visit(ConstructorCallNode& node) override;
    void Visit(ThisNode& node) override;
    void Visit(BaseNode& node) override;
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
private:
    Context* context;
    SubroutineSymbol* currentSubroutine;
    ThisNode* thisNode;
    BaseNode* baseNode;
    std::unique_ptr<BoundExpressionStatementNode> boundConstructorCallStatement;
    std::unique_ptr<BoundStatementNode> boundStatement;
    std::unique_ptr<BoundRangeNode> boundRange;
    std::unique_ptr<BoundCaseNode> boundCaseNode;
    int level;
};

} // namespace p
