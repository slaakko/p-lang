// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.binder.type_binder;

import p.binder.evaluator;
import p.binder.statement_binder;
import p.binder.code_generator;
import util;

namespace p {

TypeBinder::TypeBinder(Context* context_) : 
    symbolTable(new SymbolTable()), context(context_), currentBlock(nullptr), currentContainer(nullptr), typeId(nullptr), makeType(false), external(false), forward(false),
    currentFunction(nullptr), currentProcedure(nullptr), currentConstructor(nullptr), currentSubroutine(nullptr)
{
    context->SetSymbolTable(symbolTable.get());
    unitId = util::GetSha1MessageDigest(util::ToString(util::uuid::random()));
}

void TypeBinder::Visit(ProgramNode& node)
{
    symbolTable->SetRoot(new ProgramSymbol(node.Span(), node.UnitName()->Name(), node.FilePath()));
    context->GetUnitLoader()->ImportUnit("System", symbolTable->Root(), context);
    symbolTable->Root()->AddUsedUnitName("System");
    MakeOperatorFunctions(symbolTable.get(), node.FilePath(), node.Span());
    if (node.Uses())
    {
        node.Uses()->Accept(*this);
    }
    BlockSymbol* prevBlock = currentBlock;
    currentBlock = symbolTable->Root()->Block();
    currentBlock->SetLevel(0);
    ProcedureSymbol* program = new ProcedureSymbol(node.Span(), "@program");
    program->SetGenerated();
    currentBlock->AddSymbol(program);
    program->SetLevel(0);
    SubroutineSymbol* prevSubroutine = currentSubroutine;
    currentSubroutine = program;
    ContainerSymbol* prevContainer = currentContainer;
    currentContainer = currentSubroutine;
    context->SetCurrentUnitPartKind(UnitPartKind::program);
    node.Block()->Accept(*this);
    currentContainer = prevContainer;
    currentSubroutine = prevSubroutine;
    currentBlock = prevBlock;
}

void TypeBinder::Visit(UnitNode& node)
{
    symbolTable->SetRoot(new UnitSymbol(node.Span(), node.UnitName()->Name(), node.FilePath()));
    if (symbolTable->Root()->Name() == "System")
    {
        CreateFundamentalTypes(symbolTable.get(), node.Span());
        CreateStandardSubroutines(&node, context);
    }
    node.InterfacePart()->Accept(*this);
    node.ImplementationPart()->Accept(*this);
    node.InitializationPart()->Accept(*this);
}

void TypeBinder::Visit(UnitPartNode& node)
{
    std::string unitPartName;
    context->SetCurrentUnitPartKind(node.PartKind());
    switch (node.PartKind())
    {
        case UnitPartKind::interfacePart: unitPartName = "interface_" + unitId; break;
        case UnitPartKind::implementationPart: unitPartName = "implementation_" + unitId; break;
        case UnitPartKind::initializationPart: unitPartName = "initialization_" + unitId; break;
    }
    UnitPartSymbol* unitPartSymbol = new UnitPartSymbol(node.Span(), unitPartName, node.PartKind());
    symbolTable->Root()->Block()->AddSymbol(unitPartSymbol);
    if (symbolTable->Root()->Name() != "System")
    {
        context->GetUnitLoader()->ImportUnit("System", symbolTable->Root(), context);
        symbolTable->Root()->AddUsedUnitName("System");
    }
    MakeOperatorFunctions(symbolTable.get(), node.FilePath(), node.Span());
    if (node.Uses())
    {
        node.Uses()->Accept(*this);
    }
    SubroutineSymbol* prevSubroutine = currentSubroutine;
    BlockSymbol* prevBlock = currentBlock;
    ContainerSymbol* prevContainer = currentContainer;
    currentBlock = unitPartSymbol->Block();
    currentBlock->SetLevel(0);
    currentContainer = currentBlock;
    if (node.PartKind() == UnitPartKind::implementationPart)
    {
        symbolTable->Root()->Block()->Import(symbolTable->Root()->Block()->InterfacePart()->Block());
        symbolTable->Root()->Block()->InterfacePart()->SetImplementationBlock(symbolTable->Root()->Block()->ImplementationPart()->Block());
    }
    else if (node.PartKind() == UnitPartKind::initializationPart)
    {
        if (node.Block()->CompoundStatement())
        {
            ProcedureSymbol* init = new ProcedureSymbol(node.Span(), "@init");
            init->SetGenerated();
            currentBlock->AddSymbol(init);
            init->SetLevel(0);
            currentSubroutine = init;
            currentContainer = currentSubroutine;
            symbolTable->Root()->Block()->InitializationPart()->SetImplementationBlock(symbolTable->Root()->Block()->InterfacePart()->Block());
        }
    }
    currentBlock->SetLevel(0);
    node.Block()->Accept(*this);
    currentContainer = prevContainer;
    currentBlock = prevBlock;
    currentSubroutine = prevSubroutine;
}

void TypeBinder::Visit(UnitNameNode& node)
{
    if (node.Name() == "System") return;
    context->GetUnitLoader()->ImportUnit(node.Name(), symbolTable->Root(), context);
    symbolTable->Root()->AddUsedUnitName(node.Name());
}

void TypeBinder::Visit(UnitNameListNode& node) 
{
    for (const auto& unitName : node.UnitNameNodes())
    {
        unitName->Accept(*this);
    }
}

void TypeBinder::Visit(IntegerNode& node)
{
    if (makeType)
    {
        std::string typeName;
        if (typeId)
        {
            typeName = typeId->Str();
        }
        else
        {
            typeName = "@alias_type_" + util::GetSha1MessageDigest(util::ToString(util::uuid::random()));
        }
        TypeSymbol* type = GetType(&node, currentContainer, context);
        currentBlock->AddSymbol(new AliasTypeSymbol(node.Span(), typeName, type));
    }
}

void TypeBinder::Visit(RealNode& node)
{
    if (makeType)
    {
        std::string typeName;
        if (typeId)
        {
            typeName = typeId->Str();
        }
        else
        {
            typeName = "@alias_type_" + util::GetSha1MessageDigest(util::ToString(util::uuid::random()));
        }
        TypeSymbol* type = GetType(&node, currentContainer, context);
        currentBlock->AddSymbol(new AliasTypeSymbol(node.Span(), typeName, type));
    }
}

void TypeBinder::Visit(CharNode& node)
{
    if (makeType)
    {
        std::string typeName;
        if (typeId)
        {
            typeName = typeId->Str();
        }
        else
        {
            typeName = "@alias_type_" + util::GetSha1MessageDigest(util::ToString(util::uuid::random()));
        }
        TypeSymbol* type = GetType(&node, currentContainer, context);
        currentBlock->AddSymbol(new AliasTypeSymbol(node.Span(), typeName, type));
    }
}

void TypeBinder::Visit(BooleanNode& node)
{
    if (makeType)
    {
        std::string typeName;
        if (typeId)
        {
            typeName = typeId->Str();
        }
        else
        {
            typeName = "@alias_type_" + util::GetSha1MessageDigest(util::ToString(util::uuid::random()));
        }
        TypeSymbol* type = GetType(&node, currentContainer, context);
        currentBlock->AddSymbol(new AliasTypeSymbol(node.Span(), typeName, type));
    }
}

void TypeBinder::Visit(StringNode& node)
{
    if (makeType)
    {
        std::string typeName;
        if (typeId)
        {
            typeName = typeId->Str();
        }
        else
        {
            typeName = "@alias_type_" + util::GetSha1MessageDigest(util::ToString(util::uuid::random()));
        }
        TypeSymbol* type = GetType(&node, currentContainer, context);
        currentBlock->AddSymbol(new AliasTypeSymbol(node.Span(), typeName, type));
    }
}

void TypeBinder::Visit(PointerNode& node)
{
    if (makeType)
    {
        std::string typeName;
        if (typeId)
        {
            typeName = typeId->Str();
        }
        else
        {
            typeName = "@alias_type_" + util::GetSha1MessageDigest(util::ToString(util::uuid::random()));
        }
        TypeSymbol* type = GetType(&node, currentContainer, context);
        currentBlock->AddSymbol(new AliasTypeSymbol(node.Span(), typeName, type));
    }
}

void TypeBinder::Visit(IdentifierNode& node)
{
    if (makeType)
    {
        std::string typeName;
        if (typeId)
        {
            typeName = typeId->Str();
        }
        else
        {
            typeName = "@alias_type_" + util::GetSha1MessageDigest(util::ToString(util::uuid::random()));
        }
        TypeSymbol* type = GetType(&node, currentContainer, context);
        currentBlock->AddSymbol(new AliasTypeSymbol(node.Span(), typeName, type));
    }
}

void TypeBinder::Visit(IdentifierListNode& node)
{
}

void TypeBinder::Visit(BinaryExprNode& node)
{
}

void TypeBinder::Visit(UnaryExprNode& node)
{
}

void TypeBinder::Visit(ParenthesizedExprNode& node)
{
}

void TypeBinder::Visit(IntegerLiteralNode& node)
{
}

void TypeBinder::Visit(RealLiteralNode& node)
{
}

void TypeBinder::Visit(BooleanLiteralNode& node)
{
}

void TypeBinder::Visit(StringLiteralNode& node)
{
}

void TypeBinder::Visit(CharLiteralNode& node)
{
}

void TypeBinder::Visit(ThisNode& node)
{
}

void TypeBinder::Visit(BaseNode& node)
{
}

void TypeBinder::Visit(NilNode& node)
{
}

void TypeBinder::Visit(NewExprNode& node)
{
}

void TypeBinder::Visit(NewArrayExprNode& node)
{
}

void TypeBinder::Visit(InvokeExprNode& node)
{
}

void TypeBinder::Visit(IndexExprNode& node)
{
}

void TypeBinder::Visit(DotNode& node)
{
}

void TypeBinder::Visit(AssignmentStatementNode& node)
{
}

void TypeBinder::Visit(ExpressionStatementNode& node)
{
}

void TypeBinder::Visit(EmptyStatementNode& node)
{
}

void TypeBinder::Visit(CompoundStatementNode& node)
{
}

void TypeBinder::Visit(IfStatementNode& node)
{
}

void TypeBinder::Visit(CaseNode& node)
{
}

void TypeBinder::Visit(ConstantRangeNode& node)
{
}

void TypeBinder::Visit(CaseStatementNode& node)
{
}

void TypeBinder::Visit(RepeatStatementNode& node)
{
}

void TypeBinder::Visit(WhileStatementNode& node)
{
}

void TypeBinder::Visit(ForStatementNode& node)
{
}

void TypeBinder::Visit(BlockNode& node)
{
    BlockSymbol* prevBlock = currentBlock;
    ContainerSymbol* prevContainer = currentContainer;
    if (currentContainer->IsSubroutineSymbol())
    {
        SubroutineSymbol* subroutine = static_cast<SubroutineSymbol*>(currentContainer);
        BlockSymbol* block = new BlockSymbol(currentContainer->Span(), "@" + currentContainer->FullName());
        block->SetLevel(subroutine->Level());
        currentContainer->AddSymbol(block);
        currentBlock = block;
        currentContainer = block;
    }
    for (const auto& declaration : node.Declarations())
    {
        declaration->Accept(*this);
    }
    if (node.CompoundStatement())
    {
        StatementBinder statementBinder(context, currentSubroutine);
        node.CompoundStatement()->Accept(statementBinder);
        currentSubroutine->SetFrameSize(context);
        std::unique_ptr<BoundStatementNode> boundStatement(statementBinder.GetBoundStatement());
        CodeGenerator codeGenerator(context, currentSubroutine, symbolTable->Root()->SourceFilePath());
        boundStatement->Accept(codeGenerator);
    }
    currentBlock = prevBlock;
    currentContainer = prevContainer;
}

void TypeBinder::Visit(ConstantDeclarationNode& node)
{
    ConstantSymbol* constantSymbol = new ConstantSymbol(node.Span(), node.ConstantId()->Str());
    TypeSymbol* type = GetType(node.TypeName(), currentContainer, context);
    constantSymbol->SetType(type);
    Value* value = Evaluate(node.Constant(), context, currentBlock);
    constantSymbol->SetValue(value);
    currentBlock->AddSymbol(constantSymbol);
}

void TypeBinder::Visit(TypeDeclarationNode& node)
{
    IdentifierNode* prevTypeId = typeId;
    typeId = node.TypeId();
    bool prevMakeType = makeType;
    makeType = true;
    node.Type()->Accept(*this);
    makeType = prevMakeType;
    typeId = prevTypeId;
}

void TypeBinder::Visit(VariableDeclarationNode& node)
{
    TypeSymbol* type = GetType(node.TypeName(), currentContainer, context);
    for (const auto& varId : node.IdentifierList()->IdentifierNodes())
    {
        VariableSymbol* variableSymbol = new VariableSymbol(varId->Span(), varId->Str());
        variableSymbol->SetType(type);
        currentContainer->Parent()->AddSymbol(variableSymbol);
    }
}

void TypeBinder::Visit(SubrangeTypeNode& node)
{
    std::string subrangeTypeName;
    if (typeId)
    {
        subrangeTypeName = typeId->Str();
    }
    else
    {
        subrangeTypeName = "@subrange_type_" + util::GetSha1MessageDigest(util::ToString(util::uuid::random()));
    }
    Value* rangeStart = Evaluate(node.RangeStart(), context, currentBlock);
    Value* rangeEnd = Evaluate(node.RangeEnd(), context, currentBlock);
    currentBlock->AddSymbol(new SubrangeTypeSymbol(node.Span(), subrangeTypeName, rangeStart, rangeEnd));
}

void TypeBinder::Visit(EnumeratedTypeNode& node)
{
    std::string enumTypeName;
    if (typeId)
    {
        enumTypeName = typeId->Str();
    }
    else
    {
        enumTypeName = "@enum_type_" + util::GetSha1MessageDigest(util::ToString(util::uuid::random()));
    }
    EnumeratedTypeSymbol* enumTypeSymbol = new EnumeratedTypeSymbol(node.Span(), enumTypeName);
    int32_t index = 0;
    for (const auto& enumConstantId : node.EnumerationConstants()->IdentifierNodes())
    {
        ConstantSymbol* enumConstantSymbol = new ConstantSymbol(enumConstantId->Span(), enumConstantId->Str());
        enumConstantSymbol->SetType(enumTypeSymbol);
        IntegerValue* integerValue = new IntegerValue(index);
        integerValue->SetType(symbolTable->GetType(GetIntegerTypeId(), enumConstantId.get(), context));
        enumConstantSymbol->SetValue(integerValue);
        currentBlock->AddSymbol(enumConstantSymbol);
        enumTypeSymbol->AddEnumConstant(enumConstantSymbol);
        ++index;
    }
    currentBlock->AddSymbol(enumTypeSymbol);
}

void TypeBinder::Visit(FieldListNode& node)
{
}

void TypeBinder::Visit(SubroutineIdNode& node)
{
}

void TypeBinder::Visit(ParameterListNode& node)
{
}

void TypeBinder::Visit(ProcedureHeadingNode& node)
{
    ContainerSymbol* prevContainer = currentContainer;
    IdentifierNode* objectId = node.SubroutineId()->ObjectId();
    bool mustExist = false;
    if (objectId)
    {
        std::string objectTypeName = objectId->Str();
        if (context->InstantiatingGeneric())
        {
            objectTypeName = context->Specialization()->Name();
        }
        ObjectTypeSymbol* objectType = currentContainer->GetObjectType(objectTypeName, &node, context);
        currentContainer = objectType;
        mustExist = true;
    }
    IdentifierNode* subroutineId = node.SubroutineId()->SubroutineId();
    std::vector<std::unique_ptr<ParameterSymbol>> parameters;
    if (currentContainer->IsObjectTypeOrSpecializationSymbol())
    {
        ObjectTypeSymbol* objectType = static_cast<ObjectTypeSymbol*>(currentContainer);
        ParameterSymbol* parameterSymbol = new ParameterSymbol(objectType->Span(), "this");
        parameterSymbol->SetType(objectType);
        parameterSymbol->SetQualifier(ParameterQualifier::valueParam);
        parameters.push_back(std::unique_ptr<ParameterSymbol>(parameterSymbol));
    }
    for (const auto& parameterList : node.ParameterLists())
    {
        TypeSymbol* type = GetType(parameterList->Type(), currentContainer, context);
        for (const auto& parameterId : parameterList->IdentifierList()->IdentifierNodes())
        {
            ParameterSymbol* parameterSymbol = new ParameterSymbol(parameterId->Span(), parameterId->Str());
            parameterSymbol->SetType(type);
            parameterSymbol->SetQualifier(parameterList->Qualifier());
            parameters.push_back(std::unique_ptr<ParameterSymbol>(parameterSymbol));
        }
    }
    ProcedureSymbol* procedureSymbol = currentContainer->GetProcedure(subroutineId->Str(), &node, mustExist);
    if (!procedureSymbol)
    {
        procedureSymbol = new ProcedureSymbol(node.Span(), subroutineId->Str());
        if (context->InstantiatingGeneric())
        {
            procedureSymbol->SetSourceFilePath(node.FilePath());
        }
        currentContainer->AddSymbol(procedureSymbol);
        procedureSymbol->SetVirtual(node.GetVirtual(), &node);
        procedureSymbol->SetParameters(parameters);
    }
    else
    {
        procedureSymbol->CheckParameters(parameters, &node);
    }
    currentContainer = prevContainer;
    currentProcedure = procedureSymbol;
}

void TypeBinder::Visit(FunctionHeadingNode& node)
{
    ContainerSymbol* prevContainer = currentContainer;
    IdentifierNode* objectId = node.SubroutineId()->ObjectId();
    bool mustExist = false;
    if (objectId)
    {
        std::string objectTypeName = objectId->Str();
        if (context->InstantiatingGeneric())
        {
            objectTypeName = context->Specialization()->Name();
        }
        ObjectTypeSymbol* objectType = currentContainer->GetObjectType(objectTypeName, &node, context);
        currentContainer = objectType;
        mustExist = true;
    }
    IdentifierNode* subroutineId = node.SubroutineId()->SubroutineId();
    TypeSymbol* resultType = GetType(node.ResultType(), currentContainer, context);
    std::vector<std::unique_ptr<ParameterSymbol>> parameters;
    if (currentContainer->IsObjectTypeOrSpecializationSymbol())
    {
        ObjectTypeSymbol* objectType = static_cast<ObjectTypeSymbol*>(currentContainer);
        ParameterSymbol* parameterSymbol = new ParameterSymbol(objectType->Span(), "this");
        parameterSymbol->SetType(objectType);
        parameterSymbol->SetQualifier(ParameterQualifier::valueParam);
        parameters.push_back(std::unique_ptr<ParameterSymbol>(parameterSymbol));
    }
    for (const auto& parameterList : node.ParameterLists())
    {
        TypeSymbol* type = GetType(parameterList->Type(), currentContainer, context);
        for (const auto& parameterId : parameterList->IdentifierList()->IdentifierNodes())
        {
            ParameterSymbol* parameterSymbol = new ParameterSymbol(parameterId->Span(), parameterId->Str());
            parameterSymbol->SetType(type);
            parameterSymbol->SetQualifier(parameterList->Qualifier());
            parameters.push_back(std::unique_ptr<ParameterSymbol>(parameterSymbol));
        }
    }
    FunctionSymbol* functionSymbol = currentContainer->GetFunction(subroutineId->Str(), &node, mustExist);
    if (!functionSymbol)
    {
        functionSymbol = new FunctionSymbol(node.Span(), subroutineId->Str());
        if (context->InstantiatingGeneric())
        {
            functionSymbol->SetSourceFilePath(node.FilePath());
        }
        currentContainer->AddSymbol(functionSymbol);
        functionSymbol->SetVirtual(node.GetVirtual(), &node);
        functionSymbol->SetParameters(parameters);
        functionSymbol->SetResultType(resultType);
    }
    else
    {
        functionSymbol->CheckParameters(parameters, &node);
        functionSymbol->CheckResultType(resultType, &node);
    }
    currentContainer = prevContainer;
    currentFunction = functionSymbol;
}

void TypeBinder::Visit(ConstructorHeadingNode& node)
{
    ContainerSymbol* prevContainer = currentContainer;
    IdentifierNode* objectId = node.ObjectId();
    bool mustExist = false;
    if (objectId)
    {
        std::string objectTypeName = objectId->Str();
        if (context->InstantiatingGeneric())
        {
            objectTypeName = context->Specialization()->Name();
        }
        ObjectTypeSymbol* objectType = currentContainer->GetObjectType(objectTypeName, &node, context);
        currentContainer = objectType;
        mustExist = true;
    }
    std::vector<std::unique_ptr<ParameterSymbol>> parameters;
    if (currentContainer->IsObjectTypeOrSpecializationSymbol())
    {
        ObjectTypeSymbol* objectType = static_cast<ObjectTypeSymbol*>(currentContainer);
        ParameterSymbol* parameterSymbol = new ParameterSymbol(objectType->Span(), "this");
        parameterSymbol->SetType(objectType);
        parameterSymbol->SetQualifier(ParameterQualifier::valueParam);
        parameters.push_back(std::unique_ptr<ParameterSymbol>(parameterSymbol));
    }
    for (const auto& parameterList : node.ParameterLists())
    {
        TypeSymbol* type = GetType(parameterList->Type(), currentContainer, context);
        for (const auto& parameterId : parameterList->IdentifierList()->IdentifierNodes())
        {
            ParameterSymbol* parameterSymbol = new ParameterSymbol(parameterId->Span(), parameterId->Str());
            parameterSymbol->SetType(type);
            parameterSymbol->SetQualifier(parameterList->Qualifier());
            parameters.push_back(std::unique_ptr<ParameterSymbol>(parameterSymbol));
        }
    }
    ConstructorSymbol* constructorSymbol = currentContainer->GetOrInsertConstructor(parameters, &node, mustExist);
    if (context->InstantiatingGeneric())
    {
        constructorSymbol->SetSourceFilePath(node.FilePath());
    }
    currentContainer = prevContainer;
    currentConstructor = constructorSymbol;
}

void TypeBinder::Visit(ProcedureDeclarationNode& node)
{
    context->SetCurrentConstructor(nullptr);
    ContainerSymbol* prevContainer = currentContainer;
    ProcedureSymbol* prevProcedure = currentProcedure;
    currentProcedure = nullptr;
    bool generic = false;
    node.Heading()->Accept(*this);
    bool prevExternal = external;
    external = false;
    bool prevForward = forward;
    forward = false;
    if (currentProcedure)
    {
        if (currentProcedure->IsDefined())
        {
            ThrowError("procedure '" + currentProcedure->FullName() + "' already defined", node.FilePath(), node.Span());
        }
        currentProcedure->SetDefined();
        currentContainer = currentProcedure;
        ObjectTypeSymbol* objectType = currentProcedure->GetObjectType();
        if (objectType && objectType->IsGeneric())
        {
            generic = true;
            ProcedureDeclarationNode* clone = static_cast<ProcedureDeclarationNode*>(node.Clone());
            clone->SetFilePath(node.FilePath());
            objectType->AddSubroutineNode(clone);
        }
    }
    else
    {
        ThrowError("procedure declaration expected", node.FilePath(), node.Span());
    }
    if (!generic)
    {
        SubroutineSymbol* prevSubroutine = currentSubroutine;
        currentSubroutine = currentProcedure;
        currentSubroutine->SetLevel(currentBlock->Level() + 1);
        node.SubroutineBlock()->Accept(*this);
        currentSubroutine = prevSubroutine;
        if (external)
        {
            currentProcedure->SetExternal();
            ExternalSubroutineSymbol* externalSubroutine = GetExternalSubroutine(currentProcedure->FullName(), &node);
            currentProcedure->SetExternalSubroutineId(externalSubroutine->Id());
        }
        else if (forward)
        {
            currentProcedure->ResetDefined();
            currentProcedure->SetForward();
        }
    }
    external = prevExternal;
    forward = prevForward;
    currentProcedure = prevProcedure;
    currentContainer = prevContainer;
}

void TypeBinder::Visit(FunctionDeclarationNode& node)
{
    context->SetCurrentConstructor(nullptr);
    ContainerSymbol* prevContainer = currentContainer;
    FunctionSymbol* prevFunction = currentFunction;
    currentFunction = nullptr;
    bool generic = false;
    node.Heading()->Accept(*this);
    bool prevForward = forward;
    forward = false;
    bool prevExternal = external;
    external = false;
    if (currentFunction)
    {
        if (currentFunction->IsDefined())
        {
            ThrowError("function '" + currentFunction->FullName() + "' already defined", node.FilePath(), node.Span());
        }
        currentFunction->SetDefined();
        currentContainer = currentFunction;
        ObjectTypeSymbol* objectType = currentFunction->GetObjectType();
        if (objectType && objectType->IsGeneric())
        {
            generic = true;
            FunctionDeclarationNode* clone = static_cast<FunctionDeclarationNode*>(node.Clone());
            clone->SetFilePath(node.FilePath());
            objectType->AddSubroutineNode(clone);
        }
    }
    else
    {
        ThrowError("function declaration expected", node.FilePath(), node.Span());
    }
    if (!generic)
    {
        TypeSymbol* resultType = currentFunction->ResultType(std::vector<TypeSymbol*>(), &node);
        VariableSymbol* resultVar = new VariableSymbol(node.Span(), "@result");
        resultVar->SetType(resultType);
        currentFunction->AddSymbol(resultVar);
        SubroutineSymbol* prevSubroutine = currentSubroutine;
        currentSubroutine = currentFunction;
        currentSubroutine->SetLevel(currentBlock->Level() + 1);
        node.SubroutineBlock()->Accept(*this);
        currentSubroutine = prevSubroutine;
        if (external)
        {
            currentFunction->SetExternal();
            ExternalSubroutineSymbol* externalSubroutine = GetExternalSubroutine(currentFunction->FullName(), &node);
            currentFunction->SetExternalSubroutineId(externalSubroutine->Id());
        }
        else if (forward)
        {
            currentFunction->ResetDefined();
            currentFunction->SetForward();
        }
    }
    external = prevExternal;
    forward = prevForward;
    currentFunction = prevFunction;
    currentContainer = prevContainer;
}

void TypeBinder::Visit(ConstructorDeclarationNode& node)
{
    context->SetCurrentConstructor(nullptr);
    ContainerSymbol* prevContainer = currentContainer;
    ConstructorSymbol* prevConstructor = currentConstructor;
    currentConstructor = nullptr;
    bool generic = false;
    bool prevForward = forward;
    forward = false;
    bool prevExternal = external;
    external = false;
    node.Heading()->Accept(*this);
    if (currentConstructor)
    {
        if (currentConstructor->IsDefined())
        {
            ThrowError("constructor '" + currentConstructor->FullName() + "' already defined", node.FilePath(), node.Span());
        }
        currentConstructor->SetDefined();
        context->SetCurrentConstructor(currentConstructor);
        currentContainer = currentConstructor;
        ObjectTypeSymbol* objectType = currentConstructor->GetObjectType();
        if (objectType && objectType->IsGeneric())
        {
            generic = true;
            ConstructorDeclarationNode* clone = static_cast<ConstructorDeclarationNode*>(node.Clone());
            clone->SetFilePath(node.FilePath());
            objectType->AddSubroutineNode(clone);
        }
    }
    else
    {
        ThrowError("constructor declaration expected", node.FilePath(), node.Span());
    }
    if (!generic)
    {
        if (node.ConstructorCall())
        {
            currentConstructor->SetConstructorCall(node.ConstructorCall());
        }
        SubroutineSymbol* prevSubroutine = currentSubroutine;
        currentSubroutine = currentConstructor;
        currentSubroutine->SetLevel(currentBlock->Level() + 1);
        node.SubroutineBlock()->Accept(*this);
        currentSubroutine = prevSubroutine;
        if (external)
        {
            currentConstructor->SetExternal();
            ExternalSubroutineSymbol* externalSubroutine = GetExternalSubroutine(currentConstructor->FullName(), &node);
            currentConstructor->SetExternalSubroutineId(externalSubroutine->Id());
        }
        else if (forward)
        {
            currentConstructor->ResetDefined();
            currentConstructor->SetForward();
        }
    }
    external = prevExternal;
    forward = prevForward;
    currentConstructor = prevConstructor;
    currentContainer = prevContainer;
    context->SetCurrentConstructor(nullptr);
}

void TypeBinder::Visit(ForwardNode& node)
{
    forward = true;
}

void TypeBinder::Visit(ExternalNode& node)
{
    external = true;
}

void TypeBinder::Visit(ObjectTypeNode& node)
{
    std::string objectTypeName;
    if (typeId)
    {
        objectTypeName = typeId->Str();
    }
    else
    {
        objectTypeName = "@object_type_" + util::GetSha1MessageDigest(util::ToString(util::uuid::random()));
    }
    ObjectTypeSymbol* objectTypeSymbol = nullptr; 
    if (context->InstantiatingGeneric())
    {
        objectTypeSymbol = context->Specialization();
    }
    else
    {
        objectTypeSymbol = new ObjectTypeSymbol(node.Span(), objectTypeName);
        currentBlock->AddSymbol(objectTypeSymbol);
        if (node.TypeParamId())
        {
            TypeParamSymbol* typeParamSymbol = new TypeParamSymbol(node.TypeParamId()->Span(), node.TypeParamId()->Str());
            objectTypeSymbol->AddSymbol(typeParamSymbol);
            ObjectTypeNode* clone = static_cast<ObjectTypeNode*>(node.Clone());
            clone->SetFilePath(node.FilePath());
            objectTypeSymbol->SetObjectTypeNode(clone);
        }
    }
    if (node.BaseObjectTypeName())
    {
        TypeSymbol* type = GetType(node.BaseObjectTypeName(), currentBlock, context);
        if (type->IsObjectTypeOrSpecializationSymbol())
        {
            ObjectTypeSymbol* baseType = static_cast<ObjectTypeSymbol*>(type);
            objectTypeSymbol->SetBaseType(baseType);
        }
        else
        {
            ThrowError("error: object type expected for base type of '" + objectTypeSymbol->Name() + "'", node.FilePath(), node.BaseObjectTypeName()->Span());
        }
    }
    if (!objectTypeSymbol->IsGeneric())
    {
        TypeSymbol* booleanType = symbolTable->GetType(GetBooleanTypeId(), context);
        AddBinaryOperatorFunction(node.Span(), Operator::equal, objectTypeSymbol, booleanType);
        AddBinaryOperatorFunction(node.Span(), Operator::notEqual, objectTypeSymbol, booleanType);
    }
    ContainerSymbol* prevContainer = currentContainer;
    currentContainer = objectTypeSymbol;
    for (const auto& fieldList : node.FieldLists())
    {
        TypeSymbol* type = GetType(fieldList->TypeName(), currentContainer, context);
        for (const auto& fieldId : fieldList->FieldNames()->IdentifierNodes())
        {
            FieldSymbol* fieldSymbol = new FieldSymbol(fieldId->Span(), fieldId->Str(), type);
            objectTypeSymbol->AddSymbol(fieldSymbol);
        }
    }
    for (const auto& method : node.Methods())
    {
        method->Accept(*this);
    }
    currentContainer = prevContainer;
    objectTypeSymbol->GenerateDefaults(&node, context);
    if (objectTypeSymbol->IsVirtual())
    {
        ObjectTypeSymbol* baseType = objectTypeSymbol->BaseType();
        if (baseType && baseType->IsVirtual())
        {
            objectTypeSymbol->SetVmtPtrFieldIndex(baseType->VmtPtrFieldIndex());
        }
        else
        {
            FieldSymbol* vmtPtrField = new FieldSymbol(objectTypeSymbol->Span(), "@vmt");
            int32_t vmtPtrFieldIndex = objectTypeSymbol->Fields().size();
            objectTypeSymbol->AddSymbol(vmtPtrField);
            objectTypeSymbol->SetVmtPtrFieldIndex(vmtPtrFieldIndex);
        }
    }
}

void TypeBinder::Visit(ArrayTypeNode& node)
{
    std::string arrayTypeName;
    if (typeId)
    {
        arrayTypeName = typeId->Str();
    }
    else
    {
        arrayTypeName = "@array_type_" + util::GetSha1MessageDigest(util::ToString(util::uuid::random()));
    }
    ArrayTypeSymbol* arrayTypeSymbol = new ArrayTypeSymbol(node.Span(), arrayTypeName);
    TypeSymbol* elementType = GetType(node.ElementType(), currentContainer, context);
    arrayTypeSymbol->SetElementType(elementType);
    currentBlock->AddSymbol(arrayTypeSymbol);
}

void TypeBinder::Visit(SpecializationNode& node)
{
    if (typeId)
    {
        context->SetSpecializationName(typeId->Str());
    }
    else
    {
        context->SetSpecializationName(std::string());
    }
    TypeSymbol* specializationType = GetType(&node, currentContainer, context);
    if (specializationType->IsSpecializationSymbol())
    {
        SpecializationSymbol* specialization = static_cast<SpecializationSymbol*>(specializationType);
        if (!specialization->Instantiated())
        {
            specialization->SetInstantiated();
            ObjectTypeSymbol* genericType = specialization->GenericType();
            Node* objectTypeNode = genericType->ObjectTypeNode();
            TypeSymbol* typeArgument = specialization->TypeArgument();
            specialization->AddSymbol(new BoundTypeParamSymbol(node.Span(), genericType->TypeParam()->Name(), typeArgument));
            context->PushInstantiatingGeneric();
            context->PushSpecialization(specialization);
            objectTypeNode->Accept(*this);
            for (const auto& subroutine : genericType->SubroutineNodes())
            {
                subroutine->Accept(*this);
            }
            context->PopSpecialization();
            context->PopInstantiatingGeneric();
        }
        if (typeId)
        {
            context->PushNoThrow();
            TypeSymbol* type = GetType(typeId, currentContainer, context);
            context->PopNoThrow();
            if (!type)
            {
                currentBlock->AddSymbol(new AliasTypeSymbol(node.Span(), typeId->Str(), specialization));
            }
        }
    }
    else
    {
        ThrowError("error: specialization expected", node.FilePath(), node.Span());
    }
}

} // namespace p
