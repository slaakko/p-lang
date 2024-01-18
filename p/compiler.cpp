// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.compiler;

import p.parsing_context;
import p.lexer;
import p.main_parser;
import p.block;
import p.code;
import p.mod;
import p.execute;
import p.context;
import util;

namespace p {

CompileFlags compileFlags = CompileFlags::none;

void SetCompileFlags(CompileFlags flags)
{
    compileFlags = flags;
}

CompileFlags GetCompileFlags()
{
    return compileFlags;
}


void Compile(const std::string& filePath, bool verbose)
{
    Context context;
    context.SetVerbose(verbose);
    Compile(filePath, &context);
}

void Compile(const std::string& filePath, Context* context)
{
    ModuleMap moduleMap;
    std::string s = util::ReadFile(filePath);
    auto lexer = p::lexer::MakeLexer(s.c_str(), s.c_str() + s.length(), filePath);
    using LexerType = decltype(lexer);
    ParsingContext parsingContext(context);
    parsingContext.SetModuleMap(&moduleMap);
    parsingContext.SetVerbose(context->Verbose());
    p::main_parser::MainParser<LexerType>::Parse(lexer, &parsingContext);
    Module* mod = parsingContext.GetModule();
    moduleMap.AddModule(mod);
    mod->CheckInterface();
    mod->MakeVmts(&parsingContext);
    mod->Save();
    context->AddUpdatedUnits(parsingContext.UpdatedUnits());
    if (context->Verbose())
    {
        std::cout << "==> " << mod->FilePath() << "\n";
    }
}

} // namespace p
