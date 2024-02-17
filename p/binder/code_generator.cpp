// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.binder.code_generator;

import p.binder.bound_expression;
import p.binder.bound_statement;
import p.ast;

namespace p {

CodeGenerator::CodeGenerator(Context* context_, SubroutineSymbol* currentSubroutine_, const std::string& filePath_) :
    context(context_), currentSubroutine(currentSubroutine_), filePath(filePath_), emitter(filePath, context), level(0)
{
    emitter.SetCurrentSubroutine(currentSubroutine);
}

void CodeGenerator::Visit(BoundExpressionStatementNode& node)
{
    node.Expression()->Load(&emitter);
    if (node.Expression()->IsBoundSubroutineCallNode())
    {
        BoundSubroutineCallNode* subroutineCall = static_cast<BoundSubroutineCallNode*>(node.Expression());
        if (subroutineCall->Subroutine()->IsFunctionSymbol())
        {
            emitter.Emit(new PopValueInstruction());
        }
    }
    else if (node.Pop())
    {
        emitter.Emit(new PopValueInstruction());
    }
}

void CodeGenerator::Visit(BoundEmptyStatementNode& node)
{
    emitter.Emit(new NoOperationInstruction());
}

void CodeGenerator::Visit(BoundAssignmentStatementNode& node)
{
    node.Source()->Load(&emitter);
    node.Target()->Store(&emitter);
}

void CodeGenerator::Visit(BoundCompoundStatementNode& node)
{
    bool basicBlockPushed = false;
    BasicBlock* b = emitter.CurrentBasicBlock();
    if (!b)
    {
        b = emitter.GetCurrentSubroutine()->AddBasicBlock();
        emitter.PushBasicBlock(b);
        basicBlockPushed = true;
    }
    if (level == 0)
    {
        ReceiveInstruction* receiveInstruction = new ReceiveInstruction();
        receiveInstruction->SetCount(currentSubroutine->ParameterCount());
        emitter.Emit(receiveInstruction);
    }
    ++level;
    for (const auto& statement : node.Statements())
    {
        statement->Accept(*this);
    }
    --level;
    if (emitter.CurrentBasicBlock()->IsEmpty())
    {
        emitter.Emit(new NoOperationInstruction());
    }
    if (basicBlockPushed)
    {
        emitter.PopBasicBlock();
    }
}

void CodeGenerator::Visit(BoundIfStatementNode& node)
{
    JumpInstruction* jumpInstruction = new JumpInstruction();
    emitter.Emit(jumpInstruction);
    BasicBlock* p = emitter.GetCurrentSubroutine()->AddBasicBlock();
    emitter.PushBasicBlock(p);
    node.Condition()->Load(&emitter);
    jumpInstruction->SetTarget(p->First());
    BranchInstruction* branchInstruction = new BranchInstruction();
    emitter.Emit(branchInstruction);
    emitter.PopBasicBlock();
    BasicBlock* t = emitter.GetCurrentSubroutine()->AddBasicBlock();
    emitter.PushBasicBlock(t);
    node.ThenStatement()->Accept(*this);
    branchInstruction->SetTrueNext(t->First());
    JumpInstruction* jumpThenNext = new JumpInstruction();
    emitter.Emit(jumpThenNext);
    emitter.PopBasicBlock();
    JumpInstruction* jumpElseNext = nullptr;
    if (node.ElseStatement())
    {
        BasicBlock* e = emitter.GetCurrentSubroutine()->AddBasicBlock();
        emitter.PushBasicBlock(e);
        node.ElseStatement()->Accept(*this);
        branchInstruction->SetFalseNext(e->First());
        jumpElseNext = new JumpInstruction();
        emitter.Emit(jumpElseNext);
        emitter.PopBasicBlock();
    }
    BasicBlock* r = emitter.GetCurrentSubroutine()->AddBasicBlock();
    emitter.PushBasicBlock(r);
    emitter.IncrementOpenBasicBlocks();
    emitter.EnqueuePatch(new JumpNextPatch(r, jumpThenNext));
    if (jumpElseNext)
    {
        emitter.EnqueuePatch(new JumpNextPatch(r, jumpElseNext));
    }
    else
    {
        emitter.EnqueuePatch(new BranchFalsePatch(r, branchInstruction));
    }
}

void CodeGenerator::Visit(BoundReturnFunctionResultStatementNode& node)
{
    node.FunctionResult()->Load(&emitter);
}

void CodeGenerator::Visit(BoundCaseStatementNode& node)
{
    std::vector<JumpInstruction*> jumps;
    node.Condition()->Load(&emitter);
    CaseInstruction* caseInstruction = new CaseInstruction();
    emitter.Emit(caseInstruction);
    for (const auto& caseNode : node.Cases())
    {
        RangeList& rangeList = caseInstruction->AddRangeList();
        for (const auto& caseRange : caseNode->Ranges())
        {
            int32_t first = caseRange->RangeStart()->GetValue()->ToInteger();
            int32_t last = caseRange->RangeEnd()->GetValue()->ToInteger();
            Range range(first, last);
            rangeList.AddRange(range);
            BasicBlock* r = emitter.GetCurrentSubroutine()->AddBasicBlock();
            emitter.PushBasicBlock(r);
            caseNode->Statement()->Accept(*this);
            JumpInstruction* jumpInstruction = new JumpInstruction();
            emitter.Emit(jumpInstruction);
            jumps.push_back(jumpInstruction);
            emitter.PopBasicBlock();
            rangeList.SetNext(r->First());
        }
    }
    BasicBlock* e = emitter.GetCurrentSubroutine()->AddBasicBlock();
    emitter.PushBasicBlock(e);
    node.ElsePart()->Accept(*this);
    JumpInstruction* jumpInstruction = new JumpInstruction();
    emitter.Emit(jumpInstruction);
    jumps.push_back(jumpInstruction);
    emitter.PopBasicBlock();
    caseInstruction->SetElseInst(e->First());
    BasicBlock* r = emitter.GetCurrentSubroutine()->AddBasicBlock();
    emitter.PushBasicBlock(r);
    for (auto& jump : jumps)
    {
        emitter.EnqueuePatch(new JumpNextPatch(r, jump));
    }
}

void CodeGenerator::Visit(BoundRepeatStatementNode& node)
{
    JumpInstruction* jumpInstruction = new JumpInstruction();
    emitter.Emit(jumpInstruction);
    BasicBlock* p = emitter.GetCurrentSubroutine()->AddBasicBlock();
    emitter.PushBasicBlock(p);
    for (const auto& statement : node.Statements())
    {
        statement->Accept(*this);
    }
    node.Condition()->Load(&emitter);
    BranchInstruction* branchInstruction = new BranchInstruction();
    emitter.Emit(branchInstruction);
    branchInstruction->SetFalseNext(p->First());
    jumpInstruction->SetTarget(p->First());
    emitter.PopBasicBlock();
    BasicBlock* r = emitter.GetCurrentSubroutine()->AddBasicBlock();
    emitter.PushBasicBlock(r);
    emitter.IncrementOpenBasicBlocks();
    emitter.EnqueuePatch(new BranchTruePatch(r, branchInstruction));
}

void CodeGenerator::Visit(BoundWhileStatementNode& node)
{
    JumpInstruction* jumpInstruction = new JumpInstruction();
    emitter.Emit(jumpInstruction);
    BasicBlock* p = emitter.GetCurrentSubroutine()->AddBasicBlock();
    emitter.PushBasicBlock(p);
    node.Condition()->Load(&emitter);
    jumpInstruction->SetTarget(p->First());
    BranchInstruction* branchInstruction = new BranchInstruction();
    emitter.Emit(branchInstruction);
    emitter.PopBasicBlock();
    BasicBlock* q = emitter.GetCurrentSubroutine()->AddBasicBlock();
    emitter.PushBasicBlock(q);
    node.Statement()->Accept(*this);
    branchInstruction->SetTrueNext(q->First());
    JumpInstruction* jumpBackInstruction = new JumpInstruction();
    emitter.Emit(jumpBackInstruction);
    jumpBackInstruction->SetTarget(p->First());
    emitter.PopBasicBlock();
    BasicBlock* r = emitter.GetCurrentSubroutine()->AddBasicBlock();
    emitter.PushBasicBlock(r);
    emitter.IncrementOpenBasicBlocks();
    emitter.EnqueuePatch(new BranchFalsePatch(r, branchInstruction));
}

void CodeGenerator::Visit(BoundBinaryExpressionNode& node)
{
    node.Load(&emitter);
}

void CodeGenerator::Visit(BoundUnaryExpressionNode& node)
{
    node.Load(&emitter);
}

void CodeGenerator::Visit(BoundParameterNode& node)
{
    node.Load(&emitter);
}

void CodeGenerator::Visit(BoundLiteralNode& node)
{
    node.Load(&emitter);
}

void CodeGenerator::Visit(BoundConstantNode& node)
{
    node.Load(&emitter);
}

void CodeGenerator::Visit(BoundVariableNode& node)
{
    node.Load(&emitter);
}

void CodeGenerator::Visit(BoundProcedureNode& node)
{
    ThrowError("error: cannot generate code for bound procedure node", emitter.FilePath(), node.Span());
}

void CodeGenerator::Visit(BoundFunctionNode& node)
{
    ThrowError("error: cannot generate code for bound function node", emitter.FilePath(), node.Span());
}

void CodeGenerator::Visit(BoundMethodNode& node)
{
    ThrowError("error: cannot generate code for bound method node", emitter.FilePath(), node.Span());
}

void CodeGenerator::Visit(BoundSubroutineCallNode& node)
{
    node.Load(&emitter);
}

void CodeGenerator::Visit(BoundConstructorCallNode& node)
{
    node.Load(&emitter);
}

void CodeGenerator::Visit(BoundIndexExprNode& node)
{
    node.Load(&emitter);
}

void CodeGenerator::Visit(BoundMemberExprNode& node)
{
    node.Load(&emitter);
}

void CodeGenerator::Visit(BoundNewExpressionNode& node)
{
    node.Load(&emitter);
}

void CodeGenerator::Visit(BoundNewArrayExpressionNode& node)
{
    node.Load(&emitter);
}

void CodeGenerator::Visit(BoundArrayLengthNode& node)
{
    node.Load(&emitter);
}

void CodeGenerator::Visit(BoundStringLengthNode& node)
{
    node.Load(&emitter);
}

void CodeGenerator::Visit(BoundTypeNode& node)
{
    ThrowError("error: cannot generate code for bound type node", emitter.FilePath(), node.Span());
}

void CodeGenerator::Visit(BoundConversionNode& node)
{
    node.Load(&emitter);
}

void CodeGenerator::Visit(BoundValueConversionNode& node)
{
    node.Load(&emitter);
}

void CodeGenerator::Visit(BoundFunctionResultNode& node)
{
    node.Load(&emitter);
}

} // namespace p
