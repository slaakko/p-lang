// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

import p.parser;
import p.core;
import p.ast;
import util;
import std.core;

void Write(const std::string& astFilePath, p::Node* node)
{
    p::AstWriter writer(astFilePath);
    writer.WriteNode(node);
}

std::unique_ptr<p::Node> Read(const std::string& astFilePath)
{
    p::AstReader reader(astFilePath);
    std::unique_ptr<p::Node> node(reader.ReadNode());
    return node;
}

void Process(const std::string& file)
{
    std::unique_ptr<p::Node> node = p::ParsePLangSourceFile(file);
    std::string astFilePath = file + ".ast";
    Write(astFilePath, node.get());
    std::unique_ptr<p::Node> readNode(Read(astFilePath));
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
