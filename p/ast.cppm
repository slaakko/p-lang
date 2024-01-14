// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.ast;

import soul.lexer;
import std.core;

export namespace p {

namespace ast {}

enum class Operator
{
    equal, notEqual, less, lessOrEqual, greater, greaterOrEqual, plus, minus, or_, xor_, mul, fractionalDivide, div, mod, and_, shl, shr, not_, 
};

enum class IdentifierKind
{
    none, constant, type, variable, object, procedure, function
};

enum class Direction
{
    to, downto
};

class Visitor;
class ParsingContext;
class Variable;
class Type;
class Constant;
class Procedure;
class Function;
class Subroutine;
class ObjectType;

class Node
{
public:
    Node(int64_t pos_);
    virtual ~Node();
    int64_t Pos() const { return pos; }
    virtual Node* Clone() const = 0;
    virtual void Accept(Visitor& visitor) = 0;
    virtual void AddNode(Node* node, soul::lexer::LexerBase<char>& lexer, int64_t pos);
    virtual bool IsBaseNode() const { return false; }
    virtual bool IsInvokeExprNode() const { return false; }
private:
    int64_t pos;
};

class BinaryExprNode : public Node
{
public:
    BinaryExprNode(Operator op_, Node* left_, Node* right_, int64_t pos_);
    Operator Op() const { return op; }
    Node* Left() const { return left.get(); }
    Node* Right() const { return right.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    Operator op;
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
};

class UnaryExprNode : public Node
{
public:
    UnaryExprNode(Operator op_, Node* operand_, int64_t pos_);
    Operator Op() const { return op; }
    Node* Operand() const { return operand.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    Operator op;
    std::unique_ptr<Node> operand;
};

class ParenthesizedExprNode : public Node
{
public:
    ParenthesizedExprNode(Node* expr_, int64_t pos_);
    Node* Expr() const { return expr.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<Node> expr;
};

class IntegerLiteralNode : public Node
{
public:
    IntegerLiteralNode(int32_t value_, int64_t pos_);
    int32_t Value() const { return value; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    int32_t value;
};

class RealLiteralNode : public Node
{
public:
    RealLiteralNode(float value_, int64_t pos_);
    float Value() const { return value; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    float value;
};

class BooleanLiteralNode : public Node
{
public:
    BooleanLiteralNode(bool value_, int64_t pos_);
    bool Value() const { return value; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    bool value;
};

class StringLiteralNode : public Node
{
public:
    StringLiteralNode(int64_t pos_);
    void Append(const std::string& str);
    const std::string& Value() const { return value; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::string value;
};

class CharLiteralNode : public Node
{
public:
    CharLiteralNode(char chr_, int64_t pos_);
    char Chr() const { return chr; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    char chr;
};

class IdentifierNode : public Node
{
public:
    IdentifierNode(const std::string& str_, IdentifierKind kind_, int64_t pos_);
    const std::string& Str() const { return str; }
    IdentifierKind Kind() const { return kind; }
    void SetVariable(Variable* variable_) { variable = variable_; }
    Variable* GetVariable() const { return variable; }
    void SetType(Type* type_) { type = type_; }
    Type* GetType() const { return type; }
    Constant* GetConstant() const { return constant; }
    void SetConstant(Constant* constant_) { constant = constant_; }
    Procedure* GetProcedure() const { return procedure; }
    void SetProcedure(Procedure* procedure_) { procedure = procedure_; }
    Function* GetFunction() const { return function; }
    void SetFunction(Function* function_) { function = function_; }
    bool IsConstantIdentifier() const { return kind == IdentifierKind::constant; }
    bool IsVariableIdentifier() const { return kind == IdentifierKind::variable; }
    bool IsTypeIdentifier() const { return kind == IdentifierKind::type; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::string str;
    IdentifierKind kind;
    Variable* variable;
    Type* type;
    Constant* constant;
    Procedure* procedure;
    Function* function;
};

class QualifiedIdNode : public Node
{
public:
    QualifiedIdNode(IdentifierNode* objectTypeId_, IdentifierNode* methodId_, int64_t pos_);
    IdentifierNode* ObjectTypeId() const { return objectTypeId.get(); }
    IdentifierNode* MethodId() const { return methodId.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<IdentifierNode> objectTypeId;
    std::unique_ptr<IdentifierNode> methodId;
};

class FieldNode : public Node
{
public:
    FieldNode(ObjectType* objectType_, int32_t fieldIndex_, Type* type_, int64_t pos_);
    ObjectType* GetObjectType() const { return objectType; }
    int32_t FieldIndex() const { return fieldIndex; }
    Type* GetType() const { return type; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    ObjectType* objectType;
    int32_t fieldIndex;
    Type* type;
};

class MethodNode : public Node
{
public:
    MethodNode(ObjectType* objectType_, Subroutine* method_, int64_t pos_);
    ObjectType* GetObjectType() const { return objectType; }
    Subroutine* GetMethod() const { return method; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    ObjectType* objectType;
    Subroutine* method;
};

class ThisNode : public Node
{
public:
    ThisNode(int64_t pos_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class BaseNode : public Node
{
public:
    BaseNode(int64_t pos_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    bool IsBaseNode() const override { return true; }
};

class NilNode : public Node
{
public:
    NilNode(int64_t pos_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class NewExprNode : public Node
{
public:
    NewExprNode(const std::string& typeName_, int64_t pos_);
    const std::string& TypeName() const { return typeName; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::string typeName;
};

class NewArrayExprNode : public Node
{
public:
    NewArrayExprNode(const std::string& typeName_, Node* arraySize_, int64_t pos_);
    const std::string& TypeName() const { return typeName; }
    Node* ArraySize() const { return arraySize.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::string typeName;
    std::unique_ptr<Node> arraySize;
};

class ValueTypecastNode : public Node
{
public:
    ValueTypecastNode(const std::string& typeName_, Node* expression_, int64_t pos_);
    const std::string& TypeName() const { return typeName; }
    Node* Expression() const { return expression.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::string typeName;
    std::unique_ptr<Node> expression;
};

class VariableTypecastNode : public Node
{
public:
    VariableTypecastNode(const std::string& typeName_, Node* variableReference_, int64_t pos_);
    const std::string& TypeName() const { return typeName; }
    Node* VariableReference() const { return variableReference.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::string typeName;
    std::unique_ptr<Node> variableReference;
};

class InvokeExprNode : public Node
{
public:
    InvokeExprNode(Node* subject_, int64_t pos_);
    void AddNode(Node* node, soul::lexer::LexerBase<char>& lexer, int64_t pos) override;
    Node* Subject() const { return subject.get(); }
    const std::vector<std::unique_ptr<Node>>& Arguments() const { return arguments; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    bool IsInvokeExprNode() const override { return true; }
private:
    std::unique_ptr<Node> subject;
    std::vector<std::unique_ptr<Node>> arguments;
};

class IndexExprNode : public Node
{
public:
    IndexExprNode(Node* subject_, int64_t pos_);
    Node* Subject() const { return subject.get(); }
    void SetIndex(Node* index_);
    Node* Index() const { return index.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<Node> subject;
    std::unique_ptr<Node> index;
};

class DotNode : public Node
{
public:
    DotNode(Node* subject_, int64_t pos_);
    Node* Subject() const { return subject.get(); }
    void SetId(IdentifierNode* id_);
    IdentifierNode* Id() const { return id.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<Node> subject;
    std::unique_ptr<IdentifierNode> id;
};

class StatementNode : public Node
{
public:
    StatementNode(int64_t pos_);
};

class AssignmentStatementNode : public StatementNode
{
public:
    AssignmentStatementNode(Node* target_, Node* source_, int64_t pos_);
    Node* Target() const { return target.get(); }
    Node* Source() const { return source.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<Node> target;
    std::unique_ptr<Node> source;
};

class ProcedureCallStatementNode : public StatementNode
{
public:
    ProcedureCallStatementNode(IdentifierNode* procedureId_, int64_t pos_);
    IdentifierNode* ProcedureId() const { return procedureId.get(); }
    void AddNode(Node* node, soul::lexer::LexerBase<char>& lexer, int64_t pos) override;
    const std::vector<std::unique_ptr<Node>>& Arguments() const { return arguments; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<IdentifierNode> procedureId;
    std::vector<std::unique_ptr<Node>> arguments;
};

class ExpressionStatementNode : public StatementNode
{
public:
    ExpressionStatementNode(Node* expr_, int64_t pos_);
    Node* Expr() const { return expr.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<Node> expr;
};

class EmptyStatementNode : public StatementNode
{
public:
    EmptyStatementNode(int64_t pos_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class CompoundStatementNode : public StatementNode
{
public:
    CompoundStatementNode(int64_t pos_);
    void AddStatement(StatementNode* statement);
    const std::vector<std::unique_ptr<StatementNode>>& Statements() const { return statements; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::vector<std::unique_ptr<StatementNode>> statements;
};

class IfStatementNode : public StatementNode
{
public:
    IfStatementNode(Node* condition_, StatementNode* thenStatement_, int64_t pos_);
    Node* Condition() const { return condition.get(); }
    StatementNode* ThenStatement() const { return thenStatement.get(); }
    StatementNode* ElseStatement() const { return elseStatement.get(); }
    void SetElseStatement(StatementNode* elseStatement_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<Node> condition;
    std::unique_ptr<StatementNode> thenStatement;
    std::unique_ptr<StatementNode> elseStatement;
};

class CaseNode : public Node
{
public:
    CaseNode(int64_t pos_);
    void AddRange(Node* range);
    const std::vector<std::unique_ptr<Node>>& Ranges() const { return ranges; }
    void SetStatement(StatementNode* statement_);
    StatementNode* Statement() const { return statement.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::vector<std::unique_ptr<Node>> ranges;
    std::unique_ptr<StatementNode> statement;
};

class ConstantRangeNode : public Node
{
public:
    ConstantRangeNode(Node* start_, Node* end_, int64_t pos_);
    Node* RangeStart() const { return rangeStart.get(); }
    Node* RangeEnd() const { return rangeEnd.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<Node> rangeStart;
    std::unique_ptr<Node> rangeEnd;
};

class CaseStatementNode : public StatementNode
{
public:
    CaseStatementNode(Node* condition_, int64_t pos_);
    Node* Condition() const { return condition.get(); }
    void AddCase(CaseNode* caseNode);
    const std::vector<std::unique_ptr<CaseNode>>& Cases() const { return cases; }
    void SetElsePart(StatementNode* elsePart_);
    StatementNode* ElsePart() const { return elsePart.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<Node> condition;
    std::vector<std::unique_ptr<CaseNode>> cases;
    std::unique_ptr<StatementNode> elsePart;
};

class RepeatStatementNode : public StatementNode
{
public:
    RepeatStatementNode(int64_t pos_);
    void AddStatement(StatementNode* statement);
    const std::vector<std::unique_ptr<StatementNode>>& Statements() const { return statements; }
    void SetCondition(Node* condition_);
    Node* Condition() const { return condition.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::vector<std::unique_ptr<StatementNode>> statements;
    std::unique_ptr<Node> condition;
};

class WhileStatementNode : public StatementNode
{
public:
    WhileStatementNode(Node* condition_, StatementNode* statement_, int64_t pos_);
    Node* Condition() const { return condition.get(); }
    StatementNode* Statement() const { return statement.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<Node> condition;
    std::unique_ptr<StatementNode> statement;
};

class ForStatementNode : public StatementNode
{
public:
    ForStatementNode(IdentifierNode* controlVariable_, Node* initialValue_, Direction direction_, Node* finalValue_, StatementNode* statement_, int64_t pos_);
    IdentifierNode* ControlVariable() const { return controlVariable.get(); }
    Node* InitialValue() const { return initialValue.get(); }
    Direction Dir() const { return direction; }
    Node* FinalValue() const { return finalValue.get(); }
    StatementNode* Statement() const { return statement.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<IdentifierNode> controlVariable;
    std::unique_ptr<Node> initialValue;
    Direction direction;
    std::unique_ptr<Node> finalValue;
    std::unique_ptr<StatementNode> statement;
};

IdentifierNode* MakeVariableId(ParsingContext* context, const std::string& variableName, int64_t pos);
FieldNode* MakeField(ParsingContext* context, const std::string& fieldName, int64_t pos);
MethodNode* MakeMethod(ParsingContext* context, const std::string& methodName, int64_t pos);
IdentifierNode* MakeTypeId(ParsingContext* context, const std::string& typeName, int64_t pos);
IdentifierNode* MakeConstantId(ParsingContext* context, const std::string& constantName, int64_t pos);
IdentifierNode* MakeProcedureId(ParsingContext* context, const std::string& procedureName, int64_t pos);
IdentifierNode* MakeFunctionId(ParsingContext* context, const std::string& functionName, int64_t pos);
Node* MakeCharacterStringConstant(StringLiteralNode* node, int64_t pos);

void ThrowError(const std::string& message, soul::lexer::LexerBase<char>& lexer, int64_t pos);

IntegerLiteralNode* ParseIntegerLiteral(soul::lexer::LexerBase<char>& lexer, const soul::lexer::Token<char, soul::lexer::LexerBase<char>>& token, int64_t pos);
RealLiteralNode* ParseRealLiteral(soul::lexer::LexerBase<char>& lexer, const soul::lexer::Token<char, soul::lexer::LexerBase<char>>& token, int64_t pos);
std::string ParseQuotedString(soul::lexer::LexerBase<char>& lexer, const soul::lexer::Token<char, soul::lexer::LexerBase<char>>& token, int64_t pos);
std::string ParseControlString(soul::lexer::LexerBase<char>& lexer, const soul::lexer::Token<char, soul::lexer::LexerBase<char>>& token, int64_t pos);

} // namespace p
