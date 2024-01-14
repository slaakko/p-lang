// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.binder;

import p.value;
import soul.lexer;
import std.core;

export namespace p {

class ParsingContext;
class CompoundStatementNode;
class Type;
class ArrayType;
class Parameter;
class Variable;
class Constant;
class Emitter;
class Procedure;
class Constructor;
class Subroutine;
class BoundNodeVisitor;

enum class BoundNodeKind
{
    boundCompoundStatementNode, boundEmptyStatementNode, boundAssignmentStatementNode, boundProcedureCallStatementNode, boundExpressionStatementNode, 
    boundIfStatementNode, boundWhileStatementNode, boundRepeatStatementNode, boundRangeNode, boundCaseNode, boundCaseStatementNode, boundReturnFunctionResultStatementNode,
    boundBinaryExprNode, boundUnaryExprNode, boundLiteralNode, boundParameterNode, boundVariableNode, boundFunctionResultNode, boundConstantNode, 
    boundConversionNode, boundValueConversionNode, boundVariableTypecastNode,
    boundProcedureNode, boundFunctionNode, boundMethodNode, boundMemberExprNode, 
    boundFunctionCallNode, boundConstructorCallNode, boundMethodCallNode, boundNewExpressionNode, boundNewArrayExpressionNode, boundIndexExprNode, 
    boundArrayLengthNode, boundStringLengthNode
};

class BoundNode
{
public:
    BoundNode(BoundNodeKind kind_, int64_t pos_);
    BoundNodeKind Kind() const { return kind; }
    int64_t Pos() const { return pos; }
    bool IsBoundCompoundStatementNode() const { return kind == BoundNodeKind::boundCompoundStatementNode; }
    bool IsBoundProcedureNode() const { return kind == BoundNodeKind::boundProcedureNode; }
    bool IsBoundFunctionNode() const { return kind == BoundNodeKind::boundFunctionNode; }
    bool IsBoundMethodNode() const { return kind == BoundNodeKind::boundMethodNode; }
    bool IsBoundMethodCallNode() const { return kind == BoundNodeKind::boundMethodCallNode; }
    bool IsBoundNewExpressionNode() const { return kind == BoundNodeKind::boundNewExpressionNode;  }
    virtual void Accept(BoundNodeVisitor& visitor) = 0;
private:
    BoundNodeKind kind;
    int64_t pos;
};

enum class ArgumentFlags
{
    none = 0, varArgument = 1 << 0, constantArgument = 1 << 1, thisOrBaseArgument = 1 << 2, virtualCall = 1 << 3
};

constexpr ArgumentFlags operator|(ArgumentFlags left, ArgumentFlags right)
{
    return ArgumentFlags(int(left) | int(right));
}

constexpr ArgumentFlags operator&(ArgumentFlags left, ArgumentFlags right)
{
    return ArgumentFlags(int(left) & int(right));
}

constexpr ArgumentFlags operator~(ArgumentFlags argFlags)
{
    return ArgumentFlags(~argFlags);
}

class BoundExpressionNode : public BoundNode
{
public:
    BoundExpressionNode(BoundNodeKind kind_, int64_t pos_, Type* type_);
    Type* GetType() const { return type; }
    void SetType(Type* type_) { type = type_; }
    virtual void Load(Emitter* emitter);
    virtual void Store(Emitter* emitter);
    virtual BoundExpressionNode* Clone() const = 0;
    virtual bool IsConst() const { return false; }
    void SetArgumentFlags(ArgumentFlags argumentFlags_) { argumentFlags = argumentFlags_; }
    ArgumentFlags GetArgumentFlags() const { return argumentFlags; }
private:
    Type* type;
    ArgumentFlags argumentFlags;
};

class BoundBinaryExpressionNode : public BoundExpressionNode
{
public:
    BoundBinaryExpressionNode(Function* operatorFunction_, BoundExpressionNode* left_, BoundExpressionNode* right_, int64_t pos_);
    void Load(Emitter* emitter) override;
    void Accept(BoundNodeVisitor& visitor) override;
    BoundExpressionNode* Clone() const override;
private:
    Function* operatorFunction;
    std::unique_ptr<BoundExpressionNode> left;
    std::unique_ptr<BoundExpressionNode> right;
};

class BoundUnaryExpressionNode : public BoundExpressionNode
{
public:
    BoundUnaryExpressionNode(Function* operatorFunction_, BoundExpressionNode* operand_, int64_t pos_);
    void Load(Emitter* emitter) override;
    void Accept(BoundNodeVisitor& visitor) override;
    BoundExpressionNode* Clone() const override;
private:
    Function* operatorFunction;
    std::unique_ptr<BoundExpressionNode> operand;
};

class BoundLiteralNode : public BoundExpressionNode
{
public:
    BoundLiteralNode(int64_t pos_, Type* type_, Value* value_);
    Value* GetValue() const { return value.get(); }
    void Load(Emitter* emitter) override;
    void Accept(BoundNodeVisitor& visitor) override;
    BoundExpressionNode* Clone() const override;
private:
    std::unique_ptr<Value> value;
};

class BoundParameterNode : public BoundExpressionNode
{
public:
    BoundParameterNode(int64_t pos_, Parameter* parameter_);
    BoundParameterNode(int64_t pos_, Parameter* parameter_, Type* type_);
    Parameter* GetParameter() const { return parameter; }
    void Load(Emitter* emitter) override;
    void Store(Emitter* emitter) override;
    void Accept(BoundNodeVisitor& visitor) override;
    BoundExpressionNode* Clone() const override;
    bool IsConst() const override;
private:
    Parameter* parameter;
};

class BoundVariableNode : public BoundExpressionNode
{
public:
    BoundVariableNode(int64_t pos_, Variable* variable_);
    Variable* GetVariable() const { return variable; }
    void Load(Emitter* emitter) override;
    void Store(Emitter* emitter) override;
    void Accept(BoundNodeVisitor& visitor) override;
    BoundExpressionNode* Clone() const override;
    bool IsConst() const override;
private:
    Variable* variable;
};

class BoundFunctionResultNode : public BoundExpressionNode
{
public:
    BoundFunctionResultNode(int64_t pos_, Variable* variable_);
    Variable* GetVariable() const { return variable; }
    void Load(Emitter* emitter) override;
    void Accept(BoundNodeVisitor& visitor) override;
    BoundExpressionNode* Clone() const override;
private:
    Variable* variable;
};

class BoundConstantNode : public BoundExpressionNode
{
public:
    BoundConstantNode(int64_t pos_, Type* type_, Constant* constant_);
    void Load(Emitter* emitter) override;
    void Accept(BoundNodeVisitor& visitor) override;
    BoundExpressionNode* Clone() const override;
private:
    Constant* constant;
};

class BoundConversionNode : public BoundExpressionNode
{
public:
    BoundConversionNode(Function* conversionFunction_, BoundExpressionNode* operand_, int64_t pos_);
    void Load(Emitter* emitter) override;
    void Accept(BoundNodeVisitor& visitor) override;
    BoundExpressionNode* Clone() const override;
private:
    Function* conversionFunction;
    std::unique_ptr<BoundExpressionNode> operand;
};

class BoundValueConversionNode : public BoundExpressionNode
{
public:
    BoundValueConversionNode(BoundExpressionNode* operand_, Type* type_, int64_t pos_);
    void Load(Emitter* emitter) override;
    void Accept(BoundNodeVisitor& visitor) override;
    BoundExpressionNode* Clone() const override;
private:
    std::unique_ptr<BoundExpressionNode> operand;
};

class BoundVariableTypecastNode : public BoundExpressionNode
{
public:
    BoundVariableTypecastNode(BoundExpressionNode* var_, Type* type_, int64_t pos_);
    void Load(Emitter* emitter) override;
    void Accept(BoundNodeVisitor& visitor) override;
    BoundExpressionNode* Clone() const override;
private:
    std::unique_ptr<BoundExpressionNode> var;
};

class BoundProcedureNode : public BoundExpressionNode
{
public:
    BoundProcedureNode(Procedure* procedure_, int64_t pos_);
    Procedure* GetProcedure() const { return procedure; }
    void Accept(BoundNodeVisitor& visitor) override;
    BoundExpressionNode* Clone() const override;
private:
    Procedure* procedure;
};

class BoundFunctionNode : public BoundExpressionNode
{
public:
    BoundFunctionNode(Function* function_, const std::vector<std::unique_ptr<BoundExpressionNode>>& boundArguments, int64_t pos_);
    Function* GetFunction() const { return function; }
    void Accept(BoundNodeVisitor& visitor) override;
    BoundExpressionNode* Clone() const override;
private:
    Function* function;
};

class BoundMethodNode : public BoundExpressionNode
{
public:
    BoundMethodNode(BoundExpressionNode* subject_, Subroutine* method_, int64_t pos_);
    BoundExpressionNode* Subject() const { return subject.get(); }
    BoundExpressionNode* ReleaseSubject() { return subject.release(); }
    Subroutine* GetMethod() const { return method; }
    void Accept(BoundNodeVisitor& visitor) override;
    BoundExpressionNode* Clone() const override;
private:
    std::unique_ptr<BoundExpressionNode> subject;
    Subroutine* method;
};

class BoundFunctionCallNode : public BoundExpressionNode
{
public:
    BoundFunctionCallNode(Function* function_, const std::vector<std::unique_ptr<BoundExpressionNode>>& boundArguments, int64_t pos_);
    Function* GetFunction() const { return function; }
    void AddArgument(BoundExpressionNode* argument);
    const std::vector<std::unique_ptr<BoundExpressionNode>>& Arguments() const { return arguments; }
    void Load(Emitter* emitter) override;
    void Accept(BoundNodeVisitor& visitor) override;
    BoundExpressionNode* Clone() const override;
private:
    Function* function;
    std::vector<std::unique_ptr<BoundExpressionNode>> arguments;
};

class BoundConstructorCallNode : public BoundExpressionNode
{
public:
    BoundConstructorCallNode(Constructor* constructor_, int64_t pos_);
    void AddArgument(BoundExpressionNode* argument);
    const std::vector<std::unique_ptr<BoundExpressionNode>>& Arguments() const { return arguments; }
    void Load(Emitter* emitter) override;
    void Accept(BoundNodeVisitor& visitor) override;
    BoundExpressionNode* Clone() const override;
private:
    Constructor* constructor;
    std::vector<std::unique_ptr<BoundExpressionNode>> arguments;
};

class BoundMethodCallNode : public BoundExpressionNode
{
public:
    BoundMethodCallNode(Subroutine* method_, const std::vector<std::unique_ptr<BoundExpressionNode>>& boundArguments, int64_t pos_);
    Subroutine* GetMethod() const { return method; }
    void AddArgument(BoundExpressionNode* argument);
    const std::vector<std::unique_ptr<BoundExpressionNode>>& Arguments() const { return arguments; }
    void Load(Emitter* emitter) override;
    void Accept(BoundNodeVisitor& visitor) override;
    BoundExpressionNode* Clone() const override; 
private:
    Subroutine* method;
    std::vector<std::unique_ptr<BoundExpressionNode>> arguments;
};

class BoundNewExpressionNode : public BoundExpressionNode
{
public:
    BoundNewExpressionNode(ObjectType* objectType_, int64_t pos_);
    ObjectType* GetObjectType() const { return objectType; }
    void Load(Emitter* emitter) override;
    void Accept(BoundNodeVisitor& visitor) override;
    BoundExpressionNode* Clone() const override;
private:
    ObjectType* objectType;
};

class BoundNewArrayExpressionNode : public BoundExpressionNode
{
public:
    BoundNewArrayExpressionNode(Type* elementType_, BoundExpressionNode* arraySize_, ArrayType* arrayType_, int64_t pos_);
    Type* ElementType() const { return elementType; }
    BoundExpressionNode* ArraySize() const { return arraySize.get(); }
    void Load(Emitter* emitter) override;
    void Accept(BoundNodeVisitor& visitor) override;
    BoundExpressionNode* Clone() const override;
private:
    Type* elementType;
    std::unique_ptr<BoundExpressionNode> arraySize;
};

class BoundMemberExprNode : public BoundExpressionNode
{
public:
    BoundMemberExprNode(BoundExpressionNode* subject_, int32_t fieldIndex_, int64_t pos_, Type* type_);
    BoundExpressionNode* Subject() const { return subject.get(); }
    int32_t FieldIndex() const { return fieldIndex; }
    void Load(Emitter* emitter) override;
    void Store(Emitter* emitter) override;
    void Accept(BoundNodeVisitor& visitor) override;
    BoundExpressionNode* Clone() const override;
private:
    std::unique_ptr<BoundExpressionNode> subject;
    int32_t fieldIndex;
};

class BoundIndexExprNode : public BoundExpressionNode
{
public:
    BoundIndexExprNode(BoundExpressionNode* subject_, BoundExpressionNode* index_, int64_t pos_);
    void Load(Emitter* emitter) override;
    void Store(Emitter* emitter) override;
    void Accept(BoundNodeVisitor& visitor) override;
    BoundExpressionNode* Clone() const override;
private:
    std::unique_ptr<BoundExpressionNode> subject;
    std::unique_ptr<BoundExpressionNode> index;
};

class BoundArrayLengthNode : public BoundExpressionNode
{
public:
    BoundArrayLengthNode(BoundExpressionNode* subject_, int64_t pos_, Type* integerType_);
    void Load(Emitter* emitter) override;
    void Accept(BoundNodeVisitor& visitor) override;
    BoundExpressionNode* Clone() const override;
private:
    std::unique_ptr<BoundExpressionNode> subject;
};

class BoundStringLengthNode : public BoundExpressionNode
{
public:
    BoundStringLengthNode(BoundExpressionNode* subject_, int64_t pos_, Type* integerType_);
    void Load(Emitter* emitter) override;
    void Accept(BoundNodeVisitor& visitor) override;
    BoundExpressionNode* Clone() const override;
private:
    std::unique_ptr<BoundExpressionNode> subject;
};

class BoundStatementNode : public BoundNode
{
public:
    BoundStatementNode(BoundNodeKind kind_, int64_t pos_);
};

class BoundEmptyStatementNode : public BoundStatementNode
{
public:
    BoundEmptyStatementNode(int64_t pos_);
    void Accept(BoundNodeVisitor& visitor) override;
};

class BoundCompoundStatementNode : public BoundStatementNode
{
public:
    BoundCompoundStatementNode(int64_t pos_);
    void AddStatement(BoundStatementNode* boundStatement);
    const std::vector<std::unique_ptr<BoundStatementNode>>& BoundStatements() const { return boundStatements; }
    void Accept(BoundNodeVisitor& visitor) override;
private:
    std::vector<std::unique_ptr<BoundStatementNode>> boundStatements;
};

class BoundAssignmentStatementNode : public BoundStatementNode
{
public:
    BoundAssignmentStatementNode(BoundExpressionNode* target_, BoundExpressionNode* source_, int64_t pos_);
    BoundExpressionNode* Target() const { return target.get(); }
    BoundExpressionNode* Source() const { return source.get(); }
    void Accept(BoundNodeVisitor& visitor) override;
private:
    std::unique_ptr<BoundExpressionNode> target;
    std::unique_ptr<BoundExpressionNode> source;
};

class BoundProcedureCallStatementNode : public BoundStatementNode
{
public:
    BoundProcedureCallStatementNode(Procedure* procedure_, int64_t pos_);
    Procedure* GetProcedure() const { return procedure; }
    void AddArgument(BoundExpressionNode* argument);
    const std::vector<std::unique_ptr<BoundExpressionNode>>& Arguments() const { return arguments; }
    void Accept(BoundNodeVisitor& visitor) override;
private:
    Procedure* procedure;
    std::vector<std::unique_ptr<BoundExpressionNode>> arguments;
};

class BoundExpressionStatementNode : public BoundStatementNode
{
public:
    BoundExpressionStatementNode(BoundExpressionNode* boundExpression_, int64_t pos_);
    BoundExpressionNode* BoundExpression() const { return boundExpression.get(); }
    void Accept(BoundNodeVisitor& visitor) override;
    bool Pop() const { return pop; }
    void DontPop() { pop = false; }
private:
    std::unique_ptr<BoundExpressionNode> boundExpression;
    bool pop;
};

class BoundIfStatementNode : public BoundStatementNode
{
public:
    BoundIfStatementNode(BoundExpressionNode* condition_, BoundStatementNode* thenStatement_, BoundStatementNode* elseStatement_, int64_t pos_);
    BoundExpressionNode* Condition() const { return condition.get(); }
    BoundStatementNode* ThenStatement() const { return thenStatement.get(); }
    BoundStatementNode* ElseStatement() const { return elseStatement.get(); }
    void Accept(BoundNodeVisitor& visitor) override;
private:
    std::unique_ptr<BoundExpressionNode> condition;
    std::unique_ptr<BoundStatementNode> thenStatement;
    std::unique_ptr<BoundStatementNode> elseStatement;
};

class BoundWhileStatementNode : public BoundStatementNode
{
public:
    BoundWhileStatementNode(BoundExpressionNode* condition_, BoundStatementNode* statement_, int64_t pos_);
    BoundExpressionNode* Condition() const { return condition.get(); }
    BoundStatementNode* Statement() const { return statement.get(); }
    void Accept(BoundNodeVisitor& visitor) override;
private:
    std::unique_ptr<BoundExpressionNode> condition;
    std::unique_ptr<BoundStatementNode> statement;
};

class BoundRepeatStatementNode : public BoundStatementNode
{
public:
    BoundRepeatStatementNode(int64_t pos_);
    void AddStatement(BoundStatementNode* boundStatement);
    const std::vector<std::unique_ptr<BoundStatementNode>>& Statements() const { return statements; }
    BoundExpressionNode* Condition() const { return condition.get(); }
    void SetCondition(BoundExpressionNode* condition_);
    void Accept(BoundNodeVisitor& visitor) override;
private:
    std::vector<std::unique_ptr<BoundStatementNode>> statements;
    std::unique_ptr<BoundExpressionNode> condition;
};

class BoundRangeNode : public BoundNode
{
public:
    BoundRangeNode(int64_t pos_, Value* rangeStart_, Value* rangeEnd_);
    void Accept(BoundNodeVisitor& visitor) override;
    Value* RangeStart() const { return rangeStart.get(); }
    Value* RangeEnd() const { return rangeEnd.get(); }
private:
    std::unique_ptr<Value> rangeStart;
    std::unique_ptr<Value> rangeEnd;
};

class BoundCaseNode : public BoundNode
{
public:
    BoundCaseNode(int64_t pos_);
    BoundStatementNode* Statement() const { return statement.get(); }
    void SetStatement(BoundStatementNode* boundStatement);
    const std::vector<std::unique_ptr<BoundRangeNode>>& Ranges() const { return ranges; }
    void AddRange(BoundRangeNode* range);
    void Accept(BoundNodeVisitor& visitor) override;
private:
    std::vector<std::unique_ptr<BoundRangeNode>> ranges;
    std::unique_ptr<BoundStatementNode> statement;
};

class BoundCaseStatementNode : public BoundStatementNode
{
public:
    BoundCaseStatementNode(int64_t pos_);
    BoundExpressionNode* Condition() const { return condition.get(); }
    void SetCondition(BoundExpressionNode* condition_);
    void Accept(BoundNodeVisitor& visitor) override;
    void AddCase(BoundCaseNode* caseNode);
    const std::vector<std::unique_ptr<BoundCaseNode>>& Cases() const { return cases; }
    BoundStatementNode* ElsePart() const { return elsePart.get(); }
    void SetElsePart(BoundStatementNode* elsePart_);
private:
    std::unique_ptr<BoundExpressionNode> condition;
    std::vector<std::unique_ptr<BoundCaseNode>> cases;
    std::unique_ptr<BoundStatementNode> elsePart;
};

class BoundReturnFunctionResultStatementNode : public BoundStatementNode
{
public:
    BoundReturnFunctionResultStatementNode(BoundFunctionResultNode* functionResult_, int64_t pos_);
    void Accept(BoundNodeVisitor& visitor) override;
    BoundFunctionResultNode* FunctionResult() const { return functionResult.get(); }
private:
    std::unique_ptr<BoundFunctionResultNode> functionResult;
};

class BoundNodeVisitor
{
public:
    BoundNodeVisitor();
    virtual ~BoundNodeVisitor();
    virtual void Visit(BoundBinaryExpressionNode& node) {}
    virtual void Visit(BoundUnaryExpressionNode& node) {}
    virtual void Visit(BoundLiteralNode& node) {}
    virtual void Visit(BoundParameterNode& node) {}
    virtual void Visit(BoundVariableNode& node) {}
    virtual void Visit(BoundFunctionResultNode& node) {}
    virtual void Visit(BoundConstantNode& node) {}
    virtual void Visit(BoundConversionNode& node) {}
    virtual void Visit(BoundValueConversionNode& node) {}
    virtual void Visit(BoundVariableTypecastNode& node) {}
    virtual void Visit(BoundProcedureNode& node) {}
    virtual void Visit(BoundFunctionNode& node) {}
    virtual void Visit(BoundMethodNode& node) {}
    virtual void Visit(BoundFunctionCallNode& node) {}
    virtual void Visit(BoundMethodCallNode& node) {}
    virtual void Visit(BoundNewExpressionNode& node) {}
    virtual void Visit(BoundNewArrayExpressionNode& node) {}
    virtual void Visit(BoundMemberExprNode& node) {}
    virtual void Visit(BoundIndexExprNode& node) {}
    virtual void Visit(BoundArrayLengthNode& node) {}
    virtual void Visit(BoundStringLengthNode& node) {}
    virtual void Visit(BoundConstructorCallNode& node) {}
    virtual void Visit(BoundEmptyStatementNode& node) {}
    virtual void Visit(BoundCompoundStatementNode& node) {}
    virtual void Visit(BoundAssignmentStatementNode& node) {}
    virtual void Visit(BoundProcedureCallStatementNode& node) {}
    virtual void Visit(BoundExpressionStatementNode& node) {}
    virtual void Visit(BoundIfStatementNode& node) {}
    virtual void Visit(BoundWhileStatementNode& node) {}
    virtual void Visit(BoundRepeatStatementNode& node) {}
    virtual void Visit(BoundRangeNode& node) {}
    virtual void Visit(BoundCaseNode& node) {}
    virtual void Visit(BoundCaseStatementNode& node) {}
    virtual void Visit(BoundReturnFunctionResultStatementNode& node) {}
};

std::unique_ptr<BoundCompoundStatementNode> Bind(ParsingContext* context, CompoundStatementNode* compoundStatement, Subroutine* subroutine, soul::lexer::LexerBase<char>& lexer);

std::unique_ptr<BoundCompoundStatementNode> Bind(ParsingContext* context, CompoundStatementNode* compoundStatement, Subroutine* subroutine, soul::lexer::LexerBase<char>& lexer,
    int level_);

} // namespace p
