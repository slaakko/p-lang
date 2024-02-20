// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.binder.evaluator; 

namespace p {

class Evaluator : public Visitor
{
public:
    Evaluator(Node* node_, Context* context_, BlockSymbol* currentBlock_);
    Value* GetValue() { return value.release(); }
    void Visit(BinaryExprNode& node) override;
    void Visit(UnaryExprNode& node) override;
    void Visit(ParenthesizedExprNode& node) override;
    void Visit(IntegerLiteralNode& node) override;
    void Visit(RealLiteralNode& node) override;
    void Visit(BooleanLiteralNode& node) override;
    void Visit(StringLiteralNode& node) override;
    void Visit(CharLiteralNode& node) override;
    void Visit(IdentifierNode& node) override;
    void Visit(ThisNode& node) override;
    void Visit(BaseNode& node) override;
    void Visit(NilNode& node) override;
    void Visit(NewExprNode& node) override;
    void Visit(NewArrayExprNode& node) override;
    void Visit(InvokeExprNode& node) override;
    void Visit(IndexExprNode& node) override;
    void Visit(DotNode& node) override;
    void Visit(ArrayConstantNode& node) override;
    void Visit(ObjectConstantNode& node) override;
    void Visit(FieldNode& node) override;
private:
    std::unique_ptr<Value> value;
    int32_t fieldIndex;
    Field field;
    Node* node;
    Context* context;
    BlockSymbol* currentBlock;
};

Evaluator::Evaluator(Node* node_, Context* context_, BlockSymbol* currentBlock_) : fieldIndex(-1), node(node_), context(context_), currentBlock(currentBlock_)
{
}

void Evaluator::Visit(BinaryExprNode& node)
{
    node.Left()->Accept(*this);
    std::unique_ptr<Value> left(value.release());
    node.Right()->Accept(*this);
    std::unique_ptr<Value> right(value.release());
    Value* leftValue = left->GetValue();
    Value* rightValue = right->GetValue();
    ValueKind kind = CommonType(leftValue->Kind(), rightValue->Kind());
    if (kind == ValueKind::none)
    {
        ThrowError("error: evaluator: invalid binary operation operands", node.FilePath(), node.Span());
    }
    switch (kind)
    {
        case ValueKind::booleanValue:
        {
            switch (node.Op())
            {
                case Operator::equal:
                {
                    value.reset(new BooleanValue(BooleanTypeSymbol::Equal(left->ToBoolean(), right->ToBoolean())));
                    value->SetType(context->GetSymbolTable()->GetType(GetBooleanTypeId(), &node, context));
                    break;
                }
                case Operator::notEqual:
                {
                    value.reset(new BooleanValue(BooleanTypeSymbol::NotEqual(left->ToBoolean(), right->ToBoolean())));
                    value->SetType(context->GetSymbolTable()->GetType(GetBooleanTypeId(), &node, context));
                    break;
                }
                case Operator::and_:
                {
                    value.reset(new BooleanValue(BooleanTypeSymbol::And(left->ToBoolean(), right->ToBoolean())));
                    value->SetType(context->GetSymbolTable()->GetType(GetBooleanTypeId(), &node, context));
                    break;
                }
                case Operator::or_:
                {
                    value.reset(new BooleanValue(BooleanTypeSymbol::Or(left->ToBoolean(), right->ToBoolean())));
                    value->SetType(context->GetSymbolTable()->GetType(GetBooleanTypeId(), &node, context));
                    break;
                }
                case Operator::xor_:
                {
                    value.reset(new BooleanValue(BooleanTypeSymbol::Xor(left->ToBoolean(), right->ToBoolean())));
                    value->SetType(context->GetSymbolTable()->GetType(GetBooleanTypeId(), &node, context));
                    break;
                }
                default:
                {
                    ThrowError("error: evaluator: invalid Boolean binary operation", node.FilePath(), node.Span());
                    break;
                }
            }
            break;
        }
        case ValueKind::integerValue:
        {
            switch (node.Op())
            {
                case Operator::equal:
                {
                    value.reset(new BooleanValue(IntegerTypeSymbol::Equal(left->ToInteger(), right->ToInteger())));
                    value->SetType(context->GetSymbolTable()->GetType(GetBooleanTypeId(), &node, context));
                    break;
                }
                case Operator::notEqual:
                {
                    value.reset(new BooleanValue(IntegerTypeSymbol::NotEqual(left->ToInteger(), right->ToInteger())));
                    value->SetType(context->GetSymbolTable()->GetType(GetBooleanTypeId(), &node, context));
                    break;
                }
                case Operator::less:
                {
                    value.reset(new BooleanValue(IntegerTypeSymbol::Less(left->ToInteger(), right->ToInteger())));
                    value->SetType(context->GetSymbolTable()->GetType(GetBooleanTypeId(), &node, context));
                    break;
                }
                case Operator::greater:
                {
                    value.reset(new BooleanValue(IntegerTypeSymbol::Greater(left->ToInteger(), right->ToInteger())));
                    value->SetType(context->GetSymbolTable()->GetType(GetBooleanTypeId(), &node, context));
                    break;
                }
                case Operator::lessOrEqual:
                {
                    value.reset(new BooleanValue(IntegerTypeSymbol::LessOrEqual(left->ToInteger(), right->ToInteger())));
                    value->SetType(context->GetSymbolTable()->GetType(GetBooleanTypeId(), &node, context));
                    break;
                }
                case Operator::greaterOrEqual:
                {
                    value.reset(new BooleanValue(IntegerTypeSymbol::GreaterOrEqual(left->ToInteger(), right->ToInteger())));
                    value->SetType(context->GetSymbolTable()->GetType(GetBooleanTypeId(), &node, context));
                    break;
                }
                case Operator::plus:
                {
                    value.reset(new IntegerValue(IntegerTypeSymbol::Plus(left->ToInteger(), right->ToInteger())));
                    value->SetType(context->GetSymbolTable()->GetType(GetIntegerTypeId(), &node, context));
                    break;
                }
                case Operator::minus:
                {
                    value.reset(new IntegerValue(IntegerTypeSymbol::Minus(left->ToInteger(), right->ToInteger())));
                    value->SetType(context->GetSymbolTable()->GetType(GetIntegerTypeId(), &node, context));
                    break;
                }
                case Operator::mul:
                {
                    value.reset(new IntegerValue(IntegerTypeSymbol::Multiply(left->ToInteger(), right->ToInteger())));
                    value->SetType(context->GetSymbolTable()->GetType(GetIntegerTypeId(), &node, context));
                    break;
                }
                case Operator::fractionalDivide:
                {
                    value.reset(new RealValue(IntegerTypeSymbol::FractionalDivide(left->ToInteger(), right->ToInteger())));
                    value->SetType(context->GetSymbolTable()->GetType(GetRealTypeId(), &node, context));
                    break;
                }
                case Operator::div:
                {
                    value.reset(new IntegerValue(IntegerTypeSymbol::Div(left->ToInteger(), right->ToInteger())));
                    value->SetType(context->GetSymbolTable()->GetType(GetIntegerTypeId(), &node, context));
                    break;
                }
                case Operator::mod:
                {
                    value.reset(new IntegerValue(IntegerTypeSymbol::Mod(left->ToInteger(), right->ToInteger())));
                    value->SetType(context->GetSymbolTable()->GetType(GetIntegerTypeId(), &node, context));
                    break;
                }
                case Operator::and_:
                {
                    value.reset(new IntegerValue(IntegerTypeSymbol::And(left->ToInteger(), right->ToInteger())));
                    value->SetType(context->GetSymbolTable()->GetType(GetIntegerTypeId(), &node, context));
                    break;
                }
                case Operator::or_:
                {
                    value.reset(new IntegerValue(IntegerTypeSymbol::Or(left->ToInteger(), right->ToInteger())));
                    value->SetType(context->GetSymbolTable()->GetType(GetIntegerTypeId(), &node, context));
                    break;
                }
                case Operator::xor_:
                {
                    value.reset(new IntegerValue(IntegerTypeSymbol::Xor(left->ToInteger(), right->ToInteger())));
                    value->SetType(context->GetSymbolTable()->GetType(GetIntegerTypeId(), &node, context));
                    break;
                }
                case Operator::shl:
                {
                    value.reset(new IntegerValue(IntegerTypeSymbol::Shl(left->ToInteger(), right->ToInteger())));
                    value->SetType(context->GetSymbolTable()->GetType(GetIntegerTypeId(), &node, context));
                    break;
                }
                case Operator::shr:
                {
                    value.reset(new IntegerValue(IntegerTypeSymbol::Shr(left->ToInteger(), right->ToInteger())));
                    value->SetType(context->GetSymbolTable()->GetType(GetIntegerTypeId(), &node, context));
                    break;
                }
                default:
                {
                    ThrowError("error: evaluator: invalid integer binary operation", node.FilePath(), node.Span());
                    break;
                }
            }
            break;
        }
        case ValueKind::charValue:
        {
            switch (node.Op())
            {
                case Operator::equal:
                {
                    value.reset(new BooleanValue(CharTypeSymbol::Equal(left->ToChar(), right->ToChar())));
                    value->SetType(context->GetSymbolTable()->GetType(GetBooleanTypeId(), &node, context));
                    break;
                }
                case Operator::notEqual:
                {
                    value.reset(new BooleanValue(CharTypeSymbol::NotEqual(left->ToChar(), right->ToChar())));
                    value->SetType(context->GetSymbolTable()->GetType(GetBooleanTypeId(), &node, context));
                    break;
                }
                default:
                {
                    ThrowError("error: evaluator: invalid character binary operation", node.FilePath(), node.Span());
                    break;
                }
            }
            break;
        }
        case ValueKind::realValue:
        {
            switch (node.Op())
            {
                case Operator::equal:
                {
                    value.reset(new BooleanValue(RealTypeSymbol::Equal(left->ToReal(), right->ToReal())));
                    value->SetType(context->GetSymbolTable()->GetType(GetBooleanTypeId(), &node, context));
                    break;
                }
                case Operator::notEqual:
                {
                    value.reset(new BooleanValue(RealTypeSymbol::NotEqual(left->ToReal(), right->ToReal())));
                    value->SetType(context->GetSymbolTable()->GetType(GetBooleanTypeId(), &node, context));
                    break;
                }
                case Operator::less:
                {
                    value.reset(new BooleanValue(RealTypeSymbol::Less(left->ToReal(), right->ToReal())));
                    value->SetType(context->GetSymbolTable()->GetType(GetBooleanTypeId(), &node, context));
                    break;
                }
                case Operator::greater:
                {
                    value.reset(new BooleanValue(RealTypeSymbol::Greater(left->ToReal(), right->ToReal())));
                    value->SetType(context->GetSymbolTable()->GetType(GetBooleanTypeId(), &node, context));
                    break;
                }
                case Operator::lessOrEqual:
                {
                    value.reset(new BooleanValue(RealTypeSymbol::LessOrEqual(left->ToReal(), right->ToReal())));
                    value->SetType(context->GetSymbolTable()->GetType(GetBooleanTypeId(), &node, context));
                    break;
                }
                case Operator::greaterOrEqual:
                {
                    value.reset(new BooleanValue(RealTypeSymbol::GreaterOrEqual(left->ToReal(), right->ToReal())));
                    value->SetType(context->GetSymbolTable()->GetType(GetBooleanTypeId(), &node, context));
                    break;
                }
                case Operator::plus:
                {
                    value.reset(new RealValue(RealTypeSymbol::Plus(left->ToReal(), right->ToReal())));
                    value->SetType(context->GetSymbolTable()->GetType(GetRealTypeId(), &node, context));
                    break;
                }
                case Operator::minus:
                {
                    value.reset(new RealValue(RealTypeSymbol::Minus(left->ToReal(), right->ToReal())));
                    value->SetType(context->GetSymbolTable()->GetType(GetRealTypeId(), &node, context));
                    break;
                }
                case Operator::mul:
                {
                    value.reset(new RealValue(RealTypeSymbol::Multiply(left->ToReal(), right->ToReal())));
                    value->SetType(context->GetSymbolTable()->GetType(GetRealTypeId(), &node, context));
                    break;
                }
                case Operator::fractionalDivide:
                {
                    value.reset(new RealValue(RealTypeSymbol::FractionalDivide(left->ToReal(), right->ToReal())));
                    value->SetType(context->GetSymbolTable()->GetType(GetRealTypeId(), &node, context));
                    break;
                }
                default:
                {
                    ThrowError("error: evaluator: invalid real binary operation", node.FilePath(), node.Span());
                    break;
                }
            }
            break;
        }
        case ValueKind::stringValue:
        {
            switch (node.Op())
            {
                case Operator::equal:
                {
                    value.reset(new BooleanValue(StringTypeSymbol::Equal(left->ToString(), right->ToString())));
                    value->SetType(context->GetSymbolTable()->GetType(GetBooleanTypeId(), &node, context));
                    break;
                }
                case Operator::notEqual:
                {
                    value.reset(new BooleanValue(StringTypeSymbol::NotEqual(left->ToString(), right->ToString())));
                    value->SetType(context->GetSymbolTable()->GetType(GetBooleanTypeId(), &node, context));
                    break;
                }
                case Operator::less:
                {
                    value.reset(new BooleanValue(StringTypeSymbol::Less(left->ToString(), right->ToString())));
                    value->SetType(context->GetSymbolTable()->GetType(GetBooleanTypeId(), &node, context));
                    break;
                }
                case Operator::greater:
                {
                    value.reset(new BooleanValue(StringTypeSymbol::Greater(left->ToString(), right->ToString())));
                    value->SetType(context->GetSymbolTable()->GetType(GetBooleanTypeId(), &node, context));
                    break;
                }
                case Operator::lessOrEqual:
                {
                    value.reset(new BooleanValue(StringTypeSymbol::LessOrEqual(left->ToString(), right->ToString())));
                    value->SetType(context->GetSymbolTable()->GetType(GetBooleanTypeId(), &node, context));
                    break;
                }
                case Operator::greaterOrEqual:
                {
                    value.reset(new BooleanValue(StringTypeSymbol::GreaterOrEqual(left->ToString(), right->ToString())));
                    value->SetType(context->GetSymbolTable()->GetType(GetBooleanTypeId(), &node, context));
                    break;
                }
                case Operator::plus:
                {
                    value.reset(new StringValue(StringTypeSymbol::Plus(left->ToString(), right->ToString())));
                    value->SetType(context->GetSymbolTable()->GetType(GetStringTypeId(), &node, context));
                    break;
                }
                default:
                {
                    ThrowError("error: evaluator: invalid string binary operation", node.FilePath(), node.Span());
                    break;
                }
            }
            break;
        }
    }
}

void Evaluator::Visit(UnaryExprNode& node)
{
    node.Operand()->Accept(*this);
    std::unique_ptr<Value> operand(value.release());
    Value* operandValue = operand->GetValue();
    ValueKind kind = operandValue->Kind();
    switch (kind)
    {
        case ValueKind::booleanValue:
        {
            switch (node.Op())
            {
                case Operator::not_:
                {
                    value.reset(new BooleanValue(BooleanTypeSymbol::Not(operandValue->ToBoolean())));
                    value->SetType(context->GetSymbolTable()->GetType(GetBooleanTypeId(), &node, context));
                    break;
                }
                default:
                {
                    ThrowError("error: evaluator: invalid Boolean unary operation", node.FilePath(), node.Span());
                    break;
                }
            }
            break;
        }
        case ValueKind::integerValue:
        {
            switch (node.Op())
            {
                case Operator::not_:
                {
                    value.reset(new IntegerValue(IntegerTypeSymbol::Not(operandValue->ToInteger())));
                    value->SetType(context->GetSymbolTable()->GetType(GetIntegerTypeId(), &node, context));
                    break;
                }
                case Operator::plus:
                {
                    value.reset(new IntegerValue(IntegerTypeSymbol::UnaryPlus(operandValue->ToInteger())));
                    value->SetType(context->GetSymbolTable()->GetType(GetIntegerTypeId(), &node, context));
                    break;
                }
                case Operator::minus:
                {
                    value.reset(new IntegerValue(IntegerTypeSymbol::UnaryMinus(operandValue->ToInteger())));
                    value->SetType(context->GetSymbolTable()->GetType(GetIntegerTypeId(), &node, context));
                    break;
                }
                default:
                {
                    ThrowError("error: evaluator: invalid integer unary operation", node.FilePath(), node.Span());
                    break;
                }
            }
            break;
        }
        case ValueKind::realValue:
        {
            switch (node.Op())
            {
                case Operator::plus:
                {
                    value.reset(new RealValue(RealTypeSymbol::UnaryPlus(operandValue->ToReal())));
                    value->SetType(context->GetSymbolTable()->GetType(GetRealTypeId(), &node, context));
                    break;
                }
                case Operator::minus:
                {
                    value.reset(new RealValue(RealTypeSymbol::UnaryMinus(operandValue->ToReal())));
                    value->SetType(context->GetSymbolTable()->GetType(GetRealTypeId(), &node, context));
                    break;
                }
                default:
                {
                    ThrowError("error: evaluator: invalid real unary operation", node.FilePath(), node.Span());
                    break;
                }
            }
            break;
        }
        default:
        {
            ThrowError("error: evaluator: invalid unary operation operand", node.FilePath(), node.Span());
            break;
        }
    }
}

void Evaluator::Visit(ParenthesizedExprNode& node)
{
    node.Expr()->Accept(*this);
}

void Evaluator::Visit(IntegerLiteralNode& node)
{
    value.reset(new IntegerValue(node.Value()));
    value->SetType(context->GetSymbolTable()->GetType(GetIntegerTypeId(), &node, context));
}

void Evaluator::Visit(RealLiteralNode& node)
{
    value.reset(new RealValue(node.Value()));
    value->SetType(context->GetSymbolTable()->GetType(GetRealTypeId(), &node, context));
}

void Evaluator::Visit(BooleanLiteralNode& node)
{
    value.reset(new BooleanValue(node.Value()));
    value->SetType(context->GetSymbolTable()->GetType(GetBooleanTypeId(), &node, context));
}

void Evaluator::Visit(StringLiteralNode& node)
{
    value.reset(new StringValue(node.Value()));
    value->SetType(context->GetSymbolTable()->GetType(GetStringTypeId(), &node, context));
}

void Evaluator::Visit(CharLiteralNode& node)
{
    value.reset(new CharValue(node.Value()));
    value->SetType(context->GetSymbolTable()->GetType(GetCharTypeId(), &node, context));
}

void Evaluator::Visit(IdentifierNode& node)
{
    Symbol* symbol = currentBlock->GetSymbol(node.Str(), &node, true, true);
    if (symbol->IsFunctionSymbol())
    {
        FunctionSymbol* function = static_cast<FunctionSymbol*>(symbol);
        value.reset(new FunctionValue(function));
    }
    else
    {
        switch (symbol->Kind())
        {
            case SymbolKind::constantSymbol:
            {
                ConstantSymbol* constant = static_cast<ConstantSymbol*>(symbol);
                value.reset(new ConstantValue(constant->GetValue()));
                value->SetType(constant->Type());
                break;
            }
            case SymbolKind::functionSymbol:
            {
                FunctionSymbol* function = static_cast<FunctionSymbol*>(symbol);
                value.reset(new FunctionValue(function));
                break;
            }
            default:
            {
                ThrowError("error: constant or function identifier expected", node.FilePath(), node.Span());
                break;
            }
        }
    }
}

void Evaluator::Visit(ThisNode& node)
{
    ThrowError("error: evaluator 'this' cannot be evaluated statically", node.FilePath(), node.Span());
}

void Evaluator::Visit(BaseNode& node)
{
    ThrowError("error: evaluator 'base' cannot be evaluated statically", node.FilePath(), node.Span());
}

void Evaluator::Visit(NilNode& node)
{
    ThrowError("error: evaluator 'nil' cannot be evaluated statically", node.FilePath(), node.Span());
}

void Evaluator::Visit(NewExprNode& node)
{
    ThrowError("error: evaluator 'new' cannot be evaluated statically", node.FilePath(), node.Span());
}

void Evaluator::Visit(NewArrayExprNode& node)
{
    ThrowError("error: evaluator 'new array' cannot be evaluated statically", node.FilePath(), node.Span());
}

void Evaluator::Visit(InvokeExprNode& node)
{
    node.Subject()->Accept(*this);
    std::unique_ptr<Value> subject(value.release());
    Value* subjectValue = subject->GetValue();
    if (subjectValue->IsFunctionValue())
    {
        FunctionValue* functionValue = static_cast<FunctionValue*>(subjectValue);
        FunctionSymbol* function = functionValue->GetFunction();
        std::vector<std::unique_ptr<Value>> argumentValues;
        for (const auto& arg : node.Arguments())
        {
            arg->Accept(*this);
            argumentValues.push_back(std::unique_ptr<Value>(value.release()));
        }
        value.reset(function->Evaluate(argumentValues, context, &node));
    }
    else
    {
        ThrowError("error: evaluator: function value expected", node.FilePath(), node.Span());
    }
}

void Evaluator::Visit(IndexExprNode& node)
{
    node.Subject()->Accept(*this);
    std::unique_ptr<Value> subject(value.release());
    if (subject && subject->GetValue()->IsArrayValue())
    {
        ArrayValue* arrayValue = static_cast<ArrayValue*>(subject->GetValue());
        node.Index()->Accept(*this);
        std::unique_ptr<Value> indexValue(value.release());
        if (indexValue && indexValue->IsIntegerValue())
        {
            int32_t index = indexValue->ToInteger();
            value.reset(new ConstantValue(arrayValue->GetElement(index, &node)));
        }
        else
        {
            ThrowError("error: evaluator: cannot evaluate statically", node.FilePath(), node.Span());
        }
    }
    else
    {
        ThrowError("error: evaluator: cannot evaluate statically", node.FilePath(), node.Span());
    }
}

void Evaluator::Visit(DotNode& node)
{
    node.Subject()->Accept(*this);
    std::unique_ptr<Value> subject(value.release());
    if (subject && subject->GetValue()->IsObjectValue())
    {
        ObjectValue* objectValue = static_cast<ObjectValue*>(subject->GetValue());
        const Field& field = objectValue->GetField(node.Id()->Str(), &node);
        value.reset(new ConstantValue(field.GetValue()));
    }
    else
    {
        ThrowError("error: evaluator: cannot evaluate statically", node.FilePath(), node.Span());
    }
}

void Evaluator::Visit(ArrayConstantNode& node)
{
    ArrayValue* arrayValue = new ArrayValue();
    for (const auto& element : node.Elements())
    {
        element->Accept(*this);
        arrayValue->AddElement(value.release());
    }
    value.reset(arrayValue);
}

void Evaluator::Visit(ObjectConstantNode& node)
{
    ObjectValue* objectValue = new ObjectValue();
    int32_t prevFieldIndex = fieldIndex;
    fieldIndex = 0;
    for (const auto& fieldNode : node.Fields())
    {
        fieldNode->Accept(*this);
        objectValue->AddField(std::move(field));
        ++fieldIndex;
    }
    fieldIndex = prevFieldIndex;
    value.reset(objectValue);
}

void Evaluator::Visit(FieldNode& node)
{
    node.Constant()->Accept(*this);
    field = Field(fieldIndex, node.FieldName()->Str(), value.release());
}

Value* Evaluate(Node* node, Context* context, BlockSymbol* currentBlock)
{
    Evaluator evaluator(node, context, currentBlock);
    node->Accept(evaluator);
    return evaluator.GetValue();
}

} // namespace p
