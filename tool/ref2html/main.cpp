// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

import std.core;
import util;
import soul.xml.dom;
import soul.xml.dom.parser;
import soul.xml.xpath;

std::string Version()
{
    return "0.1.0";
}

void PrintHelp()
{
    std::cout << "Reference file to HTML converter version " << Version() << "\n";
    std::cout << "usage: ref2html [options] { FILE.ref }" << "\n";
    std::cout << "options:" << "\n";
    std::cout << "--verbose | -v" << "\n";
    std::cout << "  Be verbose." << "\n";
    std::cout << "--help | -h" << "\n";
    std::cout << "  Print help and exit." << "\n";
}

void ProcessFile(const std::string& fileName, bool verbose)
{
    if (verbose)
    {
        std::cout << "> " << fileName << std::endl;
    }
    std::string root = util::Path::GetDirectoryName(fileName);
    std::unique_ptr<soul::xml::Document> doc = soul::xml::ParseXmlFile(fileName);
    std::unique_ptr<soul::xml::xpath::NodeSet> nodeSet = soul::xml::xpath::EvaluateToNodeSet("//ref", doc.get());
    int n = nodeSet->Count();
    for (int i = 0; i < n; ++i)
    {
        soul::xml::Node* node = nodeSet->GetNode(i);
        if (node->IsElementNode())
        {
            soul::xml::Element* refElement = static_cast<soul::xml::Element*>(node);
            soul::xml::ParentNode* refParent = refElement->Parent();
            if (refParent)
            {
                std::string fileAttr = refElement->GetAttribute("file");
                if (!fileAttr.empty())
                {
                    std::string refencedFilePath = util::GetFullPath(util::Path::Combine(root, fileAttr));
                    std::unique_ptr<soul::xml::Document> referencedDoc = soul::xml::ParseXmlFile(refencedFilePath);
                    std::unique_ptr<soul::xml::Node> rootNode = referencedDoc->RemoveChild(referencedDoc->DocumentElement());
                    refParent->ReplaceChild(rootNode.release(), refElement);
                }
            }
        }
    }
    std::string htmlFileName = util::Path::ChangeExtension(fileName, ".html");
    std::ofstream htmlFile(htmlFileName);
    util::CodeFormatter formatter(htmlFile);
    formatter.SetIndentSize(2);
    formatter.WriteLine("<!DOCTYPE html>");
    formatter.WriteLine();
    doc->Write(formatter);
    if (verbose)
    {
        std::cout << "==> " << htmlFileName << std::endl;
    }
}

int main(int argc, const char** argv)
{
    try
    {
        util::Init();
        bool verbose = false;
        std::vector<std::string> fileNames;
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
                    return 1;
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
                            return 1;
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
                fileNames.push_back(util::GetFullPath(arg));
            }
        }
        for (const auto& fileName : fileNames)
        {
            ProcessFile(fileName, verbose);
        }
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
        return 1;
    }
    util::Done();
    return 0;
}
