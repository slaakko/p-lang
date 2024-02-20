// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.binder.expression_binder;

namespace p {

SymbolKind CommonType(SymbolKind left, SymbolKind right)
{
    if (left == right) return left;
    else if (left == SymbolKind::nilTypeSymbol || right == SymbolKind::nilTypeSymbol) return SymbolKind::nilTypeSymbol;
    else if (left == SymbolKind::integerTypeSymbol)
    {
        if (right == SymbolKind::realTypeSymbol)
        {
            return SymbolKind::realTypeSymbol;
        }
    }
    else if (left == SymbolKind::realTypeSymbol)
    {
        if (right == SymbolKind::integerTypeSymbol)
        {
            return SymbolKind::realTypeSymbol;
        }
    }
    else if (left == SymbolKind::charTypeSymbol)
    {
        if (right == SymbolKind::stringTypeSymbol)
        {
            return SymbolKind::stringTypeSymbol;
        }
    }
    else if (left == SymbolKind::stringTypeSymbol)
    {
        if (right == SymbolKind::charTypeSymbol)
        {
            return SymbolKind::stringTypeSymbol;
        }
    }
    return SymbolKind::none;
}

ExpressionBinder::ExpressionBinder(Context* context_, SubroutineSymbol* currentSubroutine_, const std::vector<TypeSymbol*>& argumentTypes_) : 
    context(context_), currentSubroutine(currentSubroutine_), argumentTypes(argumentTypes_)
{
}

BoundExpressionNode* ExpressionBinder::GetBoundExpression()
{
    return boundExpression.release();
}

void ExpressionBinder::Visit(IntegerNode& node)
{
    boundExpression.reset(new BoundTypeNode(node.Span(), GetType(&node, currentSubroutine->Block(), context)));
}

void ExpressionBinder::Visit(RealNode& node)
{
    boundExpression.reset(new BoundTypeNode(node.Span(), GetType(&node, currentSubroutine->Block(), context)));
}

void ExpressionBinder::Visit(CharNode& node)
{
    boundExpression.reset(new BoundTypeNode(node.Span(), GetType(&node, currentSubroutine->Block(), context)));
}

void ExpressionBinder::Visit(BooleanNode& node)
{
    boundExpression.reset(new BoundTypeNode(node.Span(), GetType(&node, currentSubroutine->Block(), context)));
}

void ExpressionBinder::Visit(StringNode& node) 
{
    boundExpression.reset(new BoundTypeNode(node.Span(), GetType(&node, currentSubroutine->Block(), context)));
}

void ExpressionBinder::Visit(PointerNode& node)
{
    boundExpression.reset(new BoundTypeNode(node.Span(), GetType(&node, currentSubroutine->Block(), context)));
}

void ExpressionBinder::Visit(BinaryExprNode& node)
{
    context->PushNode(&node);
    node.Left()->Accept(*this);
    std::unique_ptr<BoundExpressionNode> left(boundExpression.release());
    node.Right()->Accept(*this);
    std::unique_ptr<BoundExpressionNode> right(boundExpression.release());
    SymbolKind commonType = CommonType(left->Type()->Kind(), right->Type()->Kind());
    if (commonType != SymbolKind::none)
    {
        if (commonType == SymbolKind::nilTypeSymbol)
        {
            if (left->Type()->IsNilTypeSymbol())
            {
                TypeSymbol* type = context->GetSymbolTable()->GetFundamentalType(commonType, &node, context);
                FunctionSymbol* binaryOperatorFunction = GetBinaryOperatorFunction(node.Op(), type, &node);
                boundExpression.reset(new BoundBinaryExpressionNode(node.Span(), right.release(), left.release(), binaryOperatorFunction, &node));
            }
            else
            {
                TypeSymbol* type = context->GetSymbolTable()->GetFundamentalType(commonType, &node, context);
                FunctionSymbol* binaryOperatorFunction = GetBinaryOperatorFunction(node.Op(), type, &node);
                boundExpression.reset(new BoundBinaryExpressionNode(node.Span(), left.release(), right.release(), binaryOperatorFunction, &node));
            }
        }
        else if (commonType == SymbolKind::objectTypeSymbol || commonType == SymbolKind::specializationSymbol)
        {
            TypeSymbol* type = left->Type();
            FunctionSymbol* binaryOperatorFunction = GetBinaryOperatorFunction(node.Op(), type, &node);
            boundExpression.reset(new BoundBinaryExpressionNode(node.Span(), left.release(), right.release(), binaryOperatorFunction, &node));
        }
        else
        {
            if (left->Type()->Kind() != commonType)
            {
                TypeSymbol* targetType = context->GetSymbolTable()->GetFundamentalType(commonType, &node, context);
                TypeSymbol* leftType = left->Type();
                BoundConversionNode* conversion = new BoundConversionNode(node.Span(), GetConversionFunction(targetType, leftType, &node, true), left.release());
                left.reset(conversion);
            }
            if (right->Type()->Kind() != commonType)
            {
                TypeSymbol* targetType = context->GetSymbolTable()->GetFundamentalType(commonType, &node, context);
                TypeSymbol* rightType = right->Type();
                BoundConversionNode* conversion = new BoundConversionNode(node.Span(), GetConversionFunction(targetType, rightType, &node, true), right.release());
                right.reset(conversion);
            }
            TypeSymbol* type = context->GetSymbolTable()->GetFundamentalType(commonType, &node, context);
            FunctionSymbol* binaryOperatorFunction = GetBinaryOperatorFunction(node.Op(), type, &node);
            boundExpression.reset(new BoundBinaryExpressionNode(node.Span(), left.release(), right.release(), binaryOperatorFunction, &node));
        }
    }
    else
    {
        ThrowError("error: invalid binary operation operand type", node.FilePath(), node.Span());
    }
    context->PopNode();
}

void ExpressionBinder::Visit(UnaryExprNode& node)
{
    context->PushNode(&node);
    node.Operand()->Accept(*this);
    std::unique_ptr<BoundExpressionNode> operand(boundExpression.release());
    FunctionSymbol* unaryOperatorFunction = GetUnaryOperatorFunction(node.Op(), operand->Type(), &node);
    boundExpression.reset(new BoundUnaryExpressionNode(node.Span(), operand.release(), unaryOperatorFunction, &node));
    context->PopNode();
}

void ExpressionBinder::Visit(ParenthesizedExprNode& node)
{
    boundExpression.reset(BindExpression(node.Expr(), currentSubroutine, argumentTypes, context));
}

void ExpressionBinder::Visit(IntegerLiteralNode& node)
{
    TypeSymbol* integerType = context->GetSymbolTable()->GetType(GetIntegerTypeId(), &node, context);
    IntegerValue* integerValue = new IntegerValue(node.Value());
    integerValue->SetType(integerType);
    boundExpression.reset(new BoundLiteralNode(node.Span(), integerValue, integerType));
}

void ExpressionBinder::Visit(RealLiteralNode& node)
{
    TypeSymbol* realType = context->GetSymbolTable()->GetType(GetRealTypeId(), &node, context);
    RealValue* realValue = new RealValue(node.Value());
    realValue->SetType(realType);
    boundExpression.reset(new BoundLiteralNode(node.Span(), realValue, realType));
}

void ExpressionBinder::Visit(BooleanLiteralNode& node)
{
    TypeSymbol* booleanType = context->GetSymbolTable()->GetType(GetBooleanTypeId(), &node, context);
    BooleanValue* booleanValue = new BooleanValue(node.Value());
    booleanValue->SetType(booleanType);
    boundExpression.reset(new BoundLiteralNode(node.Span(), booleanValue, booleanType));
}

void ExpressionBinder::Visit(CharLiteralNode& node)
{
    TypeSymbol* charType = context->GetSymbolTable()->GetType(GetCharTypeId(), &node, context);
    CharValue* charValue = new CharValue(node.Value());
    charValue->SetType(charType);
    boundExpression.reset(new BoundLiteralNode(node.Span(), charValue, charType));
}

void ExpressionBinder::Visit(StringLiteralNode& node)
{
    TypeSymbol* stringType = context->GetSymbolTable()->GetType(GetStringTypeId(), &node, context);
    StringValue* stringValue = new StringValue(node.Value());
    stringValue->SetType(stringType);
    boundExpression.reset(new BoundLiteralNode(node.Span(), stringValue, stringType));
}

void ExpressionBinder::Visit(IdentifierNode& node)
{
    Symbol* symbol = currentSubroutine->Block()->GetSymbol(node.Str(), &node, true, true);
    if (symbol->IsTypeSymbol())
    {
        boundExpression.reset(new BoundTypeNode(node.Span(), static_cast<TypeSymbol*>(symbol)));
    }
    else
    {
        switch (symbol->Kind())
        {
            case SymbolKind::constantSymbol:
            {
                ConstantSymbol* constant = static_cast<ConstantSymbol*>(symbol);
                boundExpression.reset(new BoundConstantNode(node.Span(), constant));
                break;
            }
            case SymbolKind::variableSymbol:
            {
                VariableSymbol* variable = static_cast<VariableSymbol*>(symbol);
                boundExpression.reset(new BoundVariableNode(node.Span(), variable));
                break;
            }
            case SymbolKind::procedureSymbol:
            {
                ProcedureSymbol* procedure = static_cast<ProcedureSymbol*>(symbol);
                boundExpression.reset(new BoundProcedureNode(node.Span(), procedure));
                break;
            }
            case SymbolKind::functionSymbol:
            {
                FunctionSymbol* function = static_cast<FunctionSymbol*>(symbol);
                boundExpression.reset(new BoundFunctionNode(node.Span(), function, argumentTypes, &node));
                break;
            }
            case SymbolKind::fieldSymbol:
            {
                FieldSymbol* field = static_cast<FieldSymbol*>(symbol);
                boundExpression.reset(new BoundMemberExprNode(node.Span(), new BoundParameterNode(node.Span(), currentSubroutine->ThisParam()), field->Index(), field->Type()));
                break;
            }
            case SymbolKind::parameterSymbol:
            {
                ParameterSymbol* parameter = static_cast<ParameterSymbol*>(symbol);
                boundExpression.reset(new BoundParameterNode(node.Span(), parameter));
                break;
            }
            default:
            {
                if (symbol->IsProcedureSymbol())
                {
                    ProcedureSymbol* procedure = static_cast<ProcedureSymbol*>(symbol);
                    boundExpression.reset(new BoundProcedureNode(node.Span(), procedure));
                }
                else if (symbol->IsFunctionSymbol())
                {
                    FunctionSymbol* function = static_cast<FunctionSymbol*>(symbol);
                    boundExpression.reset(new BoundFunctionNode(node.Span(), function, argumentTypes, &node));
                }
                else
                {
                    ThrowError("symbol '" + node.Str() + "' unexpected at this context", node.FilePath(), node.Span());
                }
                break;
            }
        }
    }
}

void ExpressionBinder::Visit(ThisNode& node)
{
    ObjectTypeSymbol* objectType = currentSubroutine->GetObjectType();
    if (objectType)
    {
        boundExpression.reset(new BoundParameterNode(node.Span(), currentSubroutine->ThisParam()));
        boundExpression->SetFlag(ArgumentFlags::thisOrBaseArgument);
    }
    else
    {
        ThrowError("error: 'this' requires object type context", node.FilePath(), node.Span());
    }
}

void ExpressionBinder::Visit(BaseNode& node)
{
    ObjectTypeSymbol* objectType = currentSubroutine->GetObjectType();
    if (objectType)
    {
        ObjectTypeSymbol* baseType = objectType->BaseType();
        if (!baseType)
        {
            ThrowError("error: 'base' requires base object type context; " + objectType->Name() + "' has no base type", node.FilePath(), node.Span());
        }
        boundExpression.reset(new BoundParameterNode(node.Span(), currentSubroutine->ThisParam(), baseType));
        boundExpression->SetFlag(ArgumentFlags::thisOrBaseArgument);
    }
    else
    {
        ThrowError("error: 'base' requires object type context", node.FilePath(), node.Span());
    }
}

void ExpressionBinder::Visit(NilNode& node)
{
    boundExpression.reset(new BoundLiteralNode(node.Span(), new NilValue(), context->GetSymbolTable()->GetType(GetNilTypeId(), &node, context)));
}

void ExpressionBinder::Visit(NewExprNode& node)
{
    Node* parent = context->GetNode();
    context->PushNode(&node);
    bool invokeParent = false;
    if (parent && parent->IsInvokeExprNode())
    {
        invokeParent = true;
    }
    BlockSymbol* block = currentSubroutine->Block();
    TypeSymbol* type = GetType(node.Type(), block, context);
    if (type->IsObjectTypeOrSpecializationSymbol())
    {
        ObjectTypeSymbol* objectType = static_cast<ObjectTypeSymbol*>(type);
        if (invokeParent)
        {
            boundExpression.reset(new BoundNewExpressionNode(node.Span(), objectType));
        }
        else
        {
            InvokeExprNode invokeExprNode(node.Span(), node.Clone());
            invokeExprNode.Accept(*this);
        }
    }
    else
    {
        ThrowError("error: object type expected", node.FilePath(), node.Span());
    }
}

void ExpressionBinder::Visit(NewArrayExprNode& node)
{
    context->PushNode(&node);
    BlockSymbol* block = currentSubroutine->Block();
    TypeSymbol* elementType = GetType(node.ElementType(), block, context);
    node.ArraySize()->Accept(*this);
    std::unique_ptr<BoundExpressionNode> arraySize = std::move(boundExpression);
    ArrayTypeSymbol* arrayType = block->GetArrayType(elementType->Name());
    if (!arrayType)
    {
        arrayType = context->GetSymbolTable()->GetArrayType(elementType->Id(), &node);
    }
    if (!arrayType)
    {
        ThrowError("error: array type for element type '" + elementType->Name() + "' not found", node.FilePath(), node.Span());
    }
    boundExpression.reset(new BoundNewArrayExpressionNode(node.Span(), elementType, arraySize.release(), arrayType));
    context->PopNode();
}

void ExpressionBinder::Visit(InvokeExprNode& node)
{
    context->PushNode(&node);
    std::vector<std::unique_ptr<BoundExpressionNode>> boundArguments;
    for (const auto& argument : node.Arguments())
    {
        argument->Accept(*this);
        boundArguments.push_back(std::unique_ptr<BoundExpressionNode>(boundExpression.release()));
    }
    std::vector<TypeSymbol*> argumentTypes;
    for (const auto& boundArgument : boundArguments)
    {
        argumentTypes.push_back(boundArgument->Type());
    }
    std::unique_ptr<BoundExpressionNode> subjectNode(BindExpression(node.Subject(), currentSubroutine, argumentTypes, context));
    if (subjectNode->IsBoundTypeNode())
    {
        TypeSymbol* targetType = subjectNode->Type();
        if (boundArguments.size() == 1)
        {
            BoundExpressionNode* boundArgument = boundArguments[0].get();
            FunctionSymbol* conversionFunction = GetConversionFunction(targetType, boundArgument->Type(), &node, false);
            if (conversionFunction)
            {
                BoundConversionNode* boundConversion = new BoundConversionNode(node.Span(), conversionFunction, boundArguments[0].release());
                boundExpression.reset(boundConversion);
            }
            else
            {
                BoundValueConversionNode* boundValueConversion = new BoundValueConversionNode(node.Span(), boundArguments[0].release(), targetType);
                boundExpression.reset(boundValueConversion);
            }
        }
        else
        {
            ThrowError("single source argument expected", node.FilePath(), node.Span());
        }
    }
    else
    {
        SubroutineSymbol* subroutine = nullptr;
        TypeSymbol* type = nullptr;
        if (subjectNode->IsBoundProcedureNode())
        {
            BoundProcedureNode* procedureNode = static_cast<BoundProcedureNode*>(subjectNode.get());
            ProcedureSymbol* procedure = procedureNode->Procedure();
            if (boundArguments.size() == procedure->Parameters().size() - 1)
            {
                ObjectTypeSymbol* procedureObjectType = procedure->GetObjectType();
                ObjectTypeSymbol* currentObjectType = currentSubroutine->GetObjectType();
                if (procedureObjectType && currentObjectType && currentObjectType->IsSameOrHasBaseType(procedureObjectType))
                {
                    boundArguments.insert(boundArguments.begin(), std::unique_ptr<BoundExpressionNode>(
                        new BoundParameterNode(node.Span(), procedure->ThisParam(), procedureObjectType)));
                }
            }
            subroutine = procedure;
        }
        else if (subjectNode->IsBoundFunctionNode())
        {
            BoundFunctionNode* functionNode = static_cast<BoundFunctionNode*>(subjectNode.get());
            type = functionNode->Type();
            FunctionSymbol* function = functionNode->Function();
            if (boundArguments.size() == function->Parameters().size() - 1)
            {
                ObjectTypeSymbol* functionObjectType = function->GetObjectType();
                ObjectTypeSymbol* currentObjectType = currentSubroutine->GetObjectType();
                if (functionObjectType && currentObjectType && currentObjectType->IsSameOrHasBaseType(functionObjectType))
                {
                    boundArguments.insert(boundArguments.begin(), std::unique_ptr<BoundExpressionNode>(
                        new BoundParameterNode(node.Span(), function->ThisParam(), functionObjectType)));
                }
            }
            subroutine = function;
        }
        else if (subjectNode->IsBoundMethodNode())
        {
            BoundMethodNode* methodNode = static_cast<BoundMethodNode*>(subjectNode.get());
            boundArguments.insert(boundArguments.begin(), std::unique_ptr<BoundExpressionNode>(methodNode->ReleaseSubject()));
            subroutine = methodNode->Method();
            type = methodNode->Type();
        }
        else if (subjectNode->IsBoundArrayLengthNode() || subjectNode->IsBoundStringLengthNode())
        {
            if (boundArguments.empty())
            {
                boundExpression.reset(subjectNode.release());
            }
            else
            {
                ThrowError("error: array or string length take no arguments", node.FilePath(), node.Span());
            }
        }
        else if (subjectNode->IsBoundNewExpressionNode())
        {
            BoundNewExpressionNode* boundNewExpressionNode = static_cast<BoundNewExpressionNode*>(subjectNode.get());
            argumentTypes.insert(argumentTypes.begin(), subjectNode->Type());
            ObjectTypeSymbol* objectType = boundNewExpressionNode->ObjectType();
            boundArguments.insert(boundArguments.begin(), std::move(subjectNode));
            ConstructorSymbol* constructor = objectType->GetConstructor(argumentTypes);
            if (constructor)
            {
                subroutine = constructor;
            }
            else if (!boundArguments.empty())
            {
                ThrowError("error: matching constructor not found", node.FilePath(), node.Span());
            }
            else
            {
                boundExpression.reset(subjectNode.release());
            }
        }
        if (subroutine)
        {
            if (!subroutine->IsVariableParamSubroutine())
            {
                if (subroutine->ParameterCount() != boundArguments.size())
                {
                    ThrowError("error: subroutine '" + subroutine->FullName() + "' takes " + std::to_string(subroutine->ParameterCount()) + " parameters; " +
                        std::to_string(boundArguments.size()) + " arguments passed.", node.FilePath(), node.Span());
                }
                int index = 0;
                for (auto parameter : subroutine->Parameters())
                {
                    if (parameter->Qualifier() == ParameterQualifier::varParam && index < boundArguments.size() && boundArguments[index]->IsConst())
                    {
                        ThrowError("error: cannot pass 'const' argument to 'var' parameter", node.FilePath(), boundArguments[index]->Span());
                    }
                    if (index < boundArguments.size() && boundArguments[index]->Type() != parameter->Type())
                    {
                        if (parameter->Type()->IsObjectTypeOrSpecializationSymbol() && boundArguments[index]->Type()->IsObjectTypeOrSpecializationSymbol())
                        {
                            ObjectTypeSymbol* paramObjectType = static_cast<ObjectTypeSymbol*>(parameter->Type());
                            ObjectTypeSymbol* argObjectType = static_cast<ObjectTypeSymbol*>(boundArguments[index]->Type());
                            if (!argObjectType->IsSameOrHasBaseType(paramObjectType))
                            {
                                ThrowError("error: argument type mismatch", node.FilePath(), boundArguments[index]->Span());
                            }
                        }
                        else
                        {
                            FunctionSymbol* conversionFunction = GetConversionFunction(parameter->Type(), boundArguments[index]->Type(), &node, false);
                            if (!conversionFunction)
                            {
                                ThrowError("error: no conversion for argument " + std::to_string(index) + " found for subroutine call '" + subroutine->FullName() + "'", 
                                    node.FilePath(), boundArguments[index]->Span());
                            }
                            boundArguments[index].reset(new BoundConversionNode(node.Span(), conversionFunction, boundArguments[index]->Clone()));
                        }
                    }
                    ++index;
                }
            }
            if (subroutine->IsProcedureSymbol() || subroutine->IsFunctionSymbol())
            {
                BoundSubroutineCallNode* subroutineCall = new BoundSubroutineCallNode(node.Span(), subroutine, type);
                boundExpression.reset(subroutineCall);
                for (auto& argument : boundArguments)
                {
                    subroutineCall->AddArgument(argument.release());
                }
                ObjectTypeSymbol* objectType = subroutine->GetObjectType();
                if (objectType && !subroutineCall->Arguments().empty())
                {
                    BoundExpressionNode* subjectArg = subroutineCall->Arguments()[0].get();
                    if (!subjectArg->GetFlag(ArgumentFlags::thisOrBaseArgument))
                    {
                        TypeSymbol* type = subjectArg->Type();
                        if (type->IsObjectTypeOrSpecializationSymbol())
                        {
                            ObjectTypeSymbol* objectType = static_cast<ObjectTypeSymbol*>(type);
                            if (objectType->IsVirtual() && subroutine->GetVirtual() != Virtual::none)
                            {
                                subroutineCall->SetFlag(ArgumentFlags::virtualCall);
                            }
                        }
                    }
                }
            }
            else if (subroutine->IsConstructorSymbol())
            {
                BoundConstructorCallNode* constructorCall = new BoundConstructorCallNode(node.Span(), static_cast<ConstructorSymbol*>(subroutine));
                boundExpression.reset(constructorCall);
                for (auto& argument : boundArguments)
                {
                    constructorCall->AddArgument(argument.release());
                }
            }
        }
    }
    context->PopNode();
}

void ExpressionBinder::Visit(IndexExprNode& node)
{
    context->PushNode(&node);
    node.Subject()->Accept(*this);
    std::unique_ptr<BoundExpressionNode> subject = std::move(boundExpression);
    node.Index()->Accept(*this);
    std::unique_ptr<BoundExpressionNode> index = std::move(boundExpression);
    boundExpression.reset(new BoundIndexExprNode(node.Span(), subject.release(), index.release()));
    context->PopNode();
}

void ExpressionBinder::Visit(DotNode& node)
{
    context->PushNode(&node);
    node.Subject()->Accept(*this);
    if (boundExpression->Type()->IsObjectTypeOrSpecializationSymbol())
    {
        ObjectTypeSymbol* objectType = static_cast<ObjectTypeSymbol*>(boundExpression->Type());
        int32_t fieldIndex = objectType->GetFieldIndexNoThrow(node.Id()->Str());
        if (fieldIndex != -1)
        {
            FieldSymbol* field = objectType->GetField(fieldIndex);
            boundExpression.reset(new BoundMemberExprNode(node.Span(), boundExpression.release(), fieldIndex, field->Type()));
        }
        else
        {
            SubroutineSymbol* subroutine = objectType->GetSubroutineNoThrow(node.Id()->Str());
            if (node.Subject()->IsBaseNode())
            {
                while (!subroutine && objectType && objectType->BaseType())
                {
                    objectType = objectType->BaseType();
                    subroutine = objectType->GetSubroutineNoThrow(node.Id()->Str());
                    if (subroutine)
                    {
                        boundExpression.reset(new BoundParameterNode(node.Span(), subroutine->ThisParam(), objectType));
                        boundExpression->SetFlag(ArgumentFlags::thisOrBaseArgument);
                    }
                }
            }
            else
            {
                if (!subroutine)
                {
                    while (objectType)
                    {
                        subroutine = objectType->GetSubroutineNoThrow(node.Id()->Str());
                        if (subroutine) break;
                        objectType = objectType->BaseType();
                    }
                }
            }
            if (subroutine)
            {
                boundExpression.reset(new BoundMethodNode(node.Span(), boundExpression.release(), subroutine, argumentTypes, &node));
            }
            else
            {
                ThrowError("error: field or method '" + node.Id()->Str() + "' not found", node.FilePath(), node.Span());
            }
        }
    }
    else if (boundExpression->Type()->IsArrayTypeSymbol())
    {
        if (node.Id()->Str() == "Length")
        {
            TypeSymbol* integerType = context->GetSymbolTable()->GetType(GetIntegerTypeId(), &node, context);
            boundExpression.reset(new BoundArrayLengthNode(node.Span(), boundExpression.release(), integerType));
        }
        else
        {
            ThrowError("error: array.Length expected", node.FilePath(), node.Span());
        }
    }
    else if (boundExpression->Type()->IsStringTypeSymbol())
    {
        if (node.Id()->Str() == "Length")
        {
            TypeSymbol* integerType = context->GetSymbolTable()->GetType(GetIntegerTypeId(), &node, context);
            boundExpression.reset(new BoundStringLengthNode(node.Span(), boundExpression.release(), integerType));
        }
        else
        {
            ThrowError("error: string.Length expected", node.FilePath(), node.Span());
        }
    }
    else
    {
        ThrowError("error: object, array or string type subject expected", node.FilePath(), node.Span());
    }
    context->PopNode();
}

BoundExpressionNode* BindExpression(Node* node, SubroutineSymbol* currentSubroutine, const std::vector<TypeSymbol*>& argumentTypes, Context* context)
{
    ExpressionBinder binder(context, currentSubroutine, argumentTypes);
    node->Accept(binder);
    return binder.GetBoundExpression();
}

} // namespace p
