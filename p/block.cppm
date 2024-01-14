// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.block;

import p.type;
import p.constant;
import p.variable;
import p.subroutine;
import soul.lexer;
import util;
import std.core;

export namespace p {

namespace block {}

class Node;
class CompoundStatementNode;
class Writer;
class Reader;
class Module;
class ExecutionContext;

class Block
{
public:
    Block();
    Block(Block* parent_);
    void AddFundamentalTypes();
    void AddOperatorFunctions();
    void SetSubroutine(Subroutine* subroutine_);
    Subroutine* GetSubroutine() const { return subroutine; }
    void Write(Writer& writer);
    void Read(Reader& reader);
    void Resolve(Context* context);
    Type* GetFundamentalType(TypeKind kind);
    Type* GetFundamentalType(TypeKind kind, soul::lexer::LexerBase<char>& lexer, int64_t pos) const;
    Type* GetType(const std::string& typeName) const;
    Type* GetType(const std::string& typeName, soul::lexer::LexerBase<char>& lexer, int64_t pos) const;
    Type* GetType(const util::uuid& typeId) const;
    ArrayType* GetArrayType(const std::string& elementTypeName, soul::lexer::LexerBase<char>& lexer, int64_t pos) const;
    void AddType(Type* type);
    void AddType(Type* type, soul::lexer::LexerBase<char>& lexer, int64_t pos);
    Constant* GetConstant(const std::string& constantName) const;
    Constant* GetConstant(const std::string& constantName, soul::lexer::LexerBase<char>& lexer, int64_t pos) const;
    Constant* GetConstant(const util::uuid& constantId) const;
    void AddConstant(Constant* constant);
    void AddConstant(Constant* constant, soul::lexer::LexerBase<char>& lexer, int64_t pos);
    Variable* GetVariable(const std::string& variableName) const;
    void AddVariable(Variable* variable);
    void AddVariable(Variable* variable, soul::lexer::LexerBase<char>& lexer, int64_t pos);
    void AddParameter(Parameter* parameter);
    int32_t NumVariables() const { return variables.size(); }
    void AddProcedure(Procedure* procedure);
    void AddProcedure(Procedure* procedure, soul::lexer::LexerBase<char>& lexer, int64_t pos);
    void AddFunction(Function* function);
    void AddFunction(Function* function, soul::lexer::LexerBase<char>& lexer, int64_t pos);
    void AddConstructor(Constructor* constructor);
    void AddConstructor(Constructor* constructor, soul::lexer::LexerBase<char>& lexer, int64_t pos);
    Procedure* GetProcedure(const std::string& procedureName) const;
    Function* GetFunction(const std::string& functionName) const;
    GlobalTypeMap* GetGlobalTypeMap() const;
    GlobalConstantMap* GetGlobalConstantMap() const;
    GlobalVariableMap* GetGlobalVariableMap() const;
    void ImportModules(ParsingContext* context);
    void Import(Module* mod);
    void Import(Block* blk, Module* mod);
    void Print(util::CodeFormatter& formatter, ExecutionContext* context);
    void CheckInterface();
    void MakeVmts(Context* context);
    int32_t Level() const { return level; }
    void SetLevel(int32_t level_) { level = level_; }
    bool ReturnTypesSet() const { return returnTypesSet; }
    void SetReturnTypesSet() { returnTypesSet = true; }
private:
    Block* parent;
    Subroutine* subroutine;
    int32_t level;
    std::set<Module*> importedModules;
    std::map<std::string, Type*> typeMap;
    std::vector<std::unique_ptr<Type>> types;
    std::map<std::string, ArrayType*> arrayTypeMap;
    std::map<std::string, Constant*> constantMap;
    std::vector<std::unique_ptr<Constant>> constants;
    std::map<std::string, Variable*> variableMap;
    std::vector<std::unique_ptr<Variable>> variables;
    std::map<std::string, Procedure*> procedureMap;
    std::vector<std::unique_ptr<Procedure>> procedures;
    std::map<std::string, Function*> functionMap;
    std::vector<std::unique_ptr<Function>> functions;
    std::vector<std::unique_ptr<Constructor>> constructors;
    std::unique_ptr<GlobalTypeMap> globalTypeMap;
    std::unique_ptr<GlobalConstantMap> globalConstantMap;
    std::unique_ptr<GlobalVariableMap> globalVariableMap;
    bool operatorFunctionsAdded;
    bool returnTypesSet;
};

void MakeVariables(ParsingContext* context, const std::vector<std::string>& variableNames, Type* type, soul::lexer::LexerBase<char>& lexer, int64_t pos);

void AddSimpleConstant(ParsingContext* context, Node* constantExpression, Type* type, soul::lexer::LexerBase<char>& lexer, int64_t pos);

void MakeArrayConstant(ParsingContext* context, Type* type, Type*& elementType, soul::lexer::LexerBase<char>& lexer, int64_t pos);

void MakeObjectConstant(ParsingContext* context, Type* type, soul::lexer::LexerBase<char>& lexer, int64_t pos);

Type* GetFieldType(Type* type, const std::string& fieldName, soul::lexer::LexerBase<char>& lexer, int64_t pos);

void CompileStatementPart(ParsingContext* context, CompoundStatementNode* compoundStatement, soul::lexer::LexerBase<char>& lexer);

void NoSubroutineBlock(soul::lexer::LexerBase<char>& lexer, int64_t pos);

void GenerateImplementationIds(ParsingContext* context, soul::lexer::LexerBase<char>& lexer, int64_t pos);

void GenerateDefaultImplementations(ParsingContext* context, soul::lexer::LexerBase<char>& lexer, int64_t pos);

} // namespace p
