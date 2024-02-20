// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.core.symbol;

import p.core.program_symbol;
import p.core.unit_symbol;
import p.core.unit_part_symbol;
import p.core.block_symbol;
import p.core.type_symbol;
import p.core.object_type_symbol;
import p.core.array_type_symbol;
import p.core.specialization_symbol;
import p.core.alias_type_symbol;
import p.core.container_symbol;
import p.core.constant_symbol;
import p.core.variable_symbol;
import p.core.subroutine_symbol;
import p.core.standard_subroutine;
import p.core.value;

namespace p {

std::string pcodeHeaderMagic = "PCODE2";

std::string SymbolKindStr(SymbolKind kind)
{
    switch (kind)
    {
        case SymbolKind::none: return "none";
        case SymbolKind::programSymbol: return "program";
        case SymbolKind::unitSymbol: return "unit";
        case SymbolKind::unitPartSymbol: return "unit part";
        case SymbolKind::blockSymbol: return "block";
        case SymbolKind::booleanTypeSymbol: return "boolean type";
        case SymbolKind::integerTypeSymbol: return "integer type";
        case SymbolKind::charTypeSymbol: return "char type";
        case SymbolKind::enumeratedTypeSymbol: return "enumerated type";
        case SymbolKind::subrangeTypeSymbol: return "subrange type";
        case SymbolKind::realTypeSymbol: return "real type";
        case SymbolKind::stringTypeSymbol: return "string type";
        case SymbolKind::pointerTypeSymbol: return "pointer type";
        case SymbolKind::nilTypeSymbol: return "nil type";
        case SymbolKind::objectTypeSymbol: return "object type";
        case SymbolKind::arrayTypeSymbol: return "array type";
        case SymbolKind::aliasTypeSymbol: return "alias type";
        case SymbolKind::typeParamSymbol: return "type parameter";
        case SymbolKind::boundTypeParamSymbol: return "bound type parameter";
        case SymbolKind::specializationSymbol: return "specialization";
        case SymbolKind::constantSymbol: return "constant";
        case SymbolKind::variableSymbol: return "variable";
        case SymbolKind::parameterSymbol: return "parameter";
        case SymbolKind::fieldSymbol: return "field";
        case SymbolKind::procedureSymbol: return "procedure";
        case SymbolKind::functionSymbol: return "function";
        case SymbolKind::constructorSymbol: return "constructor";
        case SymbolKind::constructorGroupSymbol: return "constructor group";
        case SymbolKind::externalSubroutineSymbol: return "external subroutine";
        case SymbolKind::ordSymbol: return "ord";
        case SymbolKind::chrSymbol: return "chr";
        case SymbolKind::sinSymbol: return "sin";
        case SymbolKind::cosSymbol: return "cos";
        case SymbolKind::arcTanSymbol: return "arcTan";
        case SymbolKind::sqrtSymbol: return "sqrt";
        case SymbolKind::absSymbol: return "abs";
        case SymbolKind::succSymbol: return "succ";
        case SymbolKind::predSymbol: return "pred";
        case SymbolKind::minSymbol: return "min";
        case SymbolKind::maxSymbol: return "max";
        case SymbolKind::writeSymbol: return "write";
        case SymbolKind::writelnSymbol: return "writeln";
    }
    return std::string("unknown");
}

Symbol::Symbol(SymbolKind kind_, const soul::ast::Span& span_, const std::string& name_) : kind(kind_), span(span_), name(name_), parent(nullptr)
{
}

Symbol::~Symbol()
{
}

const std::string& Symbol::SourceFilePath() const
{
    if (parent)
    {
        return parent->SourceFilePath();
    }
    else
    {
        static std::string empty;
        return empty;
    }
}

std::string Symbol::FullName() const
{
    ContainerSymbol* parent = Parent();
    if (parent && parent->IsObjectTypeOrSpecializationSymbol())
    {
        return parent->Name() + "." + Name();
    }
    else if (parent && parent->IsConstructorGroupSymbol())
    {
        std::string name = Name();
        parent = parent->Parent();
        if (parent && parent->IsObjectTypeOrSpecializationSymbol())
        {
            return parent->Name() + "." + name;
        }
    }
    return Name();
}

void Symbol::Resolve()
{
}

BlockSymbol* Symbol::CurrentBlock() const
{
    ContainerSymbol* p = parent;
    while (p && !p->IsBlockSymbol())
    {
        p = p->parent;
    }
    if (p && p->IsBlockSymbol())
    {
        return static_cast<BlockSymbol*>(p);
    }
    else
    {
        return nullptr;
    }
}

SymbolTable* Symbol::GetSymbolTable() const
{
    if (parent)
    {
        return parent->GetSymbolTable();
    }
    else
    {
        return nullptr;
    }
}

void Symbol::MakeVmts()
{
}

void Symbol::CheckDefined()
{
}

void Symbol::Print(util::CodeFormatter& formatter, bool full, ExecutionContext* context)
{
    std::string fullName = FullName();
    std::string fullNameStr;
    if (fullName != name)
    {
        fullNameStr = "['" + fullName + "']";
    }
    formatter.WriteLine(SymbolKindStr(kind) + " '" + name  + "'" + fullNameStr);
}

Symbol* CreateSymbol(SymbolKind kind, const soul::ast::Span& span, const std::string& name)
{
    switch (kind)
    {
        case SymbolKind::programSymbol: return new ProgramSymbol(span, name);
        case SymbolKind::unitSymbol: return new UnitSymbol(span, name);
        case SymbolKind::unitPartSymbol: return new UnitPartSymbol(span, name);
        case SymbolKind::blockSymbol: return new BlockSymbol(span, name);
        case SymbolKind::booleanTypeSymbol: return new BooleanTypeSymbol(span, name);
        case SymbolKind::integerTypeSymbol: return new IntegerTypeSymbol(span, name);
        case SymbolKind::charTypeSymbol: return new CharTypeSymbol(span, name);
        case SymbolKind::enumeratedTypeSymbol: return new EnumeratedTypeSymbol(span, name);
        case SymbolKind::subrangeTypeSymbol: return new SubrangeTypeSymbol(span, name);
        case SymbolKind::realTypeSymbol: return new RealTypeSymbol(span, name);
        case SymbolKind::stringTypeSymbol: return new StringTypeSymbol(span, name);
        case SymbolKind::pointerTypeSymbol: return new PointerTypeSymbol(span, name);
        case SymbolKind::nilTypeSymbol: return new NilTypeSymbol(span, name);
        case SymbolKind::objectTypeSymbol: return new ObjectTypeSymbol(span, name);
        case SymbolKind::arrayTypeSymbol: return new ArrayTypeSymbol(span, name);
        case SymbolKind::aliasTypeSymbol: return new AliasTypeSymbol(span, name);
        case SymbolKind::typeParamSymbol: return new TypeParamSymbol(span, name);
        case SymbolKind::boundTypeParamSymbol: return new BoundTypeParamSymbol(span, name);
        case SymbolKind::specializationSymbol: return new SpecializationSymbol(span, name);
        case SymbolKind::constantSymbol: return new ConstantSymbol(span, name);
        case SymbolKind::variableSymbol: return new VariableSymbol(span, name);
        case SymbolKind::parameterSymbol: return new ParameterSymbol(span, name);
        case SymbolKind::fieldSymbol: return new FieldSymbol(span, name);
        case SymbolKind::procedureSymbol: return new ProcedureSymbol(span, name);
        case SymbolKind::functionSymbol: return new FunctionSymbol(span, name);
        case SymbolKind::constructorSymbol: return new ConstructorSymbol(span, name);
        case SymbolKind::constructorGroupSymbol: return new ConstructorGroupSymbol(span, name);
        case SymbolKind::ordSymbol: return new Ord(span, name);
        case SymbolKind::chrSymbol: return new Chr(span, name);
        case SymbolKind::sinSymbol: return new Sin(span, name);
        case SymbolKind::cosSymbol: return new Cos(span, name);
        case SymbolKind::arcTanSymbol: return new ArcTan(span, name);
        case SymbolKind::sqrtSymbol: return new Sqrt(span, name);
        case SymbolKind::absSymbol: return new Abs(span, name);
        case SymbolKind::succSymbol: return new Succ(span, name);
        case SymbolKind::predSymbol: return new Pred(span, name);
        case SymbolKind::minSymbol: return new Min(span, name);
        case SymbolKind::maxSymbol: return new Max(span, name);
        case SymbolKind::writeSymbol: return new Write(span, name);
        case SymbolKind::writelnSymbol: return new Writeln(span, name);
    }
    throw std::runtime_error("error: create symbol: unknown symbol kind '" + std::to_string(static_cast<uint8_t>(kind)) + "'");
}

} // namespace p
