// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.core.unit_loader;

import p.core.root_symbol;
import std.core;

export namespace p {

class Context;

class UnitLoader
{
public:
    void ImportUnit(const std::string& unitName, RootSymbol* root, Context* context);
private:
    std::map<std::string, std::unique_ptr<SymbolTable>> unitMap;
};

} // namespace p
