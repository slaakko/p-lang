// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.core.object_type_symbol;

import p.core.symbol;
import p.core.type_symbol;
import p.core.vmt;
import util;
import soul.ast.span;
import std.core;

export namespace p {

enum class ObjectTypeFlags
{
    none = 0, isVirtual = 1 << 0, vmtComputed = 1 << 1, resolved = 1 << 2
};

constexpr ObjectTypeFlags operator|(ObjectTypeFlags left, ObjectTypeFlags right)
{
    return ObjectTypeFlags(int(left) | int(right));
}

constexpr ObjectTypeFlags operator&(ObjectTypeFlags left, ObjectTypeFlags right)
{
    return ObjectTypeFlags(int(left) & int(right));
}

constexpr ObjectTypeFlags operator~(ObjectTypeFlags flags)
{
    return ObjectTypeFlags(~int(flags));
}

class FieldSymbol : public Symbol
{
public:
    FieldSymbol(const soul::ast::Span& span_, const std::string& name_);
    FieldSymbol(const soul::ast::Span& span_, const std::string& name_, TypeSymbol* type_);
    TypeSymbol* Type() const { return type; }
    void SetIndex(int32_t index_) { index = index_; }
    int32_t Index() const { return index; }
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void Resolve() override;
    void Print(util::CodeFormatter& formatter, bool full, ExecutionContext* context) override;
private:
    TypeSymbol* type;
    util::uuid typeId;
    int32_t index;
};

class ObjectTypeSymbol : public TypeSymbol
{
public:
    ObjectTypeSymbol(const soul::ast::Span& span_, const std::string& name_);
    void AddSymbol(Symbol* symbol) override;
    Symbol* GetSymbol(const std::string& symbolName, Node* node, bool searchBase, bool mustExist) const override;
    void SetBaseType(ObjectTypeSymbol* baseType_);
    ObjectTypeSymbol* BaseType() const { return baseType; }
    void AddFields(ObjectTypeSymbol* objectTypeSymbol);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void Resolve() override;
    void MakeVmts() override;
    Vmt* GetVmtPtr() const { return const_cast<Vmt*>(&vmt); }
    ObjectTypeFlags Flags() const { return flags; }
    bool IsVirtual() const { return (flags & ObjectTypeFlags::isVirtual) != ObjectTypeFlags::none; }
    void SetVirtual() { flags = flags | ObjectTypeFlags::isVirtual; }
    int32_t VmtPtrFieldIndex() const { return vmtPtrFieldIndex; }
    void SetVmtPtrFieldIndex(int32_t vmtPtrFieldIndex_) { vmtPtrFieldIndex = vmtPtrFieldIndex_; }
    const std::vector<FieldSymbol*>& Fields() const { return fields; }
    const std::vector<SubroutineSymbol*>& Methods() const { return methods; }
    int32_t GetFieldIndex(const std::string& fieldName) const;
    int32_t GetFieldIndexNoThrow(const std::string& fieldName) const;
    FieldSymbol* GetField(int32_t fieldIndex) const;
    ConstructorSymbol* GetDefaultConstructor() const;
    void GenerateDefaults(Node* node, Context* context);
    bool IsSameOrHasBaseType(ObjectTypeSymbol* objectType) const;
    void Print(util::CodeFormatter& formatter, bool full, ExecutionContext* context) override;
private:
    ObjectTypeFlags flags;
    ObjectTypeSymbol* baseType;
    util::uuid baseTypeId;
    int32_t vmtPtrFieldIndex;
    std::vector<FieldSymbol*> fields;
    std::vector<SubroutineSymbol*> methods;
    Vmt vmt;
};

} // namespace p
