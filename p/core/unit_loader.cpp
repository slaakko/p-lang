// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.core.unit_loader;

import p.core.unit_symbol;
import p.core.reader;
import p.core.writer;
import p.core.symbol_table;
import p.core.compile_flags;
import p.core.context;

namespace p {

CompileUnitFunc compileUnitFunc = CompileUnitFunc();

void SetCompileUnitFunc(CompileUnitFunc compileUnitFunc_)
{
    compileUnitFunc = compileUnitFunc_;
}

void CompileUnit(const std::string& sourceFilePath, Context* context)
{
    if (compileUnitFunc)
    {
        compileUnitFunc(sourceFilePath, context);
    }
    else
    {
        throw std::runtime_error("compileUnitFunc not set");
    }
}

void UnitLoader::ImportUnit(const std::string& unitName, RootSymbol* root, Context* context)
{
    SymbolTable* symbolTableToImport = nullptr;
    auto it = unitMap.find(unitName);
    if (it != unitMap.end())
    {
        symbolTableToImport = it->second.get();
    }
    else
    {
        if (GetCompileFlag(CompileFlags::rebuild | CompileFlags::update))
        {
            RootSymbol* root = GetUnitHeader(unitName, context);
            if ((GetCompileFlag(CompileFlags::rebuild) || context->UpdateUnits() || !root->IsUpToDate(context)) && !context->UnitUpdated(unitName))
            {
                context->AddUpdatedUnit(unitName);
                CompileUnit(root->SourceFilePath(), context);
                context->SetUpdateUnits();
            }
        }
        std::string pcodeFilePath = GetUnitPCodeFilePath(unitName);
        SymbolReader reader(pcodeFilePath);
        reader.SetContext(context);
        reader.ReadHeader();
        std::unique_ptr<SymbolTable> symbolTable(new SymbolTable());
        context->SetSymbolTable(symbolTable.get());
        reader.SetSymbolTable(symbolTable.get());
        symbolTable->Read(reader);
        symbolTable->Resolve();
        symbolTableToImport = symbolTable.get();
        unitMap[unitName] = std::move(symbolTable);
    }
    root->GetSymbolTable()->Import(symbolTableToImport);
}

RootSymbol* UnitLoader::GetUnitHeader(const std::string& unitName, Context* context)
{
    auto it = headerMap.find(unitName);
    if (it != headerMap.end())
    {
        return it->second.get();
    }
    else
    {
        std::string pcodeFilePath = GetUnitPCodeFilePath(unitName);
        SymbolReader reader(pcodeFilePath);
        reader.SetContext(context);
        reader.ReadHeader();
        reader.SetReadOnlyHeader();
        std::unique_ptr<SymbolTable> symbolTable(new SymbolTable());
        reader.SetSymbolTable(symbolTable.get());
        RootSymbol* root = static_cast<RootSymbol*>(reader.ReadSymbol(nullptr));
        headerMap[unitName] = std::unique_ptr<RootSymbol>(root);
        return root;
    }
}

} // namespace p
