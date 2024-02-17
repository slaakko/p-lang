// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.ast.error;

import util;

namespace p {

std::string ReadFileContent(const std::string& filePath)
{
    return util::ReadFile(filePath);
}

std::vector<int> ComputeLineStartIndeces(const std::string& content)
{
    std::vector<int> indeces;
    indeces.push_back(0);
    const char* start = content.c_str();
    const char* end = content.c_str() + content.size();
    const char* pos = start;
    bool startOfLine = true;
    while (pos != end)
    {
        if (startOfLine)
        {
            indeces.push_back(static_cast<int>(pos - start));
        }
        startOfLine = *pos == '\n';
        ++pos;
    }
    indeces.push_back(static_cast<int>(end - start));
    return indeces;
}

std::string ErrorLines(const std::string& content, const std::vector<int>& lineStarts, const soul::ast::LineColLen& lineColLen)
{
    int lineNumber = lineColLen.line;
    int start = lineStarts[lineNumber];
    std::string lines;
    if (lineNumber < lineStarts.back())
    {
        int end = lineStarts[lineNumber + 1];
        lines = content.substr(start, end - start);
    }
    else
    {
        lines = content.substr(start);
    }
    lines = util::TrimEnd(lines);
    lines.append(1, '\n');
    if (lineColLen.col > 1)
    {
        lines.append(lineColLen.col - 1, ' ');
    }
    lines.append(lineColLen.len, '^');
    return lines;
}

void ThrowError(const std::string& message, const std::string& filePath, const soul::ast::Span& span)
{
    std::string msg(message);
    std::string fileContent = ReadFileContent(filePath);
    std::vector<int> lineStarts = ComputeLineStartIndeces(fileContent);
    soul::ast::LineColLen lineColLen = soul::ast::SpanToLineColLen(span, lineStarts);
    msg.append(":\nat ").append(filePath).append(" line ").append(std::to_string(lineColLen.line).append(1, '\n').append(ErrorLines(fileContent, lineStarts, lineColLen)));
    throw std::runtime_error(msg);
}

} // namespace p
