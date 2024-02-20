// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.core.standard_subroutine;

import p.core.subroutine_symbol;
import std.core;

export namespace p {

class ExecutionContext;
class Context;

class StandardFunctionSymbol : public FunctionSymbol
{
public:
    StandardFunctionSymbol(SymbolKind kind_, const soul::ast::Span& span_, const std::string& name_);
    virtual void Execute(ExecutionContext* context) = 0;
};

class StandardProcedureSymbol : public ProcedureSymbol
{
public:
    StandardProcedureSymbol(SymbolKind kind_, const soul::ast::Span& span_, const std::string& name_);
    virtual void Execute(ExecutionContext* context) = 0;
};

class Ord : public StandardFunctionSymbol
{
public:
    Ord(Node* node, Context* context);
    Ord(const soul::ast::Span& span_, const std::string& name_);
    int ParameterCount() const override { return 1; }
    Value* Evaluate(const std::vector<std::unique_ptr<Value>>& argumentValues, Context* context, Node* node) const override;
    void Execute(ExecutionContext* context) override;
};

class Chr : public StandardFunctionSymbol
{
public:
    Chr(Node* node, Context* context);
    Chr(const soul::ast::Span& span_, const std::string& name_);
    int ParameterCount() const override { return 1; }
    Value* Evaluate(const std::vector<std::unique_ptr<Value>>& argumentValues, Context* context, Node* node) const override;
    void Execute(ExecutionContext* context) override;
};

class Sin : public StandardFunctionSymbol
{
public:
    Sin(Node* node, Context* context);
    Sin(const soul::ast::Span& span_, const std::string& name_);
    int ParameterCount() const override { return 1; }
    void Execute(ExecutionContext* context) override;
};

class Cos : public StandardFunctionSymbol
{
public:
    Cos(Node* node, Context* context);
    Cos(const soul::ast::Span& span_, const std::string& name_);
    int ParameterCount() const override { return 1; }
    void Execute(ExecutionContext* context) override;
};

class ArcTan : public StandardFunctionSymbol
{
public:
    ArcTan(Node* node, Context* context);
    ArcTan(const soul::ast::Span& span_, const std::string& name_);
    int ParameterCount() const override { return 1; }
    void Execute(ExecutionContext* context) override;
};

class Sqrt : public StandardFunctionSymbol
{
public:
    Sqrt(Node* node, Context* context);
    Sqrt(const soul::ast::Span& span_, const std::string& name_);
    int ParameterCount() const override { return 1; }
    void Execute(ExecutionContext* context) override;
};

class Abs : public StandardFunctionSymbol
{
public:
    Abs(Node* node, Context* context);
    Abs(const soul::ast::Span& span_, const std::string& name_);
    int ParameterCount() const override { return 1; }
    Value* Evaluate(const std::vector<std::unique_ptr<Value>>& argumentValues, Context* context, Node* node) const override;
    void Execute(ExecutionContext* context) override;
};

class Succ : public StandardFunctionSymbol
{
public:
    Succ(Node* node, Context* context);
    Succ(const soul::ast::Span& span_, const std::string& name_);
    int ParameterCount() const override { return 1; }
    Value* Evaluate(const std::vector<std::unique_ptr<Value>>& argumentValues, Context* context, Node* node) const override;
    void Execute(ExecutionContext* context) override;
};

class Pred : public StandardFunctionSymbol
{
public:
    Pred(Node* node, Context* context);
    Pred(const soul::ast::Span& span_, const std::string& name_);
    int ParameterCount() const override { return 1; }
    Value* Evaluate(const std::vector<std::unique_ptr<Value>>& argumentValues, Context* context, Node* node) const override;
    void Execute(ExecutionContext* context) override;
};

class Min : public StandardFunctionSymbol
{
public:
    Min(Node* node, Context* context);
    Min(const soul::ast::Span& span_, const std::string& name_);
    int ParameterCount() const override { return 2; }
    TypeSymbol* ResultType(const std::vector<TypeSymbol*>& argumentTypes, Node* node) const override;
    void Execute(ExecutionContext* context) override;
};

class Max : public StandardFunctionSymbol
{
public:
    Max(Node* node, Context* context);
    Max(const soul::ast::Span& span_, const std::string& name_);
    int ParameterCount() const override { return 2; }
    TypeSymbol* ResultType(const std::vector<TypeSymbol*>& argumentTypes, Node* node) const override;
    void Execute(ExecutionContext* context) override;
};

class Write : public StandardProcedureSymbol
{
public:
    Write(Node* node, Context* context);
    Write(const soul::ast::Span& span_, const std::string& name_);
    bool IsVariableParamSubroutine() const override { return true; }
    void Execute(ExecutionContext* context) override;
};

class Writeln : public StandardProcedureSymbol
{
public:
    Writeln(Node* node, Context* context);
    Writeln(const soul::ast::Span& span_, const std::string& name_);
    bool IsVariableParamSubroutine() const override { return true; }
    void Execute(ExecutionContext* context) override;
};

void CreateStandardSubroutines(Node* node, Context* context);

} // namespace p
