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

void Write(const std::string& pcodeFilePath, p::SymbolTable* symbolTable)
{
    p::SymbolWriter writer(pcodeFilePath);
    writer.WriteHeader();
    symbolTable->Write(writer);
}

std::unique_ptr<p::SymbolTable> Read(const std::string& pcodeFilePath)
{
    p::SymbolReader reader(pcodeFilePath);
    p::UnitLoader loader;
    p::Context context;
    context.SetUnitLoader(&loader);
    reader.SetContext(&context);
    reader.ReadHeader();
    std::unique_ptr<p::SymbolTable> symbolTable(new p::SymbolTable());
    symbolTable->Read(reader);
    symbolTable->Resolve();
    return symbolTable;
}

void Process(const std::string& file)
{
    std::unique_ptr<p::Node> node = p::ParsePLangSourceFile(file);
    p::Init(file, node->Span());
    p::UnitLoader loader;
    p::Context context;
    context.SetUnitLoader(&loader);
    p::TypeBinder typeBinder(&context);
    node->Accept(typeBinder);
    p::SymbolTable* symbolTable = typeBinder.GetSymbolTable();
    symbolTable->MakeVmts();
    symbolTable->CheckDefined();
    Write(symbolTable->Root()->PCodeFilePath(), symbolTable);
    std::unique_ptr<p::SymbolTable> readSymbolTable = Read(symbolTable->Root()->PCodeFilePath());
    p::Done();
}

int main(int argc, const char** argv)
{
    try
    {
        p::ParserLibInit();
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
        for (const auto& file : files)
        {
            if (verbose)
            {
                std::cout << "> " << file << "\n";
            }
            Process(file);
        }
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << "\n";
        return 1;
    }
    return 0;
}