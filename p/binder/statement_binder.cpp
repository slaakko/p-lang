// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.binder.statement_binder;

import p.binder.evaluator;
import p.binder.bound_expression;
import p.binder.expression_binder;

namespace p {

StatementBinder::StatementBinder(Context* context_, SubroutineSymbol* currentSubroutine_) : context(context_), currentSubroutine(currentSubroutine_), level(0)
{
    if (context->CurrentConstructor())
    {
        Node* constructorCallNode = context->CurrentConstructor()->ConstructorCall();
        if (constructorCallNode)
        {
            constructorCallNode->Accept(*this);
        }
        else
        {
            ObjectTypeSymbol* objectType = context->CurrentConstructor()->GetObjectType();
            if (objectType)
            {
                if (objectType->BaseType())
                {
                    ConstructorSymbol* baseConstructor = objectType->BaseType()->GetDefaultConstructor();
                    if (!baseConstructor)
                    {
                        ThrowError("error: base type '" + objectType->BaseType()->Name() + "' has no default constructor", objectType->SourceFilePath(), objectType->Span());
                    }
                    BoundConstructorCallNode* boundConstructorCall = new BoundConstructorCallNode(objectType->Span(), baseConstructor);
                    boundConstructorCall->AddArgument(new BoundParameterNode(objectType->Span(), currentSubroutine->ThisParam(), objectType->BaseType()));
                    BoundExpressionStatementNode* boundExpressionStatementNode(new BoundExpressionStatementNode(objectType->Span(), boundConstructorCall));
                    boundExpressionStatementNode->DontPop();
                    boundConstructorCallStatement.reset(boundExpressionStatementNode);
                }
            }
        }
    }
}

BoundStatementNode* StatementBinder::GetBoundStatement()
{
    return boundStatement.release();
}

void StatementBinder::Visit(ConstructorCallNode& node)
{
    thisNode = nullptr;
    baseNode = nullptr;
    node.KindNode()->Accept(*this);
    std::vector<std::unique_ptr<BoundExpressionNode>> arguments;
    ObjectTypeSymbol* objectType = context->CurrentConstructor()->GetObjectType();
    if (!objectType)
    {
        ThrowError("error: object type expected", node.FilePath(), node.Span());
    }
    if (baseNode)
    {
        if (objectType->BaseType())
        {
            arguments.push_back(std::unique_ptr<BoundExpressionNode>(new BoundParameterNode(node.Span(), currentSubroutine->ThisParam(), objectType->BaseType())));
        }
        else
        {
            ThrowError("error: object type '" + objectType->Name() + "' has no base type", node.FilePath(), node.Span());
        }
    }
    else if (thisNode)
    {
        arguments.push_back(std::unique_ptr<BoundExpressionNode>(new BoundParameterNode(node.Span(), currentSubroutine->ThisParam())));
    }
    else
    {
        ThrowError("error: 'base' or 'this' expected", node.FilePath(), node.Span());
    }
    for (const auto& argument : node.Arguments())
    {
        BoundExpressionNode* boundArgument = BindExpression(argument.get(), currentSubroutine, std::vector<TypeSymbol*>(), context);
        arguments.push_back(std::unique_ptr<BoundExpressionNode>(boundArgument));
    }
    std::vector<TypeSymbol*> argumentTypes;
    for (const auto& argument : arguments)
    {
        argumentTypes.push_back(argument->Type());
    }
    ConstructorSymbol* constructorToCall = nullptr;
    if (baseNode)
    {
        constructorToCall = objectType->BaseType()->GetConstructor(argumentTypes);
        if (!constructorToCall)
        {
            ThrowError("error: base type '" + objectType->BaseType()->Name() + "' has no matching constructor", node.FilePath(), node.Span());
        }
    }
    else if (thisNode)
    {
        constructorToCall = objectType->GetConstructor(argumentTypes);
        if (!constructorToCall)
        {
            ThrowError("error: type '" + objectType->Name() + "' has no matching constructor", node.FilePath(), node.Span());
        }
    }
    else
    {
        ThrowError("error: 'base' or 'this' expected", node.FilePath(), node.Span());
    }
    BoundConstructorCallNode* boundConstructorCall = new BoundConstructorCallNode(node.Span(), constructorToCall);
    for (auto& arg : arguments)
    {
        boundConstructorCall->AddArgument(arg.release());
    }
    boundConstructorCallStatement.reset(new BoundExpressionStatementNode(node.Span(), boundConstructorCall));
    boundConstructorCallStatement->DontPop();
}

void StatementBinder::Visit(ThisNode& node)
{
    thisNode = &node;
}

void StatementBinder::Visit(BaseNode& node)
{
    baseNode = &node;
}

void StatementBinder::Visit(AssignmentStatementNode& node)
{
    std::vector<TypeSymbol*> argumentTypes;
    std::unique_ptr<BoundExpressionNode> target(BindExpression(node.Target(), currentSubroutine, argumentTypes, context));
    if (target->IsConst())
    {
        ThrowError("error: cannot assign to constant", node.FilePath(), node.Span());
    }
    if (target->IsBoundFunctionNode())
    {
        BoundFunctionNode* boundFunction = static_cast<BoundFunctionNode*>(target.get());
        FunctionSymbol* function = boundFunction->Function();
        if (function == currentSubroutine)
        {
            VariableSymbol* resultVar = function->GetVariable("@result", &node, context);
            if (resultVar)
            {
                target.reset(new BoundVariableNode(node.Span(), resultVar));
            }
            else
            {
                ThrowError("error: function result variable not found", node.FilePath(), node.Span());
            }
        }
        else
        {
            ThrowError("error: invalid function result assignment", node.FilePath(), node.Span());
        }
    }
    std::unique_ptr<BoundExpressionNode> source(BindExpression(node.Source(), currentSubroutine, argumentTypes, context));
    if (target->Type() != source->Type())
    {
        bool objectTypeMatch = false;
        if (source->Type()->IsNilTypeSymbol())
        {
            objectTypeMatch = true;
        }
        else if (target->Type()->IsObjectTypeOrSpecializationSymbol() && source->Type()->IsObjectTypeOrSpecializationSymbol())
        {
            ObjectTypeSymbol* targetObjectType = static_cast<ObjectTypeSymbol*>(target->Type());
            ObjectTypeSymbol* sourceObjectType = static_cast<ObjectTypeSymbol*>(source->Type());
            if (sourceObjectType->IsSameOrHasBaseType(targetObjectType))
            {
                objectTypeMatch = true;
            }
        }
        if (!objectTypeMatch)
        {
            FunctionSymbol* conversionFunction = GetConversionFunction(target->Type(), source->Type(), &node, false);
            if (!conversionFunction)
            {
                ThrowError("error: invalid assignment in subroutine '" + currentSubroutine->FullName() + "'", node.FilePath(), node.Span());
            }
            source.reset(new BoundConversionNode(node.Span(), conversionFunction, source.release()));
        }
    }
    boundStatement.reset(new BoundAssignmentStatementNode(node.Span(), target.release(), source.release()));
}

void StatementBinder::Visit(ExpressionStatementNode& node)
{
    std::vector<TypeSymbol*> argumentTypes;
    boundStatement.reset(new BoundExpressionStatementNode(node.Span(), BindExpression(node.Expr(), currentSubroutine, argumentTypes, context)));
}

void StatementBinder::Visit(EmptyStatementNode& node)
{
    boundStatement.reset(new BoundEmptyStatementNode(node.Span()));
}

void StatementBinder::Visit(CompoundStatementNode& node)
{
    std::unique_ptr<BoundCompoundStatementNode> boundCompoundStatement(new BoundCompoundStatementNode(node.Span()));
    if (level == 0)
    {
        boundCompoundStatement->SetFilePath(node.FilePath());
    }
    if (boundConstructorCallStatement)
    {
        boundCompoundStatement->AddStatement(boundConstructorCallStatement.release());
    }
    for (const auto& statement : node.Statements())
    {
        ++level;
        statement->Accept(*this);
        --level;
        boundCompoundStatement->AddStatement(boundStatement.release());
    }
    if (level == 0 && currentSubroutine->IsFunctionSymbol())
    {
        VariableSymbol* functionResultVar = currentSubroutine->Block()->GetVariable("@result", &node, context);
        if (functionResultVar)
        {
            BoundFunctionResultNode* boundFunctionResult = new BoundFunctionResultNode(node.Span(), functionResultVar);
            boundCompoundStatement->AddStatement(new BoundReturnFunctionResultStatementNode(node.Span(), boundFunctionResult));
        }
        else
        {
            ThrowError("error: function result not found", node.FilePath(), node.Span());
        }
    }
    boundStatement.reset(boundCompoundStatement.release());
}

void StatementBinder::Visit(IfStatementNode& node)
{
    std::vector<TypeSymbol*> argumentTypes;
    std::unique_ptr<BoundExpressionNode> condition(BindExpression(node.Condition(), currentSubroutine, argumentTypes, context));
    if (!condition->Type()->IsBooleanTypeSymbol())
    {
        ThrowError("error: Boolean condition expected", node.FilePath(), node.Span());
    }
    ++level;
    node.ThenStatement()->Accept(*this);
    --level;
    std::unique_ptr<BoundStatementNode> thenStatement = std::move(boundStatement);
    std::unique_ptr<BoundStatementNode> elseStatement;
    if (node.ElseStatement())
    {
        ++level;
        node.ElseStatement()->Accept(*this);
        --level;
        elseStatement = std::move(boundStatement);
    }
    boundStatement.reset(new BoundIfStatementNode(node.Span(), condition.release(), thenStatement.release(), elseStatement.release()));
}

void StatementBinder::Visit(CaseNode& node)
{
    boundCaseNode.reset(new BoundCaseNode(node.Span()));
    for (const auto& range : node.Ranges())
    {
        range->Accept(*this);
        boundCaseNode->AddRange(boundRange.release());
    }
    node.Statement()->Accept(*this);
    boundCaseNode->SetStatement(boundStatement.release());
}

void StatementBinder::Visit(ConstantRangeNode& node)
{
    Value* rangeStart = Evaluate(node.RangeStart(), context, currentSubroutine->Block());
    Value* rangeEnd = nullptr;
    if (node.RangeEnd())
    {
        rangeEnd = Evaluate(node.RangeEnd(), context, currentSubroutine->Block());
    }
    else
    {
        rangeEnd = static_cast<Value*>(rangeStart->Clone());
    }
    boundRange.reset(new BoundRangeNode(node.Span(), rangeStart, rangeEnd));
}

void StatementBinder::Visit(CaseStatementNode& node)
{
    std::unique_ptr<BoundCaseStatementNode> boundCaseStatement(new BoundCaseStatementNode(node.Span()));
    std::vector<TypeSymbol*> argumentTypes;
    std::unique_ptr<BoundExpressionNode> condition(BindExpression(node.Condition(), currentSubroutine, argumentTypes, context));
    boundCaseStatement->SetCondition(condition.release());
    for (const auto& caseNode : node.Cases())
    {
        caseNode->Accept(*this);
        boundCaseStatement->AddCase(boundCaseNode.release());
    }
    if (node.ElsePart())
    {
        ++level;
        node.ElsePart()->Accept(*this);
        --level;
        boundCaseStatement->SetElsePart(boundStatement.release());
    }
    else
    {
        boundCaseStatement->SetElsePart(new BoundEmptyStatementNode(node.Span()));
    }
    boundStatement.reset(boundCaseStatement.release());
}

void StatementBinder::Visit(RepeatStatementNode& node)
{
    BoundRepeatStatementNode* boundRepeatStatementNode = new BoundRepeatStatementNode(node.Span());
    for (const auto& statement : node.Statements())
    {
        ++level;
        statement->Accept(*this);
        --level;
        boundRepeatStatementNode->AddStatement(boundStatement.release());
    }
    std::vector<TypeSymbol*> argumentTypes;
    std::unique_ptr<BoundExpressionNode> condition(BindExpression(node.Condition(), currentSubroutine, argumentTypes, context));
    if (!condition->Type()->IsBooleanTypeSymbol())
    {
        ThrowError("error: Boolean condition expected", node.FilePath(), node.Span());
    }
    boundRepeatStatementNode->SetCondition(condition.release());
    boundStatement.reset(boundRepeatStatementNode);
}

void StatementBinder::Visit(WhileStatementNode& node)
{
    std::vector<TypeSymbol*> argumentTypes;
    std::unique_ptr<BoundExpressionNode> condition(BindExpression(node.Condition(), currentSubroutine, argumentTypes, context));
    if (condition->Type()->IsBooleanTypeSymbol())
    {
        ++level;
        node.Statement()->Accept(*this);
        --level;
        std::unique_ptr<BoundStatementNode> statement = std::move(boundStatement);
        boundStatement.reset(new BoundWhileStatementNode(node.Span(), condition.release(), statement.release()));
    }
    else
    {
        ThrowError("error: Boolean condition expected", node.FilePath(), node.Span());
    }
}

void StatementBinder::Visit(ForStatementNode& node)
{
    std::vector<TypeSymbol*> argumentTypes;
    if (node.GetDirection() == Direction::to)
    {
        std::unique_ptr<CompoundStatementNode> toCompound(new CompoundStatementNode(node.Span()));
        std::unique_ptr<BoundExpressionNode> initialValue(BindExpression(node.InitialValue(), currentSubroutine, argumentTypes, context));
        VariableSymbol* initialTemp = currentSubroutine->MakeTempVar(node.Span(), initialValue->Type());
        std::unique_ptr<IdentifierNode> initialTempId(new IdentifierNode(node.Span(), initialTemp->Name()));
        std::unique_ptr<BoundExpressionNode> finalValue(BindExpression(node.FinalValue(), currentSubroutine, argumentTypes, context));
        VariableSymbol* finalTemp = currentSubroutine->MakeTempVar(node.Span(), finalValue->Type());
        std::unique_ptr<IdentifierNode> finalTempId(new IdentifierNode(node.Span(), finalTemp->Name()));
        AssignmentStatementNode* assignInitialTemp = new AssignmentStatementNode(node.Span(), initialTempId->Clone(), node.InitialValue()->Clone());
        toCompound->AddStatement(assignInitialTemp);
        AssignmentStatementNode* assignFinalTemp = new AssignmentStatementNode(node.Span(), finalTempId->Clone(), node.FinalValue()->Clone());
        toCompound->AddStatement(assignFinalTemp);
        BinaryExprNode* cond = new BinaryExprNode(Operator::lessOrEqual, initialTempId->Clone(), finalTempId->Clone(), node.Span());
        CompoundStatementNode* thenStatement = new CompoundStatementNode(node.Span());
        thenStatement->AddStatement(new AssignmentStatementNode(node.Span(), node.ControlVariable()->Clone(), initialTempId->Clone()));
        thenStatement->AddStatement(static_cast<StatementNode*>(node.Statement()->Clone()));
        BinaryExprNode* whileCond = new BinaryExprNode(Operator::notEqual, node.ControlVariable()->Clone(), finalTempId->Clone(), node.Span());
        CompoundStatementNode* doStatement = new CompoundStatementNode(node.Span());
        InvokeExprNode* succExpr = new InvokeExprNode(node.Span(), new IdentifierNode(node.Span(), "Succ"));
        succExpr->AddNode(node.ControlVariable()->Clone());
        doStatement->AddStatement(new AssignmentStatementNode(node.Span(), node.ControlVariable()->Clone(), succExpr));
        doStatement->AddStatement(static_cast<StatementNode*>(node.Statement()->Clone()));
        WhileStatementNode* whileStatement = new WhileStatementNode(node.Span(), whileCond, doStatement);
        thenStatement->AddStatement(whileStatement);
        IfStatementNode* ifStatement = new IfStatementNode(node.Span(), cond, thenStatement);
        toCompound->AddStatement(ifStatement);
        toCompound->SetFilePath(node.FilePath());
        toCompound->Accept(*this);
    }
    else if (node.GetDirection() == Direction::downto)
    {
        std::unique_ptr<CompoundStatementNode> downtoCompound(new CompoundStatementNode(node.Span()));
        std::unique_ptr<BoundExpressionNode> initialValue(BindExpression(node.InitialValue(), currentSubroutine, argumentTypes, context));
        VariableSymbol* initialTemp = currentSubroutine->MakeTempVar(node.Span(), initialValue->Type());
        std::unique_ptr<IdentifierNode> initialTempId(new IdentifierNode(node.Span(), initialTemp->Name()));
        std::unique_ptr<BoundExpressionNode> finalValue(BindExpression(node.FinalValue(), currentSubroutine, argumentTypes, context));
        VariableSymbol* finalTemp = currentSubroutine->MakeTempVar(node.Span(), finalValue->Type());
        std::unique_ptr<IdentifierNode> finalTempId(new IdentifierNode(node.Span(), finalTemp->Name()));
        AssignmentStatementNode* assignInitialTemp = new AssignmentStatementNode(node.Span(), initialTempId->Clone(), node.InitialValue()->Clone());
        downtoCompound->AddStatement(assignInitialTemp);
        AssignmentStatementNode* assignFinalTemp = new AssignmentStatementNode(node.Span(), finalTempId->Clone(), node.FinalValue()->Clone());
        downtoCompound->AddStatement(assignFinalTemp);
        BinaryExprNode* cond = new BinaryExprNode(Operator::greaterOrEqual, initialTempId->Clone(), finalTempId->Clone(), node.Span());
        CompoundStatementNode* thenStatement = new CompoundStatementNode(node.Span());
        thenStatement->AddStatement(new AssignmentStatementNode(node.Span(), node.ControlVariable()->Clone(), initialTempId->Clone()));
        thenStatement->AddStatement(static_cast<StatementNode*>(node.Statement()->Clone()));
        BinaryExprNode* whileCond = new BinaryExprNode(Operator::notEqual, node.ControlVariable()->Clone(), finalTempId->Clone(), node.Span());
        CompoundStatementNode* doStatement = new CompoundStatementNode(node.Span());
        InvokeExprNode* predExpr = new InvokeExprNode(node.Span(), new IdentifierNode(node.Span(), "Pred"));
        predExpr->AddNode(node.ControlVariable()->Clone());
        doStatement->AddStatement(new AssignmentStatementNode(node.Span(), node.ControlVariable()->Clone(), predExpr));
        doStatement->AddStatement(static_cast<StatementNode*>(node.Statement()->Clone()));
        WhileStatementNode* whileStatement = new WhileStatementNode(node.Span(), whileCond, doStatement);
        thenStatement->AddStatement(whileStatement);
        IfStatementNode* ifStatement = new IfStatementNode(node.Span(), cond, thenStatement);
        downtoCompound->AddStatement(ifStatement);
        downtoCompound->SetFilePath(node.FilePath());
        downtoCompound->Accept(*this);
    }
}

} // namespace p
