// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.rt;

import p.subroutine;
import std.core;

export namespace p {

class ExternalSubroutine : public Subroutine
{
public:
    ExternalSubroutine(const std::string& name_);
    const std::string& Name() const { return name; }
    int32_t Id() const { return id; }
    void SetId(int32_t id_) { id = id_; }
private:
    std::string name;
    int32_t id;
};

ExternalSubroutine* GetExternalSubroutine(const std::string& externalSubroutineName);
ExternalSubroutine* GetExternalSubroutine(int32_t id);

void Init();
void Done();

} // namespace p
