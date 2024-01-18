// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

import p;
import util;
import std.core;

void PrintHelp()
{
    std::cout << "Usage: pc [options] { FILE.p }" << "\n";
    std::cout << "Compile p-language file FILE.p to binary file FILE.pcode" << "\n";
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

int main(int argc, const char** argv)
{
    try
    {
        bool verbose = false;
        std::vector<std::string> files;
        p::CompileFlags compileFlags = p::CompileFlags::none;
        for (int i = 1; i < argc; ++i)
        {
            std::string arg = argv[i];
            if (arg.starts_with("--"))
            {
                if (arg == "--verbose")
                {
                    verbose = true;
                }
                if (arg == "--update")
                {
                    compileFlags = compileFlags | p::CompileFlags::update;
                }
                else if (arg == "--rebuild")
                {
                    compileFlags = compileFlags | p::CompileFlags::rebuild;
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
                        case 'u':
                        {
                            compileFlags = compileFlags | p::CompileFlags::update;
                            break;
                        }
                        case 'r':
                        {
                            compileFlags = compileFlags | p::CompileFlags::rebuild;
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
            p::SetCompileFlags(compileFlags);
            p::Compile(file, verbose);
        }
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
        return 1;
    }
    return 0;
}
