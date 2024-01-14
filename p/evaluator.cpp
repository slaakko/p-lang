// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.evaluator;

import p.visitor;
import p.ast;
import p.value;
import p.constant;
import p.block;
import p.type;
import p.subroutine;

namespace p {

class EvaluatorVisitor : public Visitor
{
public:
    EvaluatorVisitor(soul::lexer::LexerBase<char>& lexer_, Block* block_);
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
    void Visit(QualifiedIdNode& node) override;
    void Visit(ThisNode& node) override;
    void Visit(BaseNode& node) override;
    void Visit(NilNode& node) override;
    void Visit(NewExprNode& node) override;
    void Visit(NewArrayExprNode& node) override;
    void Visit(ValueTypecastNode& node) override;
    void Visit(VariableTypecastNode& node) override;
    void Visit(InvokeExprNode& node) override;
    void Visit(IndexExprNode& node) override;
    void Visit(DotNode& node) override;
private:
    soul::lexer::LexerBase<char>& lexer;
    Block* block;
    std::unique_ptr<Value> value;
};

EvaluatorVisitor::EvaluatorVisitor(soul::lexer::LexerBase<char>& lexer_, Block* block_) : lexer(lexer_), block(block_)
{
}

void EvaluatorVisitor::Visit(BinaryExprNode& node)
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
        ThrowError("error: invalid binary operation operands", lexer, node.Pos());
    }
    switch (kind)
    {
        case ValueKind::booleanValue:
        {
            switch (node.Op())
            {
                case Operator::equal:
                {
                    value.reset(new BooleanValue(BooleanType::Equal(left->ToBoolean(), right->ToBoolean())));
                    break;
                }
                case Operator::notEqual:
                {
                    value.reset(new BooleanValue(BooleanType::NotEqual(left->ToBoolean(), right->ToBoolean())));
                    break;
                }
                case Operator::and_:
                {
                    value.reset(new BooleanValue(BooleanType::And(left->ToBoolean(), right->ToBoolean())));
                    break;
                }
                case Operator::or_:
                {
                    value.reset(new BooleanValue(BooleanType::Or(left->ToBoolean(), right->ToBoolean())));
                    break;
                }
                case Operator::xor_:
                {
                    value.reset(new BooleanValue(BooleanType::Xor(left->ToBoolean(), right->ToBoolean())));
                    break;
                }
                default:
                {
                    ThrowError("error: invalid Boolean binary operation", lexer, node.Pos());
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
                    value.reset(new BooleanValue(IntegerType::Equal(left->ToInteger(), right->ToInteger())));
                    break;
                }
                case Operator::notEqual:
                {
                    value.reset(new BooleanValue(IntegerType::NotEqual(left->ToInteger(), right->ToInteger())));
                    break;
                }
                case Operator::less:
                {
                    value.reset(new BooleanValue(IntegerType::Less(left->ToInteger(), right->ToInteger())));
                    break;
                }
                case Operator::greater:
                {
                    value.reset(new BooleanValue(IntegerType::Greater(left->ToInteger(), right->ToInteger())));
                    break;
                }
                case Operator::lessOrEqual:
                {
                    value.reset(new BooleanValue(IntegerType::LessOrEqual(left->ToInteger(), right->ToInteger())));
                    break;
                }
                case Operator::greaterOrEqual:
                {
                    value.reset(new BooleanValue(IntegerType::GreaterOrEqual(left->ToInteger(), right->ToInteger())));
                    break;
                }
                case Operator::plus:
                {
                    value.reset(new IntegerValue(IntegerType::Plus(left->ToInteger(), right->ToInteger())));
                    break;
                }
                case Operator::minus:
                {
                    value.reset(new IntegerValue(IntegerType::Minus(left->ToInteger(), right->ToInteger())));
                    break;
                }
                case Operator::mul:
                {
                    value.reset(new IntegerValue(IntegerType::Multiply(left->ToInteger(), right->ToInteger())));
                    break;
                }
                case Operator::fractionalDivide:
                {
                    value.reset(new RealValue(IntegerType::FractionalDivide(left->ToInteger(), right->ToInteger())));
                    break;
                }
                case Operator::div:
                {
                    value.reset(new IntegerValue(IntegerType::Div(left->ToInteger(), right->ToInteger())));
                    break;
                }
                case Operator::mod:
                {
                    value.reset(new IntegerValue(IntegerType::Mod(left->ToInteger(), right->ToInteger())));
                    break;
                }
                case Operator::and_:
                {
                    value.reset(new IntegerValue(IntegerType::And(left->ToInteger(), right->ToInteger())));
                    break;
                }
                case Operator::or_:
                {
                    value.reset(new IntegerValue(IntegerType::Or(left->ToInteger(), right->ToInteger())));
                    break;
                }
                case Operator::xor_:
                {
                    value.reset(new IntegerValue(IntegerType::Xor(left->ToInteger(), right->ToInteger())));
                    break;
                }
                case Operator::shl:
                {
                    value.reset(new IntegerValue(IntegerType::Shl(left->ToInteger(), right->ToInteger())));
                    break;
                }
                case Operator::shr:
                {
                    value.reset(new IntegerValue(IntegerType::Shr(left->ToInteger(), right->ToInteger())));
                    break;
                }
                default:
                {
                    ThrowError("error: invalid integer binary operation", lexer, node.Pos());
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
                    value.reset(new BooleanValue(IntegerType::Equal(left->ToChar(), right->ToChar())));
                    break;
                }
                case Operator::notEqual:
                {
                    value.reset(new BooleanValue(IntegerType::NotEqual(left->ToChar(), right->ToChar())));
                    break;
                }
                default:
                {
                    ThrowError("error: invalid character binary operation", lexer, node.Pos());
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
                    value.reset(new BooleanValue(RealType::Equal(left->ToReal(), right->ToReal())));
                    break;
                }
                case Operator::notEqual:
                {
                    value.reset(new BooleanValue(RealType::NotEqual(left->ToReal(), right->ToReal())));
                    break;
                }
                case Operator::less:
                {
                    value.reset(new BooleanValue(RealType::Less(left->ToReal(), right->ToReal())));
                    break;
                }
                case Operator::greater:
                {
                    value.reset(new BooleanValue(RealType::Greater(left->ToReal(), right->ToReal())));
                    break;
                }
                case Operator::lessOrEqual:
                {
                    value.reset(new BooleanValue(RealType::LessOrEqual(left->ToReal(), right->ToReal())));
                    break;
                }
                case Operator::greaterOrEqual:
                {
                    value.reset(new BooleanValue(RealType::GreaterOrEqual(left->ToReal(), right->ToReal())));
                    break;
                }
                case Operator::plus:
                {
                    value.reset(new RealValue(RealType::Plus(left->ToReal(), right->ToReal())));
                    break;
                }
                case Operator::minus:
                {
                    value.reset(new RealValue(RealType::Minus(left->ToReal(), right->ToReal())));
                    break;
                }
                case Operator::mul:
                {
                    value.reset(new RealValue(RealType::Multiply(left->ToReal(), right->ToReal())));
                    break;
                }
                case Operator::fractionalDivide:
                {
                    value.reset(new RealValue(RealType::FractionalDivide(left->ToReal(), right->ToReal())));
                    break;
                }
                default:
                {
                    ThrowError("error: invalid real binary operation", lexer, node.Pos());
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
                    value.reset(new BooleanValue(StringType::Equal(left->ToString(), right->ToString())));
                    break;
                }
                case Operator::notEqual:
                {
                    value.reset(new BooleanValue(StringType::NotEqual(left->ToString(), right->ToString())));
                    break;
                }
                case Operator::less:
                {
                    value.reset(new BooleanValue(StringType::Less(left->ToString(), right->ToString())));
                    break;
                }
                case Operator::greater:
                {
                    value.reset(new BooleanValue(StringType::Greater(left->ToString(), right->ToString())));
                    break;
                }
                case Operator::lessOrEqual:
                {
                    value.reset(new BooleanValue(StringType::LessOrEqual(left->ToString(), right->ToString())));
                    break;
                }
                case Operator::greaterOrEqual:
                {
                    value.reset(new BooleanValue(StringType::GreaterOrEqual(left->ToString(), right->ToString())));
                    break;
                }
                case Operator::plus:
                {
                    value.reset(new StringValue(StringType::Plus(left->ToString(), right->ToString())));
                    break;
                }
                default:
                {
                    ThrowError("error: invalid string binary operation", lexer, node.Pos());
                    break;
                }
            }
            break;
        }
    }
}

void EvaluatorVisitor::Visit(UnaryExprNode& node)
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
                    value.reset(new BooleanValue(BooleanType::Not(operandValue->ToBoolean())));
                    break;
                }
                default:
                {
                    ThrowError("error: invalid Boolean unary operation", lexer, node.Pos());
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
                    value.reset(new IntegerValue(IntegerType::Not(operandValue->ToInteger())));
                    break;
                }
                case Operator::plus:
                {
                    value.reset(new IntegerValue(IntegerType::UnaryPlus(operandValue->ToInteger())));
                    break;
                }
                case Operator::minus:
                {
                    value.reset(new IntegerValue(IntegerType::UnaryMinus(operandValue->ToInteger())));
                    break;
                }
                default:
                {
                    ThrowError("error: invalid integer unary operation", lexer, node.Pos());
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
                    value.reset(new RealValue(RealType::UnaryPlus(operandValue->ToReal())));
                    break;
                }
                case Operator::minus:
                {
                    value.reset(new RealValue(RealType::UnaryMinus(operandValue->ToReal())));
                    break;
                }
                default:
                {
                    ThrowError("error: invalid real unary operation", lexer, node.Pos());
                }
            }
            break;
        }
        default:
        {
            ThrowError("error: invalid unary operation operand", lexer, node.Pos());
        }
    }
}

void EvaluatorVisitor::Visit(ParenthesizedExprNode& node)
{
    node.Expr()->Accept(*this);
}

void EvaluatorVisitor::Visit(IntegerLiteralNode& node)
{
    value.reset(new IntegerValue(node.Value()));
}

void EvaluatorVisitor::Visit(RealLiteralNode& node)
{
    value.reset(new RealValue(node.Value()));
}

void EvaluatorVisitor::Visit(BooleanLiteralNode& node)
{
    value.reset(new BooleanValue(node.Value()));
}

void EvaluatorVisitor::Visit(StringLiteralNode& node)
{
    value.reset(new StringValue(node.Value()));
}

void EvaluatorVisitor::Visit(CharLiteralNode& node)
{
    value.reset(new CharValue(node.Chr()));
}

void EvaluatorVisitor::Visit(IdentifierNode& node)
{
    switch (node.Kind())
    {
        case IdentifierKind::constant:
        {
            Constant* constant = node.GetConstant();
            value.reset(new ConstantValue(constant->GetValue()));
            break;
        }
        case IdentifierKind::function:
        {
            Function* function = node.GetFunction();
            value.reset(new FunctionValue(function));
            break;
        }
        default:
        {
            ThrowError("error: constant or function identifier expected", lexer, node.Pos());
            break;
        }
    }
}

void EvaluatorVisitor::Visit(QualifiedIdNode& node)
{
    ThrowError("error: constant identifier expected", lexer, node.Pos());
}

void EvaluatorVisitor::Visit(ThisNode& node)
{
    ThrowError("error: 'this' cannot be evaluated statically", lexer, node.Pos());
}

void EvaluatorVisitor::Visit(BaseNode& node)
{
    ThrowError("error: 'base' cannot be evaluated statically", lexer, node.Pos());
}

void EvaluatorVisitor::Visit(NilNode& node)
{
    ThrowError("error: nil cannot be evaluated statically", lexer, node.Pos());
}

void EvaluatorVisitor::Visit(NewExprNode& node)
{
    ThrowError("error: new cannot be evaluated statically", lexer, node.Pos());
}

void EvaluatorVisitor::Visit(NewArrayExprNode& node)
{
    ThrowError("error: new array cannot be evaluated statically", lexer, node.Pos());
}

void EvaluatorVisitor::Visit(ValueTypecastNode& node)
{
    ThrowError("error: value cast cannot be evaluated statically", lexer, node.Pos()); // todo
}

void EvaluatorVisitor::Visit(VariableTypecastNode& node)
{
    ThrowError("error: variable cast cannot be evaluated statically", lexer, node.Pos());
}

void EvaluatorVisitor::Visit(InvokeExprNode& node)
{
    node.Subject()->Accept(*this);
    std::unique_ptr<Value> subject(value.release());
    Value* subjectValue = subject->GetValue();
    if (subjectValue->IsFunctionValue())
    {
        FunctionValue* functionValue = static_cast<FunctionValue*>(subjectValue);
        Function* function = functionValue->GetFunction();
        std::vector<std::unique_ptr<Value>> argumentValues;
        for (const auto& arg : node.Arguments())
        {
            arg->Accept(*this);
            argumentValues.push_back(std::unique_ptr<Value>(value.release()));
        }
        value.reset(function->Evaluate(argumentValues, lexer, node.Pos()));
    }
    else
    {
        ThrowError("error: function value expected", lexer, node.Pos());
    }
}

void EvaluatorVisitor::Visit(IndexExprNode& node) 
{
    ThrowError("error: variable cast cannot be evaluated statically", lexer, node.Pos());
}

void EvaluatorVisitor::Visit(DotNode& node)
{
    ThrowError("error: variable cast cannot be evaluated statically", lexer, node.Pos());
}

Value* Evaluate(Node* node, soul::lexer::LexerBase<char>& lexer, Block* block)
{
    EvaluatorVisitor visitor(lexer, block);
    node->Accept(visitor);
    return visitor.GetValue();
}

} // namespace p
