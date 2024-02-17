// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.ast.expression;

import p.ast.node;
import soul.ast.span;
import soul.lexer;
import std.core;

export namespace p {

class IdentifierNode;

enum class Operator
{
    equal, notEqual, less, lessOrEqual, greater, greaterOrEqual, plus, minus, or_, xor_, mul, fractionalDivide, div, mod, and_, shl, shr, not_,
};

enum class IdentifierKind
{
    none, constant, type, variable, object, procedure, function
};

class BinaryExprNode : public Node
{
public:
    BinaryExprNode(const soul::ast::Span& span_);
    BinaryExprNode(Operator op_, Node* left_, Node* right_, const soul::ast::Span& span_);
    Operator Op() const { return op; }
    Node* Left() const { return left.get(); }
    Node* Right() const { return right.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
private:
    Operator op;
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
};

class UnaryExprNode : public Node
{
public:
    UnaryExprNode(const soul::ast::Span& span_);
    UnaryExprNode(Operator op_, Node* operand_, const soul::ast::Span& span_);
    Operator Op() const { return op; }
    Node* Operand() const { return operand.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
private:
    Operator op;
    std::unique_ptr<Node> operand;
};

class ParenthesizedExprNode : public Node
{
public:
    ParenthesizedExprNode(const soul::ast::Span& span_);
    ParenthesizedExprNode(Node* expr_, const soul::ast::Span& span_);
    Node* Expr() const { return expr.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
private:
    std::unique_ptr<Node> expr;
};

class IntegerLiteralNode : public Node
{
public:
    IntegerLiteralNode(const soul::ast::Span& span_);
    IntegerLiteralNode(int32_t value_, const soul::ast::Span& span_);
    int32_t Value() const { return value; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
private:
    int32_t value;
};

class RealLiteralNode : public Node
{
public:
    RealLiteralNode(const soul::ast::Span& span_);
    RealLiteralNode(float value_, const soul::ast::Span& span_);
    float Value() const { return value; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
private:
    float value;
};

class BooleanLiteralNode : public Node
{
public:
    BooleanLiteralNode(const soul::ast::Span& span_);
    BooleanLiteralNode(bool value_, const soul::ast::Span& span_);
    bool Value() const { return value; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
private:
    bool value;
};

class StringLiteralNode : public Node
{
public:
    StringLiteralNode(const soul::ast::Span& span_);
    StringLiteralNode(const std::string&  value_, const soul::ast::Span& span_);
    const std::string& Value() const { return value; }
    void Append(const std::string& str);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
private:
    std::string value;
};

class CharLiteralNode : public Node
{
public:
    CharLiteralNode(const soul::ast::Span& span_);
    CharLiteralNode(char value_, const soul::ast::Span& span_);
    char Value() const { return value; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
private:
    char value;
};

class ThisNode : public Node
{
public:
    ThisNode(const soul::ast::Span& span_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
};

class BaseNode : public Node
{
public:
    BaseNode(const soul::ast::Span& span_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
};

class NilNode : public Node
{
public:
    NilNode(const soul::ast::Span& span_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
};

class NewExprNode : public Node
{
public:
    NewExprNode(const soul::ast::Span& span_);
    NewExprNode(const soul::ast::Span& span_, Node* type_);
    Node* Type() const { return type.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
private:
    std::unique_ptr<Node> type;
};

class NewArrayExprNode : public Node
{
public:
    NewArrayExprNode(const soul::ast::Span& span_);
    NewArrayExprNode(const soul::ast::Span& span_, Node* elementType_, Node* arraySize_);
    Node* ElementType() const { return elementType.get(); }
    Node* ArraySize() const { return arraySize.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
private:
    std::unique_ptr<Node> elementType;
    std::unique_ptr<Node> arraySize;
};

class InvokeExprNode : public Node
{
public:
    InvokeExprNode(const soul::ast::Span& span_);
    InvokeExprNode(const soul::ast::Span& span_, Node* subject_);
    Node* Subject() const { return subject.get(); }
    const std::vector<std::unique_ptr<Node>>& Arguments() const { return arguments; }
    void AddNode(Node* node) override;
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
private:
    std::unique_ptr<Node> subject;
    std::vector<std::unique_ptr<Node>> arguments;
};

class IndexExprNode : public Node
{
public:
    IndexExprNode(const soul::ast::Span& span_);
    IndexExprNode(const soul::ast::Span& span_, Node* subject_);
    Node* Subject() const { return subject.get(); }
    Node* Index() const { return index.get(); }
    void SetIndex(Node* index_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
private:
    std::unique_ptr<Node> subject;
    std::unique_ptr<Node> index;
};

class DotNode : public Node
{
public:
    DotNode(const soul::ast::Span& span_);
    DotNode(const soul::ast::Span& span_, Node* subject_);
    Node* Subject() const { return subject.get(); }
    IdentifierNode* Id() const { return id.get(); }
    void SetId(IdentifierNode* id_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
private:
    std::unique_ptr<Node> subject;
    std::unique_ptr<IdentifierNode> id;
};

Node* MakeCharacterStringConstant(StringLiteralNode* node, const soul::ast::Span& span);

int32_t ParseIntegerLiteral(const char*& p, const char* e);
IntegerLiteralNode* ParseIntegerLiteral(soul::lexer::LexerBase<char>& lexer, const soul::lexer::Token<char, soul::lexer::LexerBase<char>>& token, const soul::ast::Span& span);
RealLiteralNode* ParseRealLiteral(soul::lexer::LexerBase<char>& lexer, const soul::lexer::Token<char, soul::lexer::LexerBase<char>>& token, const soul::ast::Span& span);
std::string ParseQuotedString(soul::lexer::LexerBase<char>& lexer, const soul::lexer::Token<char, soul::lexer::LexerBase<char>>& token, const soul::ast::Span& span);
std::string ParseControlString(soul::lexer::LexerBase<char>& lexer, const soul::lexer::Token<char, soul::lexer::LexerBase<char>>& token, const soul::ast::Span& span);

} // namespace p
