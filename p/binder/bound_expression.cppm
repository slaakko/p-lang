// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.binder.bound_expression;

import p.binder.bound_node;
import p.core;
import p.ast;
import soul.ast.span;
import std.core;

export namespace p {

class Emitter;

enum class ArgumentFlags
{
    none, varArgument = 1 << 0, constantArgument = 1 << 1, thisOrBaseArgument = 1 << 2, virtualCall = 1 << 3
};

constexpr ArgumentFlags operator|(ArgumentFlags left, ArgumentFlags right)
{
    return ArgumentFlags(int(left) | int(right));
}

constexpr ArgumentFlags operator&(ArgumentFlags left, ArgumentFlags right)
{
    return ArgumentFlags(int(left) & int(right));
}

constexpr ArgumentFlags operator&(ArgumentFlags flags)
{
    return ArgumentFlags(~int(flags));
}

class BoundExpressionNode : public BoundNode
{
public:
    BoundExpressionNode(BoundNodeKind kind_, const soul::ast::Span& span_, TypeSymbol* type_);
    TypeSymbol* Type() const { return type; }
    void SetType(TypeSymbol* type_) { type = type_; }
    void SetFlag(ArgumentFlags flag) { flags = (flags | flag); }
    bool GetFlag(ArgumentFlags flag) const { return (flags & flag) != ArgumentFlags::none; }
    ArgumentFlags Flags() const { return flags; }
    void SetFlags(ArgumentFlags flags_) { flags = flags_; }
    virtual void Load(Emitter* emitter);
    virtual void Store(Emitter* emitter);
    virtual BoundExpressionNode* Clone() const = 0;
    virtual bool IsConst() const { return false; }
private:
    TypeSymbol* type;
    ArgumentFlags flags;
};

class BoundBinaryExpressionNode : public BoundExpressionNode
{
public:
    BoundBinaryExpressionNode(const soul::ast::Span& span_, BoundExpressionNode* left_, BoundExpressionNode* right_, FunctionSymbol* operatorFunction_, Node* node_);
    BoundExpressionNode* Left() const { return left.get(); }
    BoundExpressionNode* Right() const { return right.get(); }
    FunctionSymbol* OperatorFunction() const { return operatorFunction; }
    void Load(Emitter* emitter) override;
    BoundExpressionNode* Clone() const override;
    bool IsConst() const override;
    void Accept(BoundNodeVisitor& visitor) override;
private:
    std::unique_ptr<BoundExpressionNode> left;
    std::unique_ptr<BoundExpressionNode> right;
    FunctionSymbol* operatorFunction;
    Node* node;
};

class BoundUnaryExpressionNode : public BoundExpressionNode
{
public:
    BoundUnaryExpressionNode(const soul::ast::Span& span_, BoundExpressionNode* operand_, FunctionSymbol* operatorFunction_, Node* node_);
    BoundExpressionNode* Operand() const { return operand.get(); }
    FunctionSymbol* OperatorFunction() const { return operatorFunction; }
    void Load(Emitter* emitter) override;
    BoundExpressionNode* Clone() const override;
    bool IsConst() const override;
    void Accept(BoundNodeVisitor& visitor) override;
private:
    std::unique_ptr<BoundExpressionNode> operand;
    FunctionSymbol* operatorFunction;
    Node* node;
};

class BoundParameterNode : public BoundExpressionNode
{
public:
    BoundParameterNode(const soul::ast::Span& span_, ParameterSymbol* parameter_);
    BoundParameterNode(const soul::ast::Span& span_, ParameterSymbol* parameter_, TypeSymbol* type_);
    void Load(Emitter* emitter) override;
    BoundExpressionNode* Clone() const override;
    bool IsConst() const override;
    void Accept(BoundNodeVisitor& visitor) override;
private:
    ParameterSymbol* parameter;
};

class BoundLiteralNode : public BoundExpressionNode
{
public:
    BoundLiteralNode(const soul::ast::Span& span_, Value* value_, TypeSymbol* type_);
    Value* GetValue() const { return value.get(); }
    void Load(Emitter* emitter) override;
    BoundExpressionNode* Clone() const override;
    bool IsConst() const override { return true; }
    void Accept(BoundNodeVisitor& visitor) override;
private:
    std::unique_ptr<Value> value;
};

class BoundConstantNode : public BoundExpressionNode
{
public:
    BoundConstantNode(const soul::ast::Span& span_, ConstantSymbol* constant_);
    ConstantSymbol* Constant() const { return constant; }
    void Load(Emitter* emitter) override;
    BoundExpressionNode* Clone() const override;
    bool IsConst() const override { return true; }
    void Accept(BoundNodeVisitor& visitor) override;
private:
    ConstantSymbol* constant;
};

class BoundVariableNode : public BoundExpressionNode
{
public:
    BoundVariableNode(const soul::ast::Span& span_, VariableSymbol* variable_);
    VariableSymbol* Variable() const { return variable; }
    void Load(Emitter* emitter) override;
    void Store(Emitter* emitter) override;
    BoundExpressionNode* Clone() const override;
    void Accept(BoundNodeVisitor& visitor) override;
private:
    VariableSymbol* variable;
};

class BoundProcedureNode : public BoundExpressionNode
{
public:
    BoundProcedureNode(const soul::ast::Span& span_, ProcedureSymbol* procedure_);
    ProcedureSymbol* Procedure() const { return procedure; }
    BoundExpressionNode* Clone() const override;
    void Accept(BoundNodeVisitor& visitor) override;
private:
    ProcedureSymbol* procedure;
};

class BoundFunctionNode : public BoundExpressionNode
{
public:
    BoundFunctionNode(const soul::ast::Span& span_, FunctionSymbol* function_, const std::vector<TypeSymbol*>& argumentTypes, Node* node);
    BoundFunctionNode(const soul::ast::Span& span_, FunctionSymbol* function_, TypeSymbol* type_);
    FunctionSymbol* Function() const { return function; }
    BoundExpressionNode* Clone() const override;
    void Accept(BoundNodeVisitor& visitor) override;
private:
    FunctionSymbol* function;
};

class BoundMethodNode : public BoundExpressionNode
{
public:
    BoundMethodNode(const soul::ast::Span& span_, BoundExpressionNode* subject_, SubroutineSymbol* method_, const std::vector<TypeSymbol*>& argumentTypes_, Node* node_);
    BoundMethodNode(const soul::ast::Span& span_, BoundExpressionNode* subject_, SubroutineSymbol* method_, TypeSymbol* type_);
    BoundExpressionNode* Subject() const { return subject.get(); }
    BoundExpressionNode* ReleaseSubject() { return subject.release(); }
    SubroutineSymbol* Method() const { return method; }
    BoundExpressionNode* Clone() const override;
    void Accept(BoundNodeVisitor& visitor) override;
private:
    std::unique_ptr<BoundExpressionNode> subject;
    SubroutineSymbol* method;
};

class BoundSubroutineCallNode : public BoundExpressionNode
{
public:
    BoundSubroutineCallNode(const soul::ast::Span& span_, SubroutineSymbol* subroutine_, TypeSymbol* type_);
    SubroutineSymbol* Subroutine() const { return subroutine; }
    const std::vector<std::unique_ptr<BoundExpressionNode>>& Arguments() const { return arguments; }
    void AddArgument(BoundExpressionNode* argument);
    void Load(Emitter* emitter) override;
    BoundExpressionNode* Clone() const override;
    void Accept(BoundNodeVisitor& visitor) override;
private:
    SubroutineSymbol* subroutine;
    std::vector<std::unique_ptr<BoundExpressionNode>> arguments;
};

class BoundConstructorCallNode : public BoundExpressionNode
{
public:
    BoundConstructorCallNode(const soul::ast::Span& span_, ConstructorSymbol* constructor_);
    ConstructorSymbol* Constructor() const { return constructor; }
    void AddArgument(BoundExpressionNode* argument);
    const std::vector<std::unique_ptr<BoundExpressionNode>>& Arguments() const { return arguments; }
    void Load(Emitter* emitter) override;
    BoundExpressionNode* Clone() const override;
    void Accept(BoundNodeVisitor& visitor) override;
private:
    ConstructorSymbol* constructor;
    std::vector<std::unique_ptr<BoundExpressionNode>> arguments;
};

class BoundIndexExprNode : public BoundExpressionNode
{
public:
    BoundIndexExprNode(const soul::ast::Span& span_, BoundExpressionNode* subject_, BoundExpressionNode* index_);
    BoundExpressionNode* Subject() const { return subject.get(); }
    BoundExpressionNode* Index() const { return index.get(); }
    void Load(Emitter* emitter) override;
    void Store(Emitter* emitter) override;
    BoundExpressionNode* Clone() const override;
    void Accept(BoundNodeVisitor& visitor) override;
private:
    std::unique_ptr<BoundExpressionNode> subject;
    std::unique_ptr<BoundExpressionNode> index;
};

class BoundMemberExprNode : public BoundExpressionNode
{
public:
    BoundMemberExprNode(const soul::ast::Span& span_, BoundExpressionNode* subject_, int32_t fieldIndex_, TypeSymbol* fieldType_);
    BoundExpressionNode* Subject() const { return subject.get(); }
    int32_t FieldIndex() const { return fieldIndex; }
    void Load(Emitter* emitter) override;
    void Store(Emitter* emitter) override;
    BoundExpressionNode* Clone() const override;
    void Accept(BoundNodeVisitor& visitor) override;
private:
    std::unique_ptr<BoundExpressionNode> subject;
    int32_t fieldIndex;
};

class BoundNewExpressionNode : public BoundExpressionNode
{
public:
    BoundNewExpressionNode(const soul::ast::Span& span_, ObjectTypeSymbol* objectType_);
    ObjectTypeSymbol* ObjectType() const { return objectType; }
    void Load(Emitter* emitter) override;
    BoundExpressionNode* Clone() const override;
    void Accept(BoundNodeVisitor& visitor) override;
private:
    ObjectTypeSymbol* objectType;
};

class BoundNewArrayExpressionNode : public BoundExpressionNode
{
public:
    BoundNewArrayExpressionNode(const soul::ast::Span& span_, TypeSymbol* elementType_, BoundExpressionNode* arraySize_, TypeSymbol* arrayType_);
    TypeSymbol* ElementType() const { return elementType; }
    BoundExpressionNode* ArraySize() const { return arraySize.get(); }
    void Load(Emitter* emitter) override;
    BoundExpressionNode* Clone() const override;
    void Accept(BoundNodeVisitor& visitor) override;
private:
    TypeSymbol* elementType;
    std::unique_ptr<BoundExpressionNode> arraySize;
};

class BoundArrayLengthNode : public BoundExpressionNode
{
public:
    BoundArrayLengthNode(const soul::ast::Span& span_, BoundExpressionNode* subject_, TypeSymbol* type_);
    BoundExpressionNode* Subject() const { return subject.get(); }
    void Load(Emitter* emitter) override;
    BoundExpressionNode* Clone() const override;
    void Accept(BoundNodeVisitor& visitor) override;
private:
    std::unique_ptr<BoundExpressionNode> subject;
};

class BoundStringLengthNode : public BoundExpressionNode
{
public:
    BoundStringLengthNode(const soul::ast::Span& span_, BoundExpressionNode* subject_, TypeSymbol* type_);
    BoundExpressionNode* Subject() const { return subject.get(); }
    void Load(Emitter* emitter) override;
    BoundExpressionNode* Clone() const override;
    void Accept(BoundNodeVisitor& visitor) override;
private:
    std::unique_ptr<BoundExpressionNode> subject;
};

class BoundTypeNode : public BoundExpressionNode
{
public:
    BoundTypeNode(const soul::ast::Span& span_, TypeSymbol* type_);
    BoundExpressionNode* Clone() const override;
    void Accept(BoundNodeVisitor& visitor) override;
};

class BoundConversionNode : public BoundExpressionNode
{
public:
    BoundConversionNode(const soul::ast::Span& span_, FunctionSymbol* conversionFunction_, BoundExpressionNode* argument_);
    void Load(Emitter* emitter) override;
    BoundExpressionNode* Clone() const override;
    FunctionSymbol* ConversionFunction() const { return conversionFunction; }
    BoundExpressionNode* Argument() const { return argument.get(); }
    void Accept(BoundNodeVisitor& visitor) override;
private:
    FunctionSymbol* conversionFunction;
    std::unique_ptr<BoundExpressionNode> argument;
};

class BoundValueConversionNode : public BoundExpressionNode
{
public:
    BoundValueConversionNode(const soul::ast::Span& span_, BoundExpressionNode* argument_, TypeSymbol* targetType_);
    void Load(Emitter* emitter) override;
    BoundExpressionNode* Clone() const override;
    void Accept(BoundNodeVisitor& visitor) override;
private:
    std::unique_ptr<BoundExpressionNode> argument;
};

class BoundFunctionResultNode : public BoundExpressionNode
{
public:
    BoundFunctionResultNode(const soul::ast::Span& span_, VariableSymbol* functionResultVar_);
    VariableSymbol* FunctionResultVar() const { return functionResultVar; }
    void Load(Emitter* emitter) override;
    BoundExpressionNode* Clone() const override;
    void Accept(BoundNodeVisitor& visitor) override;
private:
    VariableSymbol* functionResultVar;
};

} // namespace p
