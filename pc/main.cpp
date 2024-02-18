// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

import p.lexer;
import p.parser;
import p.core;
import p.binder;
import p.ast;
import util;
import std.core;

void InitApplication()
{
    util::Init();
}

void TerminateApplication()
{
    util::Done();
}

void PrintHelp()
{
    std::cout << "Usage: pc [options] { FILE.p }" << "\n";
    std::cout << "Compile p-language file FILE.p to binary file FILE.pcode" << "\n";
    std::cout << "\n";
    std::cout << "Options:" << "\n";
    std::cout << "--help | -h" << "\n";
    std::cout << "  Print help and exit." << "\n";
    std::cout << "--verbose | -v" << "\n";
    std::cout << "  Be verbose." << "\n";
    std::cout << "--rebuild | -r" << "\n";
    std::cout << "  Rebuild units." << "\n";
    std::cout << "--update | -u" << "\n";
    std::cout << "  Update changed units." << "\n";
    std::cout << "\n";
}

void CompileUnit(const std::string& sourceFilePath, p::Context* context)
{
    if (context->Verbose())
    {
        std::cout << "> " << sourceFilePath << "\n";
    }
    std::unique_ptr<p::Node> node = p::ParsePLangSourceFile(sourceFilePath);
    p::TypeBinder typeBinder(context);
    node->Accept(typeBinder);
    p::SymbolTable* symbolTable = typeBinder.GetSymbolTable();
    symbolTable->MakeVmts();
    symbolTable->CheckDefined();
    p::SymbolWriter writer(symbolTable->Root()->PCodeFilePath());
    writer.WriteHeader();
    symbolTable->Write(writer);
    if (context->Verbose())
    {
        std::cout << "==> " << symbolTable->Root()->SourceFilePath() << "\n";
    }
}

void Compile(const std::string& file, bool verbose)
{
    if (verbose)
    {
        std::cout << "> " << file << "\n";
    }
    std::unique_ptr<p::Node> node = p::ParsePLangSourceFile(file);
    p::Init(file, node->Span());
    p::UnitLoader loader;
    p::Context context;
    if (verbose)
    {
        context.SetVerbose();
    }
    context.SetUnitLoader(&loader);
    p::TypeBinder typeBinder(&context);
    node->Accept(typeBinder);
    p::SymbolTable* symbolTable = typeBinder.GetSymbolTable();
    symbolTable->MakeVmts();
    symbolTable->CheckDefined();
    p::SymbolWriter writer(symbolTable->Root()->PCodeFilePath());
    writer.WriteHeader();
    symbolTable->Write(writer);
    p::Done();
    if (verbose)
    {
        std::cout << "==> " << symbolTable->Root()->SourceFilePath() << "\n";
    }
}

int main(int argc, const char** argv)
{
    try
    {
        InitApplication();
        p::ParserLibInit();
        p::SetCompileUnitFunc(CompileUnit);
        p::CompileFlags flags = p::CompileFlags::none;
        bool verbose = false;
        std::vector<std::string> files;
        for (int i = 1; i < argc; ++i)
        {
            std::string arg = argv[i];
            if (arg.starts_with("--"))
            {
                if (arg == "--help")
                {
                    PrintHelp();
                    return 0;
                }
                else if (arg == "--verbose")
                {
                    verbose = true;
                }
                else if (arg == "--rebuild")
                {
                    flags = flags | p::CompileFlags::rebuild;
                }
                else if (arg == "--update")
                {
                    flags = flags | p::CompileFlags::update;
                }
                else
                {
                    throw std::runtime_error("unknown option '" + arg + "'");
                }
            }
            else if (arg.starts_with("-"))
            {
                std::string options = arg.substr(1);
                for (char o : options)
                {
                    switch (o)
                    {
                        case 'h':
                        {
                            PrintHelp();
                            return 0;
                        }
                        case 'v':
                        {
                            verbose = true;
                            break;
                        }
                        case 'r':
                        {
                            flags = flags | p::CompileFlags::rebuild;
                            break;
                        }
                        case 'u':
                        {
                            flags = flags | p::CompileFlags::update;
                            break;
                        }
                        default:
                        {
                            throw std::runtime_error("unknown option '-" + std::string(1, o) + "'");
                        }
                    }
                }
            }
            else
            {
                files.push_back(util::GetFullPath(arg));
            }
        }
        p::SetCompileFlags(flags);
        for (const auto& file : files)
        {
            Compile(file, verbose);
        }
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << "\n";
        return 1;
    }
    TerminateApplication();
    return 0;
}