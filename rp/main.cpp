// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

#include <Windows.h>

import p;
import std.core;
import util;

void InitApplication()
{
    util::Init();
}

void TerminateApplication()
{
    util::Done();
}

void Run(const std::string& filePath, int32_t heapSize, bool verbose)
{
    p::Init();
    p::Heap heap(heapSize);
    p::ModuleMap moduleMap;
    p::Module mod;
    p::ExecutionContext context;
    context.SetModuleMap(&moduleMap);
    context.SetHeap(&heap);
    mod.Load(filePath, &context);
    moduleMap.AddModule(&mod);
    if (!mod.GetImplementationPart())
    {
        throw std::runtime_error("error: module '" + filePath + "' does not contain implementation part");
    }
    p::Block* block = mod.GetImplementationPart()->GetBlock();
    p::Procedure* program = block->GetProcedure("@program");
    if (!program)
    {
        throw std::runtime_error("error: module '" + filePath + "' does not contain procedure '@program'");
    }
    context.SetGlobalVariableMap(block->GetGlobalVariableMap());
    context.GetGlobalVariableMap()->AllocateFrame();
    context.PushBlock(block);
    program->Resolve(&context);
    mod.RunInits(&context);
    program->Execute(&context);
    context.PopBlock();
    p::Done();
    if (verbose)
    {
        std::cout << "Heap size was " << heapSize / 1024 / 1024 << " MB." << "\n";
        std::cout << "Collected " << heap.CollectionCount() << " times." << "\n";
    }
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
            Run(filePath, heapSize, verbose);
        }
        else
        {
            throw std::runtime_error("file path argument not given");
        }
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
        return 1;
    }
    TerminateApplication();
    return 0;
}