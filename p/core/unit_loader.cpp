// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.core.unit_loader;

import p.core.unit_symbol;
import p.core.reader;
import p.core.writer;
import p.core.symbol_table;

namespace p {

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
        std::string pcodeFilePath = GetUnitPCodeFilePath(unitName);
        SymbolReader reader(pcodeFilePath);
        reader.SetContext(context);
        reader.ReadHeader();
        std::unique_ptr<SymbolTable> symbolTable(new SymbolTable());
        reader.SetSymbolTable(symbolTable.get());
        symbolTable->Read(reader);
        symbolTable->Resolve();
        symbolTableToImport = symbolTable.get();
        unitMap[unitName] = std::move(symbolTable);
    }
    root->GetSymbolTable()->Import(symbolTableToImport);
}

} // namespace p
