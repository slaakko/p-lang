// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.context;

import p.mod;
import p.block;
import std.core;

export namespace p {

class CallVirtualInstruction;

class Context
{
public:
    Context();
    ModuleMap* GetModuleMap() const { return moduleMap; }
    void SetModuleMap(ModuleMap* moduleMap_) { moduleMap = moduleMap_; }
    Block* GetBlock() const { return block; }
    void PushBlock(Block* block_);
    void PopBlock();
    void Reset();
    void AddUpdatedUnit(const std::string& updatedUnitName);
    const std::set<std::string>& UpdatedUnits() const { return updatedUnits; }
    void AddUpdatedUnits(const std::set<std::string>& updatedUnits_);
    bool UnitUpdated(const std::string& unitName) const;
    bool UpdateUnits() const { return updateUnits; }
    void SetUpdateUnits() { updateUnits = true; }
    bool Verbose() const { return verbose; }
    void SetVerbose(bool verbose_) { verbose = verbose_; }
private:
    ModuleMap* moduleMap;
    Block* block;
    std::stack<Block*> blockStack;
    std::set<std::string> updatedUnits;
    bool updateUnits;
    bool verbose;
};

} // namespace p
