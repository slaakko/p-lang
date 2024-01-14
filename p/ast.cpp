// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.ast;

import p.visitor;
import p.block;
import p.parsing_context;

namespace p {

Node::Node(int64_t pos_) : pos(pos_)
{
}

Node::~Node()
{
}

void Node::AddNode(Node* node, soul::lexer::LexerBase<char>& lexer, int64_t pos)
{
    ThrowError("error: cannot add node", lexer, pos);
}

BinaryExprNode::BinaryExprNode(Operator op_, Node* left_, Node* right_, int64_t pos_) : Node(pos_), op(op_), left(left_), right(right_)
{
}

Node* BinaryExprNode::Clone() const
{
    return new BinaryExprNode(op, left->Clone(), right->Clone(), Pos());
}

void BinaryExprNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

UnaryExprNode::UnaryExprNode(Operator op_, Node* operand_, int64_t pos_) : Node(pos_), op(op_), operand(operand_)
{
}

Node* UnaryExprNode::Clone() const
{
    return new UnaryExprNode(op, operand->Clone(), Pos());
}

void UnaryExprNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ParenthesizedExprNode::ParenthesizedExprNode(Node* expr_, int64_t pos_) : Node(pos_), expr(expr_)
{
}

Node* ParenthesizedExprNode::Clone() const
{
    return new ParenthesizedExprNode(expr->Clone(), Pos());
}

void ParenthesizedExprNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

IntegerLiteralNode::IntegerLiteralNode(int32_t value_, int64_t pos_) : Node(pos_), value(value_)
{
}

Node* IntegerLiteralNode::Clone() const
{
    return new IntegerLiteralNode(value, Pos());
}

void IntegerLiteralNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

RealLiteralNode::RealLiteralNode(float value_, int64_t pos_) : Node(pos_), value(value_)
{
}

Node* RealLiteralNode::Clone() const
{
    return new RealLiteralNode(value, Pos());
}

void RealLiteralNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

BooleanLiteralNode::BooleanLiteralNode(bool value_, int64_t pos_) : Node(pos_), value(value_)
{
}

Node* BooleanLiteralNode::Clone() const
{
    return new BooleanLiteralNode(value, Pos());
}

void BooleanLiteralNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

StringLiteralNode::StringLiteralNode(int64_t pos_) : Node(pos_), value()
{
}

void StringLiteralNode::Append(const std::string& str)
{
    value.append(str);
}

Node* StringLiteralNode::Clone() const
{
    StringLiteralNode* clone = new StringLiteralNode(Pos());
    clone->Append(value);
    return clone;
}

void StringLiteralNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

CharLiteralNode::CharLiteralNode(char chr_, int64_t pos_) : Node(pos_), chr(chr_)
{
}

Node* CharLiteralNode::Clone() const
{
    return new CharLiteralNode(Chr(), Pos());
}

void CharLiteralNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

IdentifierNode::IdentifierNode(const std::string& str_, IdentifierKind kind_, int64_t pos_) : 
    Node(pos_), str(str_), kind(kind_), variable(nullptr), type(nullptr), constant(nullptr), procedure(nullptr), function(nullptr)
{
}

Node* IdentifierNode::Clone() const
{
    IdentifierNode* clone = new IdentifierNode(str, kind, Pos());
    clone->variable = variable;
    clone->type = type;
    clone->constant = constant;
    clone->procedure = procedure;
    clone->function = function;
    return clone;
}

void IdentifierNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

QualifiedIdNode::QualifiedIdNode(IdentifierNode* objectTypeId_, IdentifierNode* methodId_, int64_t pos_) : Node(pos_), objectTypeId(objectTypeId_), methodId(methodId_)
{
}

Node* QualifiedIdNode::Clone() const
{
    IdentifierNode* clonedObjectTypeId = nullptr;
    if (objectTypeId)
    {
        clonedObjectTypeId = static_cast<IdentifierNode*>(objectTypeId->Clone());
    }
    IdentifierNode* clonedMethodId = nullptr;
    if (methodId)
    {
        clonedMethodId = static_cast<IdentifierNode*>(methodId->Clone());
    }
    return new QualifiedIdNode(clonedObjectTypeId, clonedMethodId, Pos());
}

void QualifiedIdNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

FieldNode::FieldNode(ObjectType* objectType_, int32_t fieldIndex_, Type* type_, int64_t pos_) : Node(pos_), objectType(objectType_), fieldIndex(fieldIndex_), type(type_)
{
}

Node* FieldNode::Clone() const
{
    return new FieldNode(objectType, fieldIndex, type, Pos());
}

void FieldNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

MethodNode::MethodNode(ObjectType* objectType_, Subroutine* method_, int64_t pos_) : Node(pos_), objectType(objectType_), method(method_)
{
}

Node* MethodNode::Clone() const
{
    return new MethodNode(objectType, method, Pos());
}

void MethodNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ThisNode::ThisNode(int64_t pos_) : Node(pos_)
{
}

Node* ThisNode::Clone() const
{
    return new ThisNode(Pos());
}

void ThisNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

BaseNode::BaseNode(int64_t pos_) : Node(pos_)
{
}

Node* BaseNode::Clone() const
{
    return new BaseNode(Pos());
}

void BaseNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

NilNode::NilNode(int64_t pos_) : Node(pos_)
{
}

Node* NilNode::Clone() const
{
    return new NilNode(Pos());
}

void NilNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this); 
}

NewExprNode::NewExprNode(const std::string& typeName_, int64_t pos_) : Node(pos_), typeName(typeName_)
{
}

Node* NewExprNode::Clone() const
{
    return new NewExprNode(typeName, Pos());
}

void NewExprNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

NewArrayExprNode::NewArrayExprNode(const std::string& typeName_, Node* arraySize_, int64_t pos_) : Node(pos_), typeName(typeName_), arraySize(arraySize_)
{
}

Node* NewArrayExprNode::Clone() const
{
    return new NewArrayExprNode(typeName, arraySize->Clone(), Pos());
}

void NewArrayExprNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ValueTypecastNode::ValueTypecastNode(const std::string& typeName_, Node* expression_, int64_t pos_) : Node(pos_), typeName(typeName_), expression(expression_)
{
}

Node* ValueTypecastNode::Clone() const
{
    return new ValueTypecastNode(typeName, expression->Clone(), Pos());
}

void ValueTypecastNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

VariableTypecastNode::VariableTypecastNode(const std::string& typeName_, Node* variableReference_, int64_t pos_) : 
    Node(pos_), typeName(typeName_), variableReference(variableReference_)
{
}

Node* VariableTypecastNode::Clone() const
{
    return new VariableTypecastNode(typeName, variableReference->Clone(), Pos());
}

void VariableTypecastNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

InvokeExprNode::InvokeExprNode(Node* subject_, int64_t pos_) : Node(pos_), subject(subject_)
{
}

void InvokeExprNode::AddNode(Node* node, soul::lexer::LexerBase<char>& lexer, int64_t pos)
{
    arguments.push_back(std::unique_ptr<Node>(node));
}

Node* InvokeExprNode::Clone() const
{
    InvokeExprNode* clone = new InvokeExprNode(subject->Clone(), Pos());
    for (const auto& arg : arguments)
    {
        clone->arguments.push_back(std::unique_ptr<Node>(arg->Clone()));
    }
    return clone;
}

void InvokeExprNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

IndexExprNode::IndexExprNode(Node* subject_, int64_t pos_) : Node(pos_), subject(subject_)
{
}

void IndexExprNode::SetIndex(Node* index_)
{
    index.reset(index_);
}

Node* IndexExprNode::Clone() const
{
    IndexExprNode* clone = new IndexExprNode(subject->Clone(), Pos());
    clone->SetIndex(index->Clone());
    return clone;
}

void IndexExprNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

DotNode::DotNode(Node* subject_, int64_t pos_) : Node(pos_), subject(subject_)
{
}

void DotNode::SetId(IdentifierNode* id_)
{
    id.reset(id_);
}

Node* DotNode::Clone() const
{
    DotNode* clone = new DotNode(subject->Clone(), Pos());
    clone->SetId(static_cast<IdentifierNode*>(id->Clone()));
    return clone;
}

void DotNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

StatementNode::StatementNode(int64_t pos_) : Node(pos_)
{
}

AssignmentStatementNode::AssignmentStatementNode(Node* target_, Node* source_, int64_t pos_) : StatementNode(pos_), target(target_), source(source_)
{
}

Node* AssignmentStatementNode::Clone() const
{
    return new AssignmentStatementNode(target->Clone(), source->Clone(), Pos());
}

void AssignmentStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ProcedureCallStatementNode::ProcedureCallStatementNode(IdentifierNode* procedureId_, int64_t pos_) : StatementNode(pos_), procedureId(procedureId_)
{
}

void ProcedureCallStatementNode::AddNode(Node* node, soul::lexer::LexerBase<char>& lexer, int64_t pos)
{
    arguments.push_back(std::unique_ptr<Node>(node));
}

Node* ProcedureCallStatementNode::Clone() const
{
    ProcedureCallStatementNode* clone = new ProcedureCallStatementNode(static_cast<IdentifierNode*>(procedureId->Clone()), Pos());
    for (const auto& arg : arguments)
    {
        clone->arguments.push_back(std::unique_ptr<Node>(arg->Clone()));
    }
    return clone;
}

void ProcedureCallStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ExpressionStatementNode::ExpressionStatementNode(Node* expr_, int64_t pos_) : StatementNode(pos_), expr(expr_)
{
}

Node* ExpressionStatementNode::Clone() const
{
    return new ExpressionStatementNode(expr->Clone(), Pos());
}

void ExpressionStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

EmptyStatementNode::EmptyStatementNode(int64_t pos_) : StatementNode(pos_)
{
}

Node* EmptyStatementNode::Clone() const
{
    return new EmptyStatementNode(Pos());
}

void EmptyStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

CompoundStatementNode::CompoundStatementNode(int64_t pos_) : StatementNode(pos_)
{
}

void CompoundStatementNode::AddStatement(StatementNode* statement)
{
    statements.push_back(std::unique_ptr<StatementNode>(statement));
}

Node* CompoundStatementNode::Clone() const
{
    CompoundStatementNode* clone = new CompoundStatementNode(Pos());
    for (const auto& statement : statements)
    {
        clone->AddStatement(static_cast<StatementNode*>(statement->Clone()));
    }
    return clone;
}

void CompoundStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

IfStatementNode::IfStatementNode(Node* condition_, StatementNode* thenStatement_, int64_t pos_) : StatementNode(pos_), condition(condition_), thenStatement(thenStatement_)
{
}

void IfStatementNode::SetElseStatement(StatementNode* elseStatement_)
{
    elseStatement.reset(elseStatement_);
}

Node* IfStatementNode::Clone() const
{
    IfStatementNode* clone = new IfStatementNode(condition->Clone(), static_cast<StatementNode*>(thenStatement->Clone()), Pos());
    if (elseStatement)
    {
        clone->SetElseStatement(static_cast<StatementNode*>(elseStatement->Clone()));
    }
    return clone;
}

void IfStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

CaseNode::CaseNode(int64_t pos_) : Node(pos_)
{
}

void CaseNode::AddRange(Node* range)
{
    ranges.push_back(std::unique_ptr<Node>(range));
}

void CaseNode::SetStatement(StatementNode* statement_)
{
    statement.reset(statement_);
}

Node* CaseNode::Clone() const
{
    CaseNode* clone = new CaseNode(Pos());
    for (const auto& range : ranges)
    {
        clone->AddRange(range->Clone());
    }
    clone->SetStatement(static_cast<StatementNode*>(statement->Clone()));
    return clone;
}

void CaseNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ConstantRangeNode::ConstantRangeNode(Node* start_, Node* end_, int64_t pos_) : Node(pos_), rangeStart(start_), rangeEnd(end_)
{
}

Node* ConstantRangeNode::Clone() const
{
    return new ConstantRangeNode(rangeStart->Clone(), rangeEnd->Clone(), Pos());
}

void ConstantRangeNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

CaseStatementNode::CaseStatementNode(Node* condition_, int64_t pos_) : StatementNode(pos_), condition(condition_)
{
}

void CaseStatementNode::AddCase(CaseNode* caseNode)
{
    cases.push_back(std::unique_ptr<CaseNode>(caseNode));
}

void CaseStatementNode::SetElsePart(StatementNode* elsePart_)
{
    elsePart.reset(elsePart_);
}

Node* CaseStatementNode::Clone() const
{
    CaseStatementNode* clone = new CaseStatementNode(condition->Clone(), Pos());
    for (const auto& caseNode : cases)
    {
        clone->AddCase(static_cast<CaseNode*>(caseNode->Clone()));
    }
    if (elsePart)
    {
        clone->SetElsePart(static_cast<StatementNode*>(elsePart->Clone()));
    }
    return clone;
}

void CaseStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

RepeatStatementNode::RepeatStatementNode(int64_t pos_) : StatementNode(pos_)
{
}

void RepeatStatementNode::AddStatement(StatementNode* statement)
{
    statements.push_back(std::unique_ptr<StatementNode>(statement));
}

void RepeatStatementNode::SetCondition(Node* condition_)
{
    condition.reset(condition_);
}

Node* RepeatStatementNode::Clone() const
{
    RepeatStatementNode* clone = new RepeatStatementNode(Pos());
    for (const auto& statement : statements)
    {
        clone->AddStatement(static_cast<StatementNode*>(statement->Clone()));
    }
    clone->SetCondition(condition->Clone());
    return clone;
}

void RepeatStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

WhileStatementNode::WhileStatementNode(Node* condition_, StatementNode* statement_, int64_t pos_) : StatementNode(pos_), condition(condition_), statement(statement_)
{
}

Node* WhileStatementNode::Clone() const
{
    return new WhileStatementNode(condition->Clone(), static_cast<StatementNode*>(statement->Clone()), Pos());
}

void WhileStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ForStatementNode::ForStatementNode(IdentifierNode* controlVariable_, Node* initialValue_, Direction direction_, Node* finalValue_, StatementNode* statement_, int64_t pos_) : 
    StatementNode(pos_), controlVariable(controlVariable_), initialValue(initialValue_), direction(direction_), finalValue(finalValue_), statement(statement_)
{
}

Node* ForStatementNode::Clone() const
{
    return new ForStatementNode(static_cast<IdentifierNode*>(controlVariable->Clone()), initialValue->Clone(), direction, finalValue->Clone(),
        static_cast<StatementNode*>(statement->Clone()), Pos());
}

void ForStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

IdentifierNode* MakeVariableId(ParsingContext* context, const std::string& variableName, int64_t pos)
{
    Variable* variable = context->GetBlock()->GetVariable(variableName);
    if (variable)
    {
        IdentifierNode* variableIdentifier = new IdentifierNode(variableName, IdentifierKind::variable, pos);
        variableIdentifier->SetVariable(variable);
        return variableIdentifier;
    }
    else
    {
        Subroutine* subroutine = context->GetBlock()->GetSubroutine();
        if (subroutine)
        {
            for (auto& parameter : subroutine->Parameters())
            {
                if (parameter.Name() == variableName)
                {
                    IdentifierNode* variableIdentifier = new IdentifierNode(variableName, IdentifierKind::variable, pos);
                    variableIdentifier->SetVariable(&parameter);
                    return variableIdentifier;
                }
            }
        }
        Constructor* constructor = context->GetConstructor();
        if (constructor)
        {
            for (auto& parameter : constructor->Parameters())
            {
                if (parameter.Name() == variableName)
                {
                    IdentifierNode* variableIdentifier = new IdentifierNode(variableName, IdentifierKind::variable, pos);
                    variableIdentifier->SetVariable(&parameter);
                    return variableIdentifier;
                }
            }
        }
        return nullptr;
    }
}

FieldNode* MakeField(ParsingContext* context, const std::string& fieldName, int64_t pos)
{
    Subroutine* subroutine = context->GetBlock()->GetSubroutine();
    if (subroutine)
    {
        ObjectType* objectType = subroutine->Heading()->GetObjectType();
        if (objectType)
        {
            int32_t fieldIndex = objectType->GetFieldIndex(fieldName);
            if (fieldIndex != -1)
            {
                return new FieldNode(objectType, fieldIndex, objectType->GetFieldType(fieldIndex), pos);
            }
        }
    }
    return nullptr;
}

MethodNode* MakeMethod(ParsingContext* context, const std::string& methodName, int64_t pos)
{
    Subroutine* subroutine = context->GetBlock()->GetSubroutine();
    if (subroutine)
    {
        ObjectType* objectType = subroutine->Heading()->GetObjectType();
        while (objectType)
        {
            Subroutine* method = objectType->GetMethod(methodName);
            if (method)
            {
                return new MethodNode(objectType, method, pos);
            }
            objectType = objectType->BaseType();
        }
    }
    return nullptr;
}

IdentifierNode* MakeTypeId(ParsingContext* context, const std::string& typeName, int64_t pos)
{
    Type* type = context->GetBlock()->GetType(typeName);
    if (type)
    {
        IdentifierNode* typeId = new IdentifierNode(typeName, IdentifierKind::type, pos);
        typeId->SetType(type);
        return typeId;
    }
    else
    {
        return nullptr;
    }
}

IdentifierNode* MakeConstantId(ParsingContext* context, const std::string& constantName, int64_t pos)
{
    Constant* constant = context->GetBlock()->GetConstant(constantName);
    if (constant)
    {
        IdentifierNode* constantId = new IdentifierNode(constantName, IdentifierKind::constant, pos);
        constantId->SetConstant(constant);
        return constantId;
    }
    else
    {
        return nullptr;
    }
}

IdentifierNode* MakeProcedureId(ParsingContext* context, const std::string& procedureName, int64_t pos)
{
    Procedure* procedure = context->GetBlock()->GetProcedure(procedureName);
    if (procedure)
    {
        IdentifierNode* procedureId = new IdentifierNode(procedureName, IdentifierKind::procedure, pos);
        procedureId->SetProcedure(procedure);
        return procedureId;
    }
    else
    {
        return nullptr;
    }
}

IdentifierNode* MakeFunctionId(ParsingContext* context, const std::string& functionName, int64_t pos)
{
    Function* function = context->GetBlock()->GetFunction(functionName);
    if (function)
    {
        IdentifierNode* functionId = new IdentifierNode(functionName, IdentifierKind::function, pos);
        functionId->SetFunction(function);
        return functionId;
    }
    else
    {
        return nullptr;
    }
}

Node* MakeCharacterStringConstant(StringLiteralNode* node, int64_t pos)
{
    const std::string& str = node->Value();
    if (str.length() == 1)
    {
        return new CharLiteralNode(str[0], pos);
    }
    else
    {
        StringLiteralNode* node = new StringLiteralNode(pos);
        node->Append(str);
        return node;
    }
}

void ThrowError(const std::string& message, soul::lexer::LexerBase<char>& lexer, int64_t pos)
{
    std::string msg(message);
    msg.append(":\nat ").append(lexer.FileName()).append(" line ").append(std::to_string(lexer.Line()).append(1, '\n').append(lexer.ErrorLines(pos)));
    throw std::runtime_error(msg);
}

int32_t ParseIntegerLiteral(const char*& p, const char* e, soul::lexer::LexerBase<char>& lexer, int64_t pos)
{
    int32_t value = 0;
    if (p != e)
    {
        if (*p == '$')
        {
            ++p;
            while (p != e)
            {
                if (*p >= '0' && *p <= '9')
                {
                    value = 16 * value + *p - '0';
                }
                else if (*p >= 'a' && *p <= 'f')
                {
                    value = 16 * value + 10 + *p - 'a';
                }
                else if (*p >= 'A' && *p <= 'F')
                {
                    value = 16 * value + 10 + *p - 'A';
                }
                else
                {
                    return value;
                }
                ++p;
            }
        }
        else
        {
            while (p != e)
            {
                if (*p >= '0' && *p <= '9')
                {
                    value = 10 * value + *p - '0';
                }
                else
                {
                    return value;
                }
                ++p;
            }
        }
    }
    return value;
}

IntegerLiteralNode* ParseIntegerLiteral(soul::lexer::LexerBase<char>& lexer, const soul::lexer::Token<char, soul::lexer::LexerBase<char>>& token, int64_t pos)
{
    const char* p = token.match.begin;
    const char* e = token.match.end;
    int32_t value = ParseIntegerLiteral(p, e, lexer, pos);
    if (p != e)
    {
        ThrowError("error: invalid integer literal", lexer, pos);
    }
    return new IntegerLiteralNode(value, pos);
}

RealLiteralNode* ParseRealLiteral(soul::lexer::LexerBase<char>& lexer, const soul::lexer::Token<char, soul::lexer::LexerBase<char>>& token, int64_t pos)
{
    std::string s = token.ToString();
    float value = std::stof(s);
    return new RealLiteralNode(value, pos);
}

std::string ParseQuotedString(soul::lexer::LexerBase<char>& lexer, const soul::lexer::Token<char, soul::lexer::LexerBase<char>>& token, int64_t pos)
{
    std::string value;
    const char* p = token.match.begin;
    const char* e = token.match.end;
    int state = 0;
    while (p != e)
    {
        switch (state)
        {
            case 0:
            {
                switch (*p)
                {
                    case '\'':
                    {
                        state = 1;
                        break;
                    }
                    default:
                    {
                        ThrowError("error: invalid string literal", lexer, pos);
                    }
                }
                break;
            }
            case 1:
            {
                switch (*p)
                {
                    case '\'':
                    {
                        state = 2;
                        break;
                    }
                    default:
                    {
                        value.append(1, *p);
                        break;
                    }
                }
                break;
            }
            case 2:
            {
                switch (*p)
                {
                    case '\'':
                    {
                        value.append(1, *p);
                        state = 1;
                        break;
                    }
                    default:
                    {
                        ThrowError("error: invalid string literal", lexer, pos);
                    }
                }
                break;
            }
        }
        ++p;
    }
    if (state != 2)
    {
        ThrowError("error: invalid string literal", lexer, pos);
    }
    return value;
}

std::string ParseControlString(soul::lexer::LexerBase<char>& lexer, const soul::lexer::Token<char, soul::lexer::LexerBase<char>>& token, int64_t pos)
{
    std::string value;
    const char* p = token.match.begin;
    const char* e = token.match.end;
    while (p != e && *p == '#')
    {
        ++p;
        if (p != e)
        {
            int32_t integerValue = ParseIntegerLiteral(p, e, lexer, pos);
            if (integerValue >= 0 && integerValue < 256)
            {
                value.append(1, char(integerValue));
            }
            else
            {
                ThrowError("error: invalid control string", lexer, pos);
            }
        }
        else
        {
            ThrowError("error: invalid control string", lexer, pos);
        }
    }
    if (p != e)
    {
        ThrowError("error: invalid control string", lexer, pos);
    }
    return value;
}

} // namespace p
