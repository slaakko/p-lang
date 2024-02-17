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

void Print(const std::string& filePath, bool verbose)
{
    std::string txtFilePath = filePath + ".txt";
    std::ofstream txtFile(txtFilePath);
    util::CodeFormatter formatter(txtFile);
    formatter.SetIndentSize(2);
    if (verbose)
    {
        std::cout << "Printing " << filePath << " to " << txtFilePath << "...\n";
    }
    soul::ast::Span span;
    p::Init(filePath, span);
    p::ExecutionContext context;
    p::UnitLoader loader;
    context.SetUnitLoader(&loader);
    std::unique_ptr<p::SymbolTable> symbolTable = Read(filePath, &context);
    context.SetSymbolTable(symbolTable.get());
    symbolTable->Print(formatter, &context);
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
        InitApplication();
        bool verbose = false;
        std::vector<std::string> filePaths;
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
                else
                {
                    throw std::runtime_error("unknown option '" + arg + "'");
                }
            }
            else if (arg.starts_with("-"))
            {
                std::string options(arg.substr(1));
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
            else
            {
                filePaths.push_back(arg);
            }
        }
        if (filePaths.empty())
        {
            throw std::runtime_error("file path argument not given");
        }
        for (const auto& filePath : filePaths)
        {
            Print(util::GetFullPath(filePath), verbose);
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