// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.binder.bound_expression;

import p.binder.bound_node_visitor;

namespace p {

BoundExpressionNode::BoundExpressionNode(BoundNodeKind kind_, const soul::ast::Span& span_, TypeSymbol* type_) : BoundNode(kind_, span_), type(type_), flags(ArgumentFlags::none)
{
}

void BoundExpressionNode::Load(Emitter* emitter)
{
    ThrowError("cannot load this kind of bound expression", FilePath(), Span());
}

void BoundExpressionNode::Store(Emitter* emitter)
{
    ThrowError("cannot store to this kind of bound expression", FilePath(), Span());
}

TypeSymbol* BinaryExprFunctionResultType(FunctionSymbol* operatorFunction, BoundExpressionNode* left, BoundExpressionNode* right, Node* node)
{
    std::vector<TypeSymbol*> argumentTypes;
    argumentTypes.push_back(left->Type());
    argumentTypes.push_back(right->Type());
    return operatorFunction->ResultType(argumentTypes, node);
}

BoundBinaryExpressionNode::BoundBinaryExpressionNode(const soul::ast::Span& span_, BoundExpressionNode* left_, BoundExpressionNode* right_, FunctionSymbol* operatorFunction_, 
    Node* node_) : BoundExpressionNode(BoundNodeKind::boundBinaryExprNode, span_, BinaryExprFunctionResultType(operatorFunction_, left_, right_, node_)),
    left(left_), right(right_), operatorFunction(operatorFunction_), node(node_)
{
    left->SetParent(this);
    right->SetParent(this);
}

void BoundBinaryExpressionNode::Load(Emitter* emitter)
{
    left->Load(emitter);
    right->Load(emitter);
    operatorFunction->GenerateCode(emitter, Span());
}

BoundExpressionNode* BoundBinaryExpressionNode::Clone() const
{
    BoundBinaryExpressionNode* clone = new BoundBinaryExpressionNode(Span(), left->Clone(), right->Clone(), operatorFunction, node);
    clone->SetFlags(Flags());
    return clone;
}

bool BoundBinaryExpressionNode::IsConst() const
{
    return left->IsConst() && right->IsConst();
}

void BoundBinaryExpressionNode::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

TypeSymbol* UnaryExprFunctionResultType(FunctionSymbol* operatorFunction, BoundExpressionNode* operand, Node* node)
{
    std::vector<TypeSymbol*> argumentTypes;
    argumentTypes.push_back(operand->Type());
    return operatorFunction->ResultType(argumentTypes, node);
}

BoundUnaryExpressionNode::BoundUnaryExpressionNode(const soul::ast::Span& span_, BoundExpressionNode* operand_, FunctionSymbol* operatorFunction_, Node* node_) : 
    BoundExpressionNode(BoundNodeKind::boundUnaryExprNode, span_, UnaryExprFunctionResultType(operatorFunction_, operand_, node_)),
    operand(operand_), operatorFunction(operatorFunction_), node(node_)
{
    operand->SetParent(this);
}

void BoundUnaryExpressionNode::Load(Emitter* emitter)
{
    operand->Load(emitter);
    operatorFunction->GenerateCode(emitter, Span());
}

BoundExpressionNode* BoundUnaryExpressionNode::Clone() const
{
    BoundUnaryExpressionNode* clone = new BoundUnaryExpressionNode(Span(), operand->Clone(), operatorFunction, node);
    clone->SetFlags(Flags());
    return clone;
}

bool BoundUnaryExpressionNode::IsConst() const
{
    return operand->IsConst();
}

void BoundUnaryExpressionNode::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundParameterNode::BoundParameterNode(const soul::ast::Span& span_, ParameterSymbol* parameter_) :
    BoundExpressionNode(BoundNodeKind::boundParameterNode, span_, parameter_->Type()), parameter(parameter_)
{
}

BoundParameterNode::BoundParameterNode(const soul::ast::Span& span_, ParameterSymbol* parameter_, TypeSymbol* type_) :
    BoundExpressionNode(BoundNodeKind::boundParameterNode, span_, type_), parameter(parameter_)
{
}

void BoundParameterNode::Load(Emitter* emitter)
{
    LoadLocalInstruction* loadLocalInstruction = new LoadLocalInstruction();
    loadLocalInstruction->SetLocalIndex(parameter->Index());
    emitter->Emit(loadLocalInstruction);
}

BoundExpressionNode* BoundParameterNode::Clone() const
{
    BoundExpressionNode* clone = new BoundParameterNode(Span(), parameter, Type());
    clone->SetFlags(Flags());
    return clone;
}

bool BoundParameterNode::IsConst() const
{
    return parameter->IsConst();
}

void BoundParameterNode::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundLiteralNode::BoundLiteralNode(const soul::ast::Span& span_, Value* value_, TypeSymbol* type_) : 
    BoundExpressionNode(BoundNodeKind::boundLiteralNode, span_, type_), value(value_)
{
}

void BoundLiteralNode::Load(Emitter* emitter)
{
    PushValueInstruction* pushValueInstruction = new PushValueInstruction();
    pushValueInstruction->SetValue(value.release());
    emitter->Emit(pushValueInstruction);
}

BoundExpressionNode* BoundLiteralNode::Clone() const
{
    BoundLiteralNode* clone = new BoundLiteralNode(Span(), static_cast<Value*>(value->Clone()), Type());
    clone->SetFlags(Flags());
    return clone;
}

void BoundLiteralNode::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundConstantNode::BoundConstantNode(const soul::ast::Span& span_, ConstantSymbol* constant_) : 
    BoundExpressionNode(BoundNodeKind::boundConstantNode, span_, constant_->Type()), constant(constant_)
{
}

void BoundConstantNode::Load(Emitter* emitter)
{
    if (constant->GetValue()->IsObjectValue() || constant->GetValue()->IsArrayValue())
    {
        LoadConstantInstruction* loadConstantInstruction = new LoadConstantInstruction();
        loadConstantInstruction->SetConstantId(constant->Id());
        emitter->Emit(loadConstantInstruction);
    }
    else
    {
        PushValueInstruction* pushValueInstruction = new PushValueInstruction();
        pushValueInstruction->SetValue(new PtrValue(constant->GetValue()));
        emitter->Emit(pushValueInstruction);
    }
}

BoundExpressionNode* BoundConstantNode::Clone() const
{
    BoundConstantNode* clone = new BoundConstantNode(Span(), constant);
    clone->SetFlags(Flags());
    return clone;
}

void BoundConstantNode::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundVariableNode::BoundVariableNode(const soul::ast::Span& span_, VariableSymbol* variable_) :
    BoundExpressionNode(BoundNodeKind::boundVariableNode, span_, variable_->Type()), variable(variable_)
{
}

void BoundVariableNode::Load(Emitter* emitter)
{
    if (variable->IsGlobal())
    {
        LoadGlobalInstruction* loadGlobalInstruction = new LoadGlobalInstruction();
        loadGlobalInstruction->SetGlobalId(variable->GlobalId());
        emitter->Emit(loadGlobalInstruction);
    }
    else if (variable->IsParameterSymbol())
    {
        LoadLocalInstruction* loadLocalInstruction = new LoadLocalInstruction();
        loadLocalInstruction->SetLocalIndex(variable->Index());
        emitter->Emit(loadLocalInstruction);
    }
    else
    {
        if (!variable->CurrentBlock() || variable->CurrentBlock() == emitter->GetCurrentSubroutine()->CurrentBlock())
        {
            LoadLocalInstruction* loadLocalInstruction = new LoadLocalInstruction();
            loadLocalInstruction->SetLocalIndex(variable->Index());
            emitter->Emit(loadLocalInstruction);
        }
        else
        {
            LoadParentInstruction* loadParentInstruction = new LoadParentInstruction();
            loadParentInstruction->SetParentLevel(variable->CurrentBlock()->Level());
            loadParentInstruction->SetVariableIndex(variable->Index());
            emitter->Emit(loadParentInstruction);
        }
    }
}

void BoundVariableNode::Store(Emitter* emitter)
{
    if (variable->IsGlobal())
    {
        StoreGlobalInstruction* storeGlobalInstruction = new StoreGlobalInstruction();
        storeGlobalInstruction->SetGlobalId(variable->GlobalId());
        emitter->Emit(storeGlobalInstruction);
    }
    else if (variable->IsParameterSymbol())
    {
        StoreLocalInstruction* storeLocalInstruction = new StoreLocalInstruction();
        storeLocalInstruction->SetLocalIndex(variable->Index());
        emitter->Emit(storeLocalInstruction);
    }
    else
    {
        if (!variable->CurrentBlock() || variable->CurrentBlock() == emitter->GetCurrentSubroutine()->CurrentBlock())
        {
            StoreLocalInstruction* storeLocalInstruction = new StoreLocalInstruction();
            storeLocalInstruction->SetLocalIndex(variable->Index());
            emitter->Emit(storeLocalInstruction);
        }
        else
        {
            StoreParentInstruction* storeParentInstruction = new StoreParentInstruction();
            storeParentInstruction->SetParentLevel(variable->CurrentBlock()->Level());
            storeParentInstruction->SetVariableIndex(variable->Index());
            emitter->Emit(storeParentInstruction);
        }
    }
}

BoundExpressionNode* BoundVariableNode::Clone() const
{
    BoundVariableNode* clone = new BoundVariableNode(Span(), variable);
    clone->SetFlags(Flags());
    return clone;
}

void BoundVariableNode::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundProcedureNode::BoundProcedureNode(const soul::ast::Span& span_, ProcedureSymbol* procedure_) :
    BoundExpressionNode(BoundNodeKind::boundProcedureNode, span_, nullptr), procedure(procedure_)
{
}

BoundExpressionNode* BoundProcedureNode::Clone() const
{
    BoundProcedureNode* clone = new BoundProcedureNode(Span(), procedure);
    clone->SetFlags(Flags());
    return clone;
}

void BoundProcedureNode::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundFunctionNode::BoundFunctionNode(const soul::ast::Span& span_, FunctionSymbol* function_, const std::vector<TypeSymbol*>& argumentTypes, Node* node) :
    BoundExpressionNode(BoundNodeKind::boundFunctionNode, span_, function_->ResultType(argumentTypes, node)), function(function_)
{
}

BoundFunctionNode::BoundFunctionNode(const soul::ast::Span& span_, FunctionSymbol* function_, TypeSymbol* type_) : 
    BoundExpressionNode(BoundNodeKind::boundFunctionNode, span_, type_), function(function_)
{
}

BoundExpressionNode* BoundFunctionNode::Clone() const
{
    BoundFunctionNode* clone = new BoundFunctionNode(Span(), function, Type());
    clone->SetFlags(Flags());
    return clone;
}

void BoundFunctionNode::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

TypeSymbol* MethodType(SubroutineSymbol* method, const std::vector<TypeSymbol*>& argumentTypes, Node* node)
{
    if (method->IsFunctionSymbol())
    {
        FunctionSymbol* functionSymbol = static_cast<FunctionSymbol*>(method);
        return functionSymbol->ResultType(argumentTypes, node);
    }
    else
    {
        return nullptr;
    }
}

BoundMethodNode::BoundMethodNode(const soul::ast::Span& span_, BoundExpressionNode* subject_, SubroutineSymbol* method_, 
    const std::vector<TypeSymbol*>& argumentTypes_, Node* node_) :
    BoundExpressionNode(BoundNodeKind::boundMethodNode, span_, MethodType(method_, argumentTypes_, node_)), subject(subject_), method(method_)
{
}

BoundMethodNode::BoundMethodNode(const soul::ast::Span& span_, BoundExpressionNode* subject_, SubroutineSymbol* method_, TypeSymbol* type_) : 
    BoundExpressionNode(BoundNodeKind::boundMethodNode, span_, type_), subject(subject_), method(method_)
{
}

BoundExpressionNode* BoundMethodNode::Clone() const
{
    BoundMethodNode* clone = new BoundMethodNode(Span(), subject->Clone(), method, Type());
    clone->SetFlags(Flags());
    return clone;
}

void BoundMethodNode::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundSubroutineCallNode::BoundSubroutineCallNode(const soul::ast::Span& span_, SubroutineSymbol* subroutine_, TypeSymbol* type_) :
    BoundExpressionNode(BoundNodeKind::boundSubroutineCallNode, span_, type_), subroutine(subroutine_)
{
}

void BoundSubroutineCallNode::AddArgument(BoundExpressionNode* argument)
{
    argument->SetParent(this);
    arguments.push_back(std::unique_ptr<BoundExpressionNode>(argument));
}

void BoundSubroutineCallNode::Load(Emitter* emitter)
{
    for (const auto& argument : arguments)
    {
        argument->Load(emitter);
    }
    if (subroutine->IsExternal())
    {
        CallExternalInstruction* callExternalInstruction = new CallExternalInstruction();
        callExternalInstruction->SetSubroutine(subroutine);
        emitter->Emit(callExternalInstruction);
    }
    else if (GetFlag(ArgumentFlags::virtualCall))
    {
        if (!arguments.empty())
        {
            arguments.front()->Load(emitter);
            CallVirtualInstruction* instruction = new CallVirtualInstruction();
            instruction->SetMethod(subroutine);
            emitter->Emit(instruction);
        }
        else
        {
            ThrowError("error: method '" + subroutine->FullName() + "' arguments empty, cannot call virtual", emitter->FilePath(), Span());
        }
    }
    else
    {
        CallSubroutineInstruction* callSubrotineInstruction = new CallSubroutineInstruction();
        callSubrotineInstruction->SetSubroutine(subroutine);
        callSubrotineInstruction->SetArgumentCount(arguments.size());
        emitter->Emit(callSubrotineInstruction);
    }
}

BoundExpressionNode* BoundSubroutineCallNode::Clone() const
{
    BoundSubroutineCallNode* clone = new BoundSubroutineCallNode(Span(), subroutine, Type());
    for (const auto& argument : arguments)
    {
        clone->AddArgument(argument->Clone());
    }
    clone->SetFlags(Flags());
    return clone;
}

void BoundSubroutineCallNode::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundConstructorCallNode::BoundConstructorCallNode(const soul::ast::Span& span_, ConstructorSymbol* constructor_) :
    BoundExpressionNode(BoundNodeKind::boundConstructorCallNode, span_, nullptr), constructor(constructor_)
{
}

void BoundConstructorCallNode::Load(Emitter* emitter)
{
    for (const auto& argument : arguments)
    {
        argument->Load(emitter);
    }
    CallConstructorInstruction* callConstructorInstruction = new CallConstructorInstruction();
    callConstructorInstruction->SetConstructor(constructor);
    emitter->Emit(callConstructorInstruction);
}

BoundExpressionNode* BoundConstructorCallNode::Clone() const
{
    BoundConstructorCallNode* clone = new BoundConstructorCallNode(Span(), constructor);
    for (const auto& argument : arguments)
    {
        clone->AddArgument(argument->Clone());
    }
    clone->SetFlags(Flags());
    return clone;
}

void BoundConstructorCallNode::AddArgument(BoundExpressionNode* argument)
{
    if (arguments.empty())
    {
        SetType(argument->Type());
    }
    argument->SetParent(this);
    arguments.push_back(std::unique_ptr<BoundExpressionNode>(argument));
}

void BoundConstructorCallNode::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

TypeSymbol* GetArrayElementType(TypeSymbol* type)
{
    if (type->IsArrayTypeSymbol())
    {
        ArrayTypeSymbol* arrayType = static_cast<ArrayTypeSymbol*>(type);
        return arrayType->ElementType();
    }
    else
    {
        return nullptr;
    }
}

BoundIndexExprNode::BoundIndexExprNode(const soul::ast::Span& span_, BoundExpressionNode* subject_, BoundExpressionNode* index_) : 
    BoundExpressionNode(BoundNodeKind::boundIndexExprNode, span_, GetArrayElementType(subject_->Type())), subject(subject_), index(index_)
{
}

void BoundIndexExprNode::Load(Emitter* emitter)
{
    index->Load(emitter);
    subject->Load(emitter);
    emitter->Emit(new LoadElementInstruction());
}

void BoundIndexExprNode::Store(Emitter* emitter)
{
    index->Load(emitter);
    subject->Load(emitter);
    emitter->Emit(new StoreElementInstruction());
}

BoundExpressionNode* BoundIndexExprNode::Clone() const
{
    BoundIndexExprNode* clone = new BoundIndexExprNode(Span(), subject->Clone(), index->Clone());
    clone->SetFlags(Flags());
    return clone;
}

void BoundIndexExprNode::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundMemberExprNode::BoundMemberExprNode(const soul::ast::Span& span_, BoundExpressionNode* subject_, int32_t fieldIndex_, TypeSymbol* fieldType_) : 
    BoundExpressionNode(BoundNodeKind::boundMemberExprNode, span_, fieldType_), subject(subject_), fieldIndex(fieldIndex_)
{
}

void BoundMemberExprNode::Load(Emitter* emitter)
{
    subject->Load(emitter);
    LoadFieldInstruction* loadFieldInstruction = new LoadFieldInstruction();
    loadFieldInstruction->SetFieldIndex(fieldIndex);
    emitter->Emit(loadFieldInstruction);
}

void BoundMemberExprNode::Store(Emitter* emitter)
{
    subject->Load(emitter);
    StoreFieldInstruction* storeFieldInstruction = new StoreFieldInstruction();
    storeFieldInstruction->SetFieldIndex(fieldIndex);
    emitter->Emit(storeFieldInstruction);
}

BoundExpressionNode* BoundMemberExprNode::Clone() const
{
    BoundMemberExprNode* clone = new BoundMemberExprNode(Span(), subject->Clone(), fieldIndex, Type());
    clone->SetFlags(Flags());
    return clone;
}

void BoundMemberExprNode::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundNewExpressionNode::BoundNewExpressionNode(const soul::ast::Span& span_, ObjectTypeSymbol* objectType_) : 
    BoundExpressionNode(BoundNodeKind::boundNewExpressionNode, span_, objectType_), objectType(objectType_)
{
}

void BoundNewExpressionNode::Load(Emitter* emitter)
{
    NewObjectInstruction* newObjectInstruction = new NewObjectInstruction();
    newObjectInstruction->SetObjectTypeId(objectType->Id());
    emitter->Emit(newObjectInstruction);
}

BoundExpressionNode* BoundNewExpressionNode::Clone() const
{
    BoundNewExpressionNode* clone = new BoundNewExpressionNode(Span(), objectType);
    clone->SetFlags(Flags());
    return clone;
}

void BoundNewExpressionNode::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundNewArrayExpressionNode::BoundNewArrayExpressionNode(const soul::ast::Span& span_, TypeSymbol* elementType_, BoundExpressionNode* arraySize_, TypeSymbol* arrayType_) :
    BoundExpressionNode(BoundNodeKind::boundNewArrayExpressionNode, span_, arrayType_), elementType(elementType_), arraySize(arraySize_)
{
    arraySize->SetParent(this);
}

void BoundNewArrayExpressionNode::Load(Emitter* emitter)
{
    arraySize->Load(emitter);
    NewArrayInstruction* newArrayInstruction = new NewArrayInstruction();
    newArrayInstruction->SetArrayTypeId(Type()->Id());
    emitter->Emit(newArrayInstruction);
}

BoundExpressionNode* BoundNewArrayExpressionNode::Clone() const
{
    BoundNewArrayExpressionNode* clone = new BoundNewArrayExpressionNode(Span(), elementType, arraySize->Clone(), Type());
    clone->SetFlags(Flags());
    return clone;
}

void BoundNewArrayExpressionNode::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundArrayLengthNode::BoundArrayLengthNode(const soul::ast::Span& span_, BoundExpressionNode* subject_, TypeSymbol* type_) : 
    BoundExpressionNode(BoundNodeKind::boundArrayLengthNode, span_, type_), subject(subject_)
{
}

void BoundArrayLengthNode::Load(Emitter* emitter)
{
    subject->Load(emitter);
    emitter->Emit(new ArrayLengthInstruction());
}

BoundExpressionNode* BoundArrayLengthNode::Clone() const
{
    BoundArrayLengthNode* clone = new BoundArrayLengthNode(Span(), subject->Clone(), Type());
    clone->SetFlags(Flags());
    return clone;
}

void BoundArrayLengthNode::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundStringLengthNode::BoundStringLengthNode(const soul::ast::Span& span_, BoundExpressionNode* subject_, TypeSymbol* type_) : 
    BoundExpressionNode(BoundNodeKind::boundStringLengthNode, span_, type_), subject(subject_)
{
}

void BoundStringLengthNode::Load(Emitter* emitter)
{
    subject->Load(emitter);
    emitter->Emit(new StringLengthInstruction());
}

BoundExpressionNode* BoundStringLengthNode::Clone() const
{
    BoundStringLengthNode* clone = new BoundStringLengthNode(Span(), subject->Clone(), Type());
    clone->SetFlags(Flags());
    return clone;
}

void BoundStringLengthNode::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundTypeNode::BoundTypeNode(const soul::ast::Span& span_, TypeSymbol* type_) : BoundExpressionNode(BoundNodeKind::boundTypeNode, span_, type_)
{
}

BoundExpressionNode* BoundTypeNode::Clone() const
{
    BoundTypeNode* clone = new BoundTypeNode(Span(), Type());
    clone->SetFlags(Flags());
    return clone;
}

void BoundTypeNode::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundConversionNode::BoundConversionNode(const soul::ast::Span& span_, FunctionSymbol* conversionFunction_, BoundExpressionNode* argument_) :
    BoundExpressionNode(BoundNodeKind::boundConversionNode, span_, conversionFunction_->ResultType(std::vector<TypeSymbol*>(), nullptr)), 
    conversionFunction(conversionFunction_), argument(argument_)
{
    argument->SetParent(this);
}

void BoundConversionNode::Load(Emitter* emitter)
{
    argument->Load(emitter);
    conversionFunction->GenerateCode(emitter, Span());
}

BoundExpressionNode* BoundConversionNode::Clone() const
{
    BoundConversionNode* clone = new BoundConversionNode(Span(), conversionFunction, argument->Clone());
    clone->SetFlags(Flags());
    return clone;
}

void BoundConversionNode::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundValueConversionNode::BoundValueConversionNode(const soul::ast::Span& span_, BoundExpressionNode* argument_, TypeSymbol* targetType_) :
    BoundExpressionNode(BoundNodeKind::boundValueConversionNode, span_, targetType_), argument(argument_)
{
}

void BoundValueConversionNode::Load(Emitter* emitter)
{
    argument->Load(emitter);
}

BoundExpressionNode* BoundValueConversionNode::Clone() const
{
    BoundValueConversionNode* clone = new BoundValueConversionNode(Span(), argument->Clone(), Type());
    clone->SetFlags(Flags());
    return clone;
}

void BoundValueConversionNode::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

BoundFunctionResultNode::BoundFunctionResultNode(const soul::ast::Span& span_, VariableSymbol* functionResultVar_) : 
    BoundExpressionNode(BoundNodeKind::boundFunctionResultNode, span_, functionResultVar_->Type()), functionResultVar(functionResultVar_)
{
}

void BoundFunctionResultNode::Load(Emitter* emitter)
{
    LoadResultVarInstruction* loadResultVarInstruction = new LoadResultVarInstruction();
    loadResultVarInstruction->SetResultVarIndex(functionResultVar->Index());
    emitter->Emit(loadResultVarInstruction);
}

BoundExpressionNode* BoundFunctionResultNode::Clone() const
{
    BoundFunctionResultNode* clone = new BoundFunctionResultNode(Span(), functionResultVar);
    clone->SetFlags(Flags());
    return clone;
}

void BoundFunctionResultNode::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

} // namespace p
