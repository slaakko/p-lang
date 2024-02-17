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
import std.filesystem;
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
    std::cout << "Usage: rp [options] FILE.pcode" << "\n";
    std::cout << "Run p-language pcode program file FILE.pcode" << "\n";
    std::cout << "\n";
    std::cout << "Options:" << "\n";
    std::cout << "\n";
    std::cout << "--help | -h" << "\n";
    std::cout << "  Print help and exit." << "\n";
    std::cout << "\n";
    std::cout << "--verbose | -v" << "\n";
    std::cout << "  Be verbose." << "\n";
    std::cout << "\n";
    std::cout << "--heap-size=SIZE_MB | -s=SIZE_MB" << "\n";
    std::cout << "  Set heap size to SIZE_MB megabytes." << "\n";
    std::cout << "  Default heap size is 16 megabytes." << "\n";
    std::cout << "\n";
}

std::unique_ptr<p::SymbolTable> Read(const std::string& pcodeFilePath, p::Context* context)
{
    p::SymbolReader reader(pcodeFilePath);
    reader.SetContext(context);
    reader.ReadHeader();
    std::unique_ptr<p::SymbolTable> symbolTable(new p::SymbolTable());
    symbolTable->Read(reader);
    symbolTable->Resolve();
    return symbolTable;
}

void Run(const std::string& pcodeFilePath, int32_t heapSize, bool verbose)
{
    if (verbose)
    {
        std::cout << "Running " << pcodeFilePath << "...\n";
    }
    soul::ast::Span span;
    p::Init(pcodeFilePath, span);
    p::Heap heap(heapSize);
    p::ExecutionContext context;
    context.SetHeap(&heap);
    p::UnitLoader loader;
    context.SetUnitLoader(&loader);
    std::unique_ptr<p::SymbolTable> symbolTable = Read(pcodeFilePath, &context);
    context.SetSymbolTable(symbolTable.get());
    p::GlobalVariableMap* globalVariableMap = symbolTable->Root()->GetGlobalVariableMap();
    context.SetGlobalVariableMap(globalVariableMap);
    globalVariableMap->AllocateFrame();
    p::ProcedureSymbol* program = symbolTable->Root()->Block()->GetProcedure("@program");
    if (program)
    {
        symbolTable->RunInits(&context);
        program->Execute(&context);
    }
    else
    {
        throw std::runtime_error("error: module '" + pcodeFilePath + "' does not contain procedure '@program'");
    }
    p::Done();
}

int main(int argc, const char** argv)
{
    try
    {
        InitApplication();
        int32_t heapSize = p::defaultHeapSize;
        std::string filePath;
        bool verbose = false;
        for (int i = 1; i < argc; ++i)
        {
            std::string arg = argv[i];
            if (arg.starts_with("--"))
            {
                if (arg.find('=') != std::string::npos)
                {
                    std::vector<std::string> components = util::Split(arg, '=');
                    if (components.size() == 2)
                    {
                        if (components[0] == "--heap-size")
                        {
                            heapSize = 1024 * 1024 * std::stoi(components[1]);
                        }
                        else
                        {
                            throw std::runtime_error("unknown option '" + arg + "'");
                        }
                    }
                    else
                    {
                        throw std::runtime_error("invalid option '" + arg + "'");
                    }
                }
                else
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
                    else
                    {
                        throw std::runtime_error("unknown option '" + arg + "'");
                    }
                }
            }
            else if (arg.starts_with("-"))
            {
                std::string options(arg.substr(1));
                if (options.find('=') != std::string::npos)
                {
                    std::vector<std::string> components = util::Split(options, '=');
                    if (components.size() == 2)
                    {
                        if (components[0] == "s")
                        {
                            heapSize = 1024 * 1024 * std::stoi(components[1]);
                        }
                        else
                        {
                            throw std::runtime_error("unknown option '" + arg + "'");
                        }
                    }
                    else
                    {
                        throw std::runtime_error("invalid option '" + arg + "'");
                    }
                }
                else
                {
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
                        default:
                        {
                            throw std::runtime_error("unknown option '-" + std::string(1, o) + "'");
                        }
                        }
                    }
                }
            }
            else
            {
                filePath = arg;
            }
        }
        if (!filePath.empty())
        {
            if (!std::filesystem::path(filePath).has_extension())
            {
                filePath.append(".pcode");
            }
            if (!std::filesystem::exists(filePath) && !std::filesystem::path(filePath).has_parent_path())
            {
                filePath = util::Path::Combine(util::Path::Combine(util::PLangRoot(), "prog"), filePath);
            }
            Run(util::GetFullPath(filePath), heapSize, verbose);
        }
        else
        {
            throw std::runtime_error("file path argument not given");
        }
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << "\n";
        return 1;
    }
    TerminateApplication();
}