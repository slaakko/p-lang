// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.stdfn;

import p.subroutine;
import std.core;

export namespace p {

class Block;
class ExecutionContext;

class StandardFunction : public Function
{
public:
    StandardFunction(const std::string& fullName_);
    void SetId(int32_t id_) { id = id_; }
    int32_t Id() const { return id; }
    virtual void Execute(ExecutionContext* context) = 0;
private:
    int32_t id;
};

class StandardProcedure : public Procedure
{
public:
    StandardProcedure(const std::string& fullName_);
    void SetId(int32_t id_) { id = id_; }
    int32_t Id() const { return id; }
    virtual void Execute(ExecutionContext* context) = 0;
private:
    int32_t id;
};

class Ord : public StandardFunction
{
public:
    Ord();
    Value* Evaluate(const std::vector<std::unique_ptr<Value>>& argumentValues, soul::lexer::LexerBase<char>& lexer, int64_t pos) override;
    int32_t MinParameterCount() const { return 1; }
    int32_t ParameterCount() const { return 1; }
    void Execute(ExecutionContext* context) override;
};

class Chr : public StandardFunction
{
public:
    Chr();
    Value* Evaluate(const std::vector<std::unique_ptr<Value>>& argumentValues, soul::lexer::LexerBase<char>& lexer, int64_t pos) override;
    int32_t MinParameterCount() const { return 1; }
    int32_t ParameterCount() const { return 1; }
    void Execute(ExecutionContext* context) override;
};

class Sin: public StandardFunction
{
public:
    Sin();
    int32_t MinParameterCount() const { return 1; }
    int32_t ParameterCount() const { return 1; }
    void Execute(ExecutionContext* context) override;
};

class Cos : public StandardFunction
{
public:
    Cos();
    int32_t MinParameterCount() const { return 1; }
    int32_t ParameterCount() const { return 1; }
    void Execute(ExecutionContext* context) override;
};

class ArcTan : public StandardFunction
{
public:
    ArcTan();
    int32_t MinParameterCount() const { return 2; }
    int32_t ParameterCount() const { return 2; }
    void Execute(ExecutionContext* context) override;
};

class Abs: public StandardFunction
{
public:
    Abs();
    int32_t MinParameterCount() const { return 1; }
    int32_t ParameterCount() const { return 1; }
    Value* Evaluate(const std::vector<std::unique_ptr<Value>>& argumentValues, soul::lexer::LexerBase<char>& lexer, int64_t pos) override;
    void Execute(ExecutionContext* context) override;
    Type* ResultType(const std::vector<Type*>& argumentTypes) const override;
};

class Succ : public StandardFunction
{
public:
    Succ();
    int32_t MinParameterCount() const { return 1; }
    int32_t ParameterCount() const { return 1; }
    Value* Evaluate(const std::vector<std::unique_ptr<Value>>& argumentValues, soul::lexer::LexerBase<char>& lexer, int64_t pos) override;
    void Execute(ExecutionContext* context) override;
    Type* ResultType(const std::vector<Type*>& argumentTypes) const override;
};

class Pred : public StandardFunction
{
public:
    Pred();
    int32_t MinParameterCount() const { return 1; }
    int32_t ParameterCount() const { return 1; }
    Value* Evaluate(const std::vector<std::unique_ptr<Value>>& argumentValues, soul::lexer::LexerBase<char>& lexer, int64_t pos) override;
    void Execute(ExecutionContext* context) override;
    Type* ResultType(const std::vector<Type*>& argumentTypes) const override;
};

class Min : public StandardFunction
{
public:
    Min();
    int32_t MinParameterCount() const { return 2; }
    int32_t ParameterCount() const { return 2; }
    void Execute(ExecutionContext* context) override;
    Type* ResultType(const std::vector<Type*>& argumentTypes) const override;
};

class Max : public StandardFunction
{
public:
    Max();
    int32_t MinParameterCount() const { return 2; }
    int32_t ParameterCount() const { return 2; }
    void Execute(ExecutionContext* context) override;
    Type* ResultType(const std::vector<Type*>& argumentTypes) const override;
};

class Sqrt : public StandardFunction
{
public:
    Sqrt();
    int32_t MinParameterCount() const { return 1; }
    int32_t ParameterCount() const { return 1; }
    void Execute(ExecutionContext* context) override;
};

class Write : public StandardProcedure
{
public:
    Write();
    int32_t MinParameterCount() const { return 0; }
    int32_t ParameterCount() const { return 0; }
    void Execute(ExecutionContext* context) override;
};

class Writeln : public StandardProcedure
{
public:
    Writeln();
    int32_t MinParameterCount() const { return 0; }
    int32_t ParameterCount() const { return 0; }
    void Execute(ExecutionContext* context) override;
};

void AddStandardFunctions(Block* block);
StandardFunction* GetStandardFunction(int32_t standardFunctionId);

void AddStandardProcedures(Block* block);
StandardProcedure* GetStandardProcedure(int32_t standardProcedureId);

void SetStandardFunctionReturnTypes(Block* block);

} // namespace p
