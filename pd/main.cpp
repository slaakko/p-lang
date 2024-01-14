// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

import p;
import util;
import std.core;

void Print(const std::string& filePath)
{
    p::Heap heap;
    p::ModuleMap moduleMap;
    p::Module mod;
    p::ExecutionContext context;
    context.SetModuleMap(&moduleMap);
    context.SetHeap(&heap);
    mod.Load(filePath, &context);
    moduleMap.AddModule(&mod);
    mod.Resolve(&context);
    if (!mod.GetImplementationPart())
    {
        throw std::runtime_error("error: module '" + filePath + "' does not contain implementation part");
    }
    mod.Print(filePath + ".txt", &context);
}

void PrintHelp()
{
    std::cout << "Usage: pd [options] { FILE.pcode }" << "\n";
    std::cout << "Dump p-language pcode file FILE.pcode to text file FILE.pcode.txt" << "\n";
    std::cout << "Options:" << "\n";
    std::cout << "--help | -h" << "\n";
    std::cout << "  Print help and exit." << "\n";
    std::cout << "--verbose | -v" << "\n";
    std::cout << "  Be verbose." << "\n";
    std::cout << "\n";
}

int main(int argc, const char** argv)
{
    try
    {
        bool verbose = false;
        std::vector<std::string> files;
        for (int i = 1; i < argc; ++i)
        {
            std::string arg = argv[i];
            if (arg.starts_with("--"))
            {
                if (arg == "--verbose")
                {
                    verbose = true;
                }
                else if (arg == "--help")
                {
                    PrintHelp();
                    return 0;
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
                        case 'v':
                        {
                            verbose = true;
                            break;
                        }
                        case 'h':
                        {
                            PrintHelp();
                            return 0;
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
                files.push_back(arg);
            }
        }
        if (files.size() != 1)
        {
            throw std::runtime_error("one file argument expected");
        }
        const std::string& filePath = files[0];
        if (verbose)
        {
            std::cout << "> " << filePath << "\n";
        }
        Print(filePath);
        if (verbose)
        {
            std::cout << "==> " << filePath  + ".txt" << "\n";
        }
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
        return 1;
    }
    return 0;
}
