// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

import soul.xml.dom;
import p.line.token; 
import p.line.lexer;
import util;
import std.core;

using namespace p::line::token;

std::string Version()
{
    return "0.1.0";
}

void PrintHelp()
{
    std::cout << "P Language to HTML converter version " << Version() << "\n";
    std::cout << "Usage: p2html [options] { FILE.p }" << "\n";
    std::cout << "Options:" << "\n";
    std::cout << "--help | -h" << "\n";
    std::cout << "  Print help and exit." << "\n";
    std::cout << "--verbose | -v" << "\n";
    std::cout << "  Be verbose." << "\n";
    std::cout << "--title TITLE | -t TITLE" << "\n";
    std::cout << "  Set title to TITLE." << "\n";
}

std::string KeywordColor()
{
    return "#000000";
}

std::string IdColor()
{
    return "#000000";
}

std::string NumberColor()
{
    return "#000000";
}

std::string StringColor()
{
    return "#a31515";
}

std::string OtherColor()
{
    return "#000000";
}

std::string CommentColor()
{
    return "#57a64a";
}

std::string MakeStyleText(int indent)
{
    std::stringstream sstream;
    util::CodeFormatter formatter(sstream);
    formatter.SetIndentSize(1);
    formatter.WriteLine("div.plang");
    formatter.WriteLine("{");
    formatter.IncIndent();
    formatter.WriteLine("background-color: #e6f7ff;");
    formatter.WriteLine("margin: 20px;");
    formatter.WriteLine("padding: 20px;");
    formatter.WriteLine("font-family: monospace;");
    formatter.DecIndent();
    formatter.WriteLine("}");
    formatter.WriteLine();
    formatter.WriteLine(".plang .kw");
    formatter.WriteLine("{");
    formatter.IncIndent();
    formatter.WriteLine("color: " + KeywordColor() + ";");
    formatter.WriteLine("font-weight: bold;");
    formatter.DecIndent();
    formatter.WriteLine("}");
    formatter.WriteLine();

    formatter.WriteLine(".plang .id");
    formatter.WriteLine("{");
    formatter.IncIndent();
    formatter.WriteLine("color: " + IdColor() + ";");
    formatter.DecIndent();
    formatter.WriteLine("}");
    formatter.WriteLine();

    formatter.WriteLine(".plang .number");
    formatter.WriteLine("{");
    formatter.IncIndent();
    formatter.WriteLine("color: " + NumberColor() + ";");
    formatter.DecIndent();
    formatter.WriteLine("}");
    formatter.WriteLine();

    formatter.WriteLine(".plang .string");
    formatter.WriteLine("{");
    formatter.IncIndent();
    formatter.WriteLine("color: " + StringColor() + ";");
    formatter.DecIndent();
    formatter.WriteLine("}");
    formatter.WriteLine();

    formatter.WriteLine(".plang .other");
    formatter.WriteLine("{");
    formatter.IncIndent();
    formatter.WriteLine("color: " + OtherColor() + ";");
    formatter.DecIndent();
    formatter.WriteLine("}");
    formatter.WriteLine();

    formatter.WriteLine(".plang .comment");
    formatter.WriteLine("{");
    formatter.IncIndent();
    formatter.WriteLine("color: " + CommentColor() + ";");
    formatter.DecIndent();
    formatter.WriteLine("}");
    formatter.WriteLine();

    return sstream.str();
}

std::vector<std::string> GetLines(const std::string& text)
{
    std::vector<std::string> lines;
    std::string line;
    int state = 0;
    for (char c : text)
    {
        switch (state)
        {
            case 0:
            {
                if (c == '\n')
                {
                    lines.push_back(std::move(line));
                    line.clear();
                    state = 1;
                }
                else if (c != '\r')
                {
                    line.append(1, c);
                }
                break;
            }
            case 1:
            {
                if (c == '\n')
                {
                    lines.push_back(std::move(line));
                    line.clear();
                }
                else if (c != '\r')
                {
                    line.append(1, c);
                    state = 0;
                }
                break;
            }
        }
    }
    if (state == 0)
    {
        lines.push_back(std::move(line));
    }
    return lines;
}

void AppendSpace(int count, soul::xml::Element* element)
{
    for (int i = 0; i < count; ++i)
    {
        soul::xml::EntityReference* nbsb = soul::xml::MakeEntityReference("nbsp");
        element->AppendChild(nbsb);
    }
}

template<typename Char>
void AppendSpace(const Char* begin, const Char* end, soul::xml::Element* element)
{
    while (begin != end)
    {
        auto chr = *begin;
        switch (chr)
        {
            case ' ':
            {
                AppendSpace(1, element);
                break;
            }
            case '\t':
            {
                AppendSpace(4, element);
                break;
            }
        }
        ++begin;
    }
}

void AppendKeyword(const std::string& kw, soul::xml::Element* element)
{
    soul::xml::Element* span = soul::xml::MakeElement("span");
    span->SetAttribute("class", "kw");
    soul::xml::Text* text = soul::xml::MakeText(kw);
    span->AppendChild(text);
    element->AppendChild(span);
}

void AppendId(const std::string& id, soul::xml::Element* element)
{
    soul::xml::Element* span = soul::xml::MakeElement("span");
    span->SetAttribute("class", "id");
    soul::xml::Text* text = soul::xml::MakeText(id);
    span->AppendChild(text);
    element->AppendChild(span);
}

void AppendNumber(const std::string& number, soul::xml::Element* element)
{
    soul::xml::Element* span = soul::xml::MakeElement("span");
    span->SetAttribute("class", "number");
    soul::xml::Text* text = soul::xml::MakeText(number);
    span->AppendChild(text);
    element->AppendChild(span);
}

void AppendString(const std::string& string, soul::xml::Element* element)
{
    soul::xml::Element* span = soul::xml::MakeElement("span");
    span->SetAttribute("class", "string");
    soul::xml::Text* text = soul::xml::MakeText(string);
    span->AppendChild(text);
    element->AppendChild(span);
}

void AppendOther(const std::string& other, soul::xml::Element* element)
{
    soul::xml::Element* span = soul::xml::MakeElement("span");
    span->SetAttribute("class", "other");
    soul::xml::Text* text = soul::xml::MakeText(other);
    span->AppendChild(text);
    element->AppendChild(span);
}

void AppendComment(const std::string& comment, soul::xml::Element* element)
{
    soul::xml::Element* span = soul::xml::MakeElement("span");
    span->SetAttribute("class", "comment");
    soul::xml::Text* text = soul::xml::MakeText(comment);
    span->AppendChild(text);
    element->AppendChild(span);
}

template<typename Token>
void ProcessToken(const Token& token, soul::xml::Element* element, bool& inBraceComment, bool& inBlockComment)
{
    if (inBraceComment || inBlockComment)
    {
        AppendComment(token.match.ToString(), element);
        if (token.id == END_BRACE_COMMENT_TOKEN)
        {
            inBraceComment = false;
        }
        else if (token.id == END_BLOCK_COMMENT_TOKEN)
        {
            inBlockComment = false;
        }
    }
    else
    {
        switch (token.id)
        {
            case START_BRACE_COMMENT_TOKEN:
            {
                AppendComment(token.match.ToString(), element);
                inBraceComment = true;
                break;
            }
            case START_BLOCK_COMMENT_TOKEN:
            {
                AppendComment(token.match.ToString(), element);
                inBlockComment = true;
                break;
            }
            case KEYWORD_TOKEN:
            {
                AppendKeyword(token.match.ToString(), element);
                break;
            }
            case IDENTIFIER_TOKEN:
            {
                AppendId(token.match.ToString(), element);
                break;
            }
            case NUMBER_TOKEN:
            {
                AppendNumber(token.match.ToString(), element);
                break;
            }
            case STRING_TOKEN:
            {
                AppendString(token.match.ToString(), element);
                break;
            }
            case OTHER_TOKEN:
            {
                AppendOther(token.match.ToString(), element);
                break;
            }
            case WS_TOKEN:
            {
                AppendSpace(token.match.begin, token.match.end, element);
                break;
            }
        }
    }
}


void Convert(const std::string& content, const std::string& fileName, const std::string& title, bool verbose)
{
    std::vector<std::string> lines = GetLines(content);
    std::string htmlFilePath = fileName + ".html";
    std::string divFilePath = fileName + ".div";
    std::ofstream htmlFile(htmlFilePath);
    util::CodeFormatter htmlFormatter(htmlFile);
    htmlFormatter.SetIndentSize(2);
    std::ofstream divFile(divFilePath);
    util::CodeFormatter divFormatter(divFile);
    divFormatter.SetIndentSize(2);
    std::unique_ptr<soul::xml::Document> htmlDoc = soul::xml::MakeDocument();
    std::unique_ptr<soul::xml::Document> divDoc = soul::xml::MakeDocument();
    htmlFormatter.WriteLine("<!DOCTYPE html>");
    htmlFormatter.WriteLine();
    soul::xml::Element* htmlElement = soul::xml::MakeElement("html");
    htmlElement->SetAttribute("lang", "en");
    htmlElement->SetAttribute("xmlns", "http://www.w3.org/1999/xhtml");
    soul::xml::Element* headElement = soul::xml::MakeElement("head");
    soul::xml::Element* metaElement = soul::xml::MakeElement("meta");
    metaElement->SetAttribute("charset", "utf-8");
    headElement->AppendChild(metaElement);
    soul::xml::Element* titleElement = soul::xml::MakeElement("title");
    soul::xml::Text* titleText = soul::xml::MakeText(title);
    titleElement->AppendChild(titleText);
    headElement->AppendChild(titleElement);
    soul::xml::Element* styleElement = soul::xml::MakeElement("style");
    soul::xml::Text* styleText = soul::xml::MakeText(MakeStyleText(3));
    styleElement->AppendChild(styleText);
    headElement->AppendChild(styleElement);
    htmlElement->AppendChild(headElement);
    soul::xml::Element* bodyElement = soul::xml::MakeElement("body");
    htmlElement->AppendChild(bodyElement);
    soul::xml::Element* divElement = soul::xml::MakeElement("div");
    divElement->SetAttribute("class", "plang");
    bodyElement->AppendChild(divElement);
    soul::xml::Element* rootDivElement = soul::xml::MakeElement("div");
    rootDivElement->SetAttribute("class", "plang");
    bodyElement->AppendChild(divElement);
    htmlDoc->AppendChild(htmlElement);
    divDoc->AppendChild(rootDivElement);
    soul::xml::Element* currentHtmlElement = nullptr;
    soul::xml::Element* currentDivElement = nullptr;
    bool inBraceCommentHtml = false;
    bool inBlockCommentHtml = false;
    bool inBraceCommentDiv = false;
    bool inBlockCommentDiv = false;
    int n = lines.size();
    for (int i = 0; i < n; ++i)
    {
        std::string line = lines[i];
        line.append("\n\n");
        auto tokenLexer = p::line::lexer::MakeLexer(line.c_str(), line.c_str() + line.length(), "");
        soul::xml::Element* htmlLineElement = soul::xml::MakeElement("span");
        htmlLineElement->SetAttribute("xml:space", "preserve");
        currentHtmlElement = htmlLineElement;
        soul::xml::Element* divLineElement = soul::xml::MakeElement("span");
        divLineElement->SetAttribute("xml:space", "preserve");
        currentDivElement = divLineElement;
        ++tokenLexer;
        while (*tokenLexer != soul::lexer::END_TOKEN)
        {
            auto token = tokenLexer.GetToken(tokenLexer.GetPos());
            ProcessToken(token, currentHtmlElement, inBraceCommentHtml, inBlockCommentHtml);
            ProcessToken(token, currentDivElement, inBraceCommentDiv, inBlockCommentDiv);
            ++tokenLexer;
        }
        soul::xml::Element* htmlBrElement = soul::xml::MakeElement("br");
        divElement->AppendChild(htmlLineElement);
        divElement->AppendChild(htmlBrElement);
        soul::xml::Element* divBrElement = soul::xml::MakeElement("br");
        rootDivElement->AppendChild(divLineElement);
        rootDivElement->AppendChild(divBrElement);
    }
    htmlDoc->Write(htmlFormatter);
    divDoc->Write(divFormatter);
    if (verbose)
    {
        std::cout << "==> " << htmlFilePath << std::endl;
        std::cout << "==> " << divFilePath << std::endl;
    }
}

int main(int argc, const char** argv)
{
    try
    {
        bool verbose = false;
        std::vector<std::string> files;
        std::string title;
        bool prevWasTitle = false;
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
                else if (arg == "--title")
                {
                    prevWasTitle = true;
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
                        case 't':
                        {
                            prevWasTitle = true;
                            break;
                        }
                        default:
                        {
                            throw std::runtime_error("unknown option '" + arg + "'");
                        }
                    }
                }
            }
            else if (prevWasTitle)
            {
                prevWasTitle = false;
                title = arg;
            }
            else
            {
                files.push_back(util::GetFullPath(arg));
            }
        }
        for (const auto& fileName : files)
        {
            if (verbose)
            {
                std::cout << "> " << fileName << "\n";
            }
            std::string file = util::ReadFile(fileName);
            if (title.empty())
            {
                title = fileName;
            }
            Convert(file, fileName, title, verbose);
        }
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << "\n";
        return 1;
    }
    return 0;
}