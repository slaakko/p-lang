// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.ast.expression;

import p.ast.visitor;
import p.ast.writer;
import p.ast.reader;
import p.ast.common;
import p.ast.error;

namespace p {

BinaryExprNode::BinaryExprNode(const soul::ast::Span& span_) : Node(NodeKind::binaryExprNode, span_), op(), left(), right()
{
}

BinaryExprNode::BinaryExprNode(Operator op_, Node* left_, Node* right_, const soul::ast::Span& span_) : 
    Node(NodeKind::binaryExprNode, span_), op(op_), left(left_), right(right_)
{
    left->SetParent(this);
    right->SetParent(this);
}

Node* BinaryExprNode::Clone() const
{
    return new BinaryExprNode(op, left->Clone(), right->Clone(), Span());
}

void BinaryExprNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void BinaryExprNode::Write(AstWriter& writer)
{
    writer.GetBinaryWriter().Write(static_cast<uint8_t>(op));
    writer.WriteNode(left.get());
    writer.WriteNode(right.get());
}

void BinaryExprNode::Read(AstReader& reader)
{
    op = static_cast<Operator>(reader.GetBinaryReader().ReadByte());
    left.reset(reader.ReadNode());
    left->SetParent(this);
    right.reset(reader.ReadNode());
    right->SetParent(this);
}

UnaryExprNode::UnaryExprNode(const soul::ast::Span& span_) : Node(NodeKind::unaryExprNode, span_), op(), operand()
{
}

UnaryExprNode::UnaryExprNode(Operator op_, Node* operand_, const soul::ast::Span& span_) : Node(NodeKind::unaryExprNode, span_), op(op_), operand(operand_)
{
    operand->SetParent(this);
}

Node* UnaryExprNode::Clone() const
{
    return new UnaryExprNode(op, operand->Clone(), Span());
}

void UnaryExprNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void UnaryExprNode::Write(AstWriter& writer)
{
    writer.GetBinaryWriter().Write(static_cast<uint8_t>(op));
    writer.WriteNode(operand.get());
}

void UnaryExprNode::Read(AstReader& reader)
{
    op = static_cast<Operator>(reader.GetBinaryReader().ReadByte());
    operand.reset(reader.ReadNode());
    operand->SetParent(this);
}

ParenthesizedExprNode::ParenthesizedExprNode(const soul::ast::Span& span_) : Node(NodeKind::parenthesizedExprNode, span_)
{
}

ParenthesizedExprNode::ParenthesizedExprNode(Node* expr_, const soul::ast::Span& span_) : Node(NodeKind::parenthesizedExprNode, span_), expr(expr_)
{
    expr->SetParent(this);
}

Node* ParenthesizedExprNode::Clone() const
{
    return new ParenthesizedExprNode(expr->Clone(), Span());
}

void ParenthesizedExprNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ParenthesizedExprNode::Write(AstWriter& writer)
{
    writer.WriteNode(expr.get());
}

void ParenthesizedExprNode::Read(AstReader& reader)
{
    expr.reset(reader.ReadNode());
    expr->SetParent(this);
}

IntegerLiteralNode::IntegerLiteralNode(const soul::ast::Span& span_) : Node(NodeKind::integerLiteralNode, span_), value()
{
}

IntegerLiteralNode::IntegerLiteralNode(int32_t value_, const soul::ast::Span& span_) : Node(NodeKind::integerLiteralNode, span_), value(value_)
{
}

Node* IntegerLiteralNode::Clone() const
{
    return new IntegerLiteralNode(value, Span());
}

void IntegerLiteralNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void IntegerLiteralNode::Write(AstWriter& writer)
{
    writer.GetBinaryWriter().Write(value);
}

void IntegerLiteralNode::Read(AstReader& reader)
{
    value = reader.GetBinaryReader().ReadInt();
}

RealLiteralNode::RealLiteralNode(const soul::ast::Span& span_) : Node(NodeKind::realLiteralNode, span_), value()
{
}

RealLiteralNode::RealLiteralNode(float value_, const soul::ast::Span& span_) : Node(NodeKind::realLiteralNode, span_), value(value_)
{
}

Node* RealLiteralNode::Clone() const
{
    return new RealLiteralNode(value, Span());
}

void RealLiteralNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void RealLiteralNode::Write(AstWriter& writer)
{
    writer.GetBinaryWriter().Write(value);
}

void RealLiteralNode::Read(AstReader& reader)
{
    value = reader.GetBinaryReader().ReadFloat();
}

BooleanLiteralNode::BooleanLiteralNode(const soul::ast::Span& span_) : Node(NodeKind::booleanLiteralNode, span_), value()
{
}

BooleanLiteralNode::BooleanLiteralNode(bool value_, const soul::ast::Span& span_) : Node(NodeKind::booleanLiteralNode, span_), value(value_)
{
}

Node* BooleanLiteralNode::Clone() const
{
    return new BooleanLiteralNode(value, Span());
}

void BooleanLiteralNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void BooleanLiteralNode::Write(AstWriter& writer)
{
    writer.GetBinaryWriter().Write(value);
}

void BooleanLiteralNode::Read(AstReader& reader)
{
    value = reader.GetBinaryReader().ReadBool();
}

StringLiteralNode::StringLiteralNode(const soul::ast::Span& span_) : Node(NodeKind::stringLiteralNode, span_), value()
{
}

StringLiteralNode::StringLiteralNode(const std::string& value_, const soul::ast::Span& span_) : Node(NodeKind::stringLiteralNode, span_), value(value_)
{
}

void StringLiteralNode::Append(const std::string& str)
{
    value.append(str);
}

Node* StringLiteralNode::Clone() const
{
    return new StringLiteralNode(value, Span());
}

void StringLiteralNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void StringLiteralNode::Write(AstWriter& writer)
{
    writer.GetBinaryWriter().Write(value);
}

void StringLiteralNode::Read(AstReader& reader)
{
    value = reader.GetBinaryReader().ReadUtf8String();
}

CharLiteralNode::CharLiteralNode(const soul::ast::Span& span_) : Node(NodeKind::charLiteralNode, span_), value()
{
}

CharLiteralNode::CharLiteralNode(char value_, const soul::ast::Span& span_) : Node(NodeKind::charLiteralNode, span_), value(value_)
{
}

Node* CharLiteralNode::Clone() const
{
    return new CharLiteralNode(value, Span());
}

void CharLiteralNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void CharLiteralNode::Write(AstWriter& writer)
{
    writer.GetBinaryWriter().Write(value);
}

void CharLiteralNode::Read(AstReader& reader)
{
    value = reader.GetBinaryReader().ReadChar();
}

ThisNode::ThisNode(const soul::ast::Span& span_) : Node(NodeKind::thisNode, span_)
{
}

Node* ThisNode::Clone() const
{
    return new ThisNode(Span());
}

void ThisNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ThisNode::Write(AstWriter& writer)
{
}

void ThisNode::Read(AstReader& reader)
{
}

BaseNode::BaseNode(const soul::ast::Span& span_) : Node(NodeKind::baseNode, span_)
{
}

Node* BaseNode::Clone() const
{
    return new BaseNode(Span());
}

void BaseNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void BaseNode::Write(AstWriter& writer)
{
}

void BaseNode::Read(AstReader& reader)
{
}

NilNode::NilNode(const soul::ast::Span& span_) : Node(NodeKind::nilNode, span_)
{
}

Node* NilNode::Clone() const
{
    return new NilNode(Span());
}

void NilNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void NilNode::Write(AstWriter& writer)
{
}

void NilNode::Read(AstReader& reader)
{
}

NewExprNode::NewExprNode(const soul::ast::Span& span_) : Node(NodeKind::newExprNode, span_)
{
}

NewExprNode::NewExprNode(const soul::ast::Span& span_, Node* type_) : Node(NodeKind::newExprNode, span_), type(type_)
{
    type->SetParent(this);
}

Node* NewExprNode::Clone() const
{
    return new NewExprNode(Span(), type->Clone());
}

void NewExprNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void NewExprNode::Write(AstWriter& writer)
{
    writer.WriteNode(type.get());
}

void NewExprNode::Read(AstReader& reader)
{
    type.reset(reader.ReadNode());
    type->SetParent(this);
}

NewArrayExprNode::NewArrayExprNode(const soul::ast::Span& span_) : Node(NodeKind::newArrayExprNode, span_)
{
}

NewArrayExprNode::NewArrayExprNode(const soul::ast::Span& span_, Node* elementType_, Node* arraySize_) :
    Node(NodeKind::newArrayExprNode, span_), elementType(elementType_), arraySize(arraySize_)
{
    elementType->SetParent(this);
    arraySize->SetParent(this);
}

Node* NewArrayExprNode::Clone() const
{
    return new NewArrayExprNode(Span(), elementType->Clone(), arraySize->Clone());
}

void NewArrayExprNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void NewArrayExprNode::Write(AstWriter& writer)
{
    writer.WriteNode(elementType.get());
    writer.WriteNode(arraySize.get());
}

void NewArrayExprNode::Read(AstReader& reader)
{
    elementType.reset(reader.ReadNode());
    elementType->SetParent(this);
    arraySize.reset(reader.ReadNode());
    arraySize->SetParent(this);
}

InvokeExprNode::InvokeExprNode(const soul::ast::Span& span_) : Node(NodeKind::invokeExprNode, span_)
{
}

InvokeExprNode::InvokeExprNode(const soul::ast::Span& span_, Node* subject_) : Node(NodeKind::invokeExprNode, span_), subject(subject_)
{
    subject->SetParent(this);
}

void InvokeExprNode::AddNode(Node* node)
{
    node->SetParent(this);
    arguments.push_back(std::unique_ptr<Node>(node));
}

Node* InvokeExprNode::Clone() const
{
    InvokeExprNode* clone = new InvokeExprNode(Span(), subject->Clone());
    for (const auto& arg : arguments)
    {
        clone->AddNode(arg->Clone());
    }
    return clone;
}

void InvokeExprNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void InvokeExprNode::Write(AstWriter& writer)
{
    writer.WriteNode(subject.get());
    writer.GetBinaryWriter().Write(static_cast<int32_t>(arguments.size()));
    for (const auto& arg : arguments)
    {
        writer.WriteNode(arg.get());
    }
}

void InvokeExprNode::Read(AstReader& reader)
{
    subject.reset(reader.ReadNode());
    subject->SetParent(this);
    int32_t n = reader.GetBinaryReader().ReadInt();
    for (int32_t i = 0; i < n; ++i)
    {
        Node* arg = reader.ReadNode();
        AddNode(arg);
    }
}

IndexExprNode::IndexExprNode(const soul::ast::Span& span_) : Node(NodeKind::indexExprNode, span_)
{
}

IndexExprNode::IndexExprNode(const soul::ast::Span& span_, Node* subject_) : Node(NodeKind::indexExprNode, span_), subject(subject_)
{
    subject->SetParent(this);
}

void IndexExprNode::SetIndex(Node* index_) 
{
    index.reset(index_);
    index->SetParent(this);
}

Node* IndexExprNode::Clone() const
{
    IndexExprNode* clone = new IndexExprNode(Span(), subject->Clone());
    clone->SetIndex(index->Clone());
    return clone;
}

void IndexExprNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void IndexExprNode::Write(AstWriter& writer)
{
    writer.WriteNode(subject.get());
    writer.WriteNode(index.get());
}

void IndexExprNode::Read(AstReader& reader)
{
    subject.reset(reader.ReadNode());
    subject->SetParent(this);
    index.reset(reader.ReadNode());
    index->SetParent(this);
}

DotNode::DotNode(const soul::ast::Span& span_) : Node(NodeKind::dotNode, span_)
{
}

DotNode::DotNode(const soul::ast::Span& span_, Node* subject_) : Node(NodeKind::dotNode, span_), subject(subject_)
{
    subject->SetParent(this);
}

void DotNode::SetId(IdentifierNode* id_)
{
    id.reset(id_);
    id->SetParent(this);
}

Node* DotNode::Clone() const
{
    DotNode* clone = new DotNode(Span(), subject->Clone());
    clone->SetId(static_cast<IdentifierNode*>(id->Clone()));
    return clone;
}

void DotNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void DotNode::Write(AstWriter& writer)
{
    writer.WriteNode(subject.get());
    writer.WriteNode(id.get());
}

void DotNode::Read(AstReader& reader)
{
    subject.reset(reader.ReadNode());
    subject->SetParent(this);
    id.reset(static_cast<IdentifierNode*>(reader.ReadNode()));
    id->SetParent(this);
}

Node* MakeCharacterStringConstant(StringLiteralNode* node, const soul::ast::Span& span)
{
    const std::string& str = node->Value();
    if (str.length() == 1)
    {
        return new CharLiteralNode(str[0], span);
    }
    else
    {
        return new StringLiteralNode(str, span);
    }
}

int32_t ParseIntegerLiteral(const char*& p, const char* e)
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

IntegerLiteralNode* ParseIntegerLiteral(soul::lexer::LexerBase<char>& lexer, const soul::lexer::Token<char, soul::lexer::LexerBase<char>>& token, const soul::ast::Span& span)
{
    const char* p = token.match.begin;
    const char* e = token.match.end;
    int32_t value = ParseIntegerLiteral(p, e);
    if (p != e)
    {
        ThrowError("error: invalid integer literal", lexer.FileName(), span);
    }
    return new IntegerLiteralNode(value, span);
}

RealLiteralNode* ParseRealLiteral(soul::lexer::LexerBase<char>& lexer, const soul::lexer::Token<char, soul::lexer::LexerBase<char>>& token, const soul::ast::Span& span)
{
    std::string s = token.ToString();
    float value = std::stof(s);
    return new RealLiteralNode(value, span);
}

std::string ParseQuotedString(soul::lexer::LexerBase<char>& lexer, const soul::lexer::Token<char, soul::lexer::LexerBase<char>>& token, const soul::ast::Span& span)
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
                ThrowError("error: invalid string literal", lexer.FileName(), span);
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
                ThrowError("error: invalid string literal", lexer.FileName(), span);
            }
            }
            break;
        }
        }
        ++p;
    }
    if (state != 2)
    {
        ThrowError("error: invalid string literal", lexer.FileName(), span);
    }
    return value;
}

std::string ParseControlString(soul::lexer::LexerBase<char>& lexer, const soul::lexer::Token<char, soul::lexer::LexerBase<char>>& token, const soul::ast::Span& span)
{
    std::string value;
    const char* p = token.match.begin;
    const char* e = token.match.end;
    while (p != e && *p == '#')
    {
        ++p;
        if (p != e)
        {
            int32_t integerValue = ParseIntegerLiteral(p, e);
            if (integerValue >= 0 && integerValue < 256)
            {
                value.append(1, char(integerValue));
            }
            else
            {
                ThrowError("error: invalid control string", lexer.FileName(), span);
            }
        }
        else
        {
            ThrowError("error: invalid control string", lexer.FileName(), span);
        }
    }
    if (p != e)
    {
        ThrowError("error: invalid control string", lexer.FileName(), span);
    }
    return value;
}

} // namespace p
