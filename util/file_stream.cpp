// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <cstdio>
#include <errno.h>

module util.file.stream;

import std.core;
import std.filesystem;
import util.text.util;
import util.error;

namespace util {

FileStream::FileStream(int handle) : filePath(), file(nullptr), needToClose(false)
{
    switch (handle)
    {
        case 0:
        {
            file = stdin;
            filePath = "STDIN";
            break;
        }
        case 1:
        {
            file = stdout;
            filePath = "STDOUT";
            break;
        }
        case 2:
        {
            file = stderr;
            filePath = "STDERR";
            break;
        }
        default:
        {
            throw std::runtime_error("invalid handle value");
        }
    }
}

FileStream::FileStream(const std::string& filePath_, OpenMode openMode) : filePath(filePath_), file()
{
    std::string mode;
    if ((openMode & OpenMode::read) != OpenMode::none && (openMode & OpenMode::write) == OpenMode::none)
    {
        mode += "r";
    }
    if ((openMode & OpenMode::write) != OpenMode::none)
    {
        mode += "w";
        if ((openMode & OpenMode::read) != OpenMode::none)
        {
            mode += "+";
        }
    }
    if ((openMode & OpenMode::append) != OpenMode::none)
    {
        if ((openMode & (OpenMode::read | OpenMode::write)) != OpenMode::none)
        {
            std::string msg("could not open file '");
            msg.append(filePath).append("': open mode not supported");
            throw std::runtime_error(msg);
        }
        else
        {
            mode += "a";
        }
    }
    if ((openMode & OpenMode::binary) != OpenMode::none)
    {
        mode += "b";
    }
    std::string nativeFilePath = Utf8StringToPlatformString(filePath);
#ifdef _WIN32
    errno_t error = fopen_s(&file, nativeFilePath.c_str(), mode.c_str());
    if (error)
    {
        char buf[4096];
        strerror_s(buf, sizeof(buf), error);
        throw std::runtime_error("could not open file '" + std::string(filePath) + "': " + PlatformStringToUtf8(buf));
    }
    needToClose = true;
#elif defined(__linux) || defined(__unix) || defined(__posix) || defined(OTAVA)
    file = std::fopen(nativeFilePath.c_str(), mode.c_str());
    if (!file)
    { 
        std::string msg("could not open file '");
        msg.append(filePath).append("': ").append(std::strerror(ErrorNumber()));
        throw std::runtime_error(msg);
    }
    needToClose = true;
#else

#error unknown platform

#endif
}

FileStream::~FileStream()
{
    if (file && needToClose)
    {
        std::fclose(file);
    }
}

int FileStream::ReadByte()
{
    int result = std::fgetc(file);
    if (result == EOF)
    {
        if (std::feof(file))
        {
            return -1;
        }
        else
        {
#ifdef _WIN32
            char buf[4096];
            strerror_s(buf, sizeof(buf), ErrorNumber());
            throw std::runtime_error("could not read from file '" + filePath + "': " + PlatformStringToUtf8(buf));
#elif defined(__linux) || defined(__unix) || defined(__posix) || defined(OTAVA)
            std::string msg("could not read from file '");
            msg.append(filePath).append(std::strerror(ErrorNumber()));
            throw std::runtime_error(msg);
#else
#error unknown platform
#endif
        }
    }
    SetPosition(Position() + 1);
    return result;
}

int64_t FileStream::Read(uint8_t* buf, int64_t count)
{
    int64_t result = std::fread(buf, 1, count, file);
    if (std::ferror(file))
    {
#ifdef _WIN32
        char buf[4096];
        strerror_s(buf, sizeof(buf), ErrorNumber());
        throw std::runtime_error("could not read from file '" + filePath + "': " + PlatformStringToUtf8(buf));
#elif defined(__linux) || defined(__unix) || defined(__posix) || defined(OTAVA)
        std::string msg("could not read from file '");
        msg.append(filePath).append(std::strerror(ErrorNumber()));
        throw std::runtime_error(msg);
#else
#error unknown platform
#endif
    }
    SetPosition(Position() + result);
    return result;
}

void FileStream::Write(uint8_t x)
{
    int result = std::fputc(x, file);
    if (result == EOF)
    {
#ifdef _WIN32
        char buf[4096];
        strerror_s(buf, sizeof(buf), ErrorNumber());
        throw std::runtime_error("could not write to file '" + filePath + "': " + PlatformStringToUtf8(buf));
#elif defined(__linux) || defined(__unix) || defined(__posix) || defined(OTAVA)
        std::string msg("could not write to file '");
        msg.append(filePath).append(std::strerror(ErrorNumber()));
        throw std::runtime_error(msg);
#else
#error unknown platform
#endif
    }
    SetPosition(Position() + 1);
}

void FileStream::Write(uint8_t* buf, int64_t count)
{
    int64_t result = std::fwrite(buf, 1, count, file);
    if (result != count)
    {
#ifdef _WIN32
        char buf[4096];
        strerror_s(buf, sizeof(buf), ErrorNumber());
        throw std::runtime_error("could not write to file '" + filePath + "': " + PlatformStringToUtf8(buf));
#elif defined(__linux) || defined(__unix) || defined(__posix) || defined(OTAVA)
        std::string msg("could not write to file '");
        msg.append(filePath).append(std::strerror(ErrorNumber()));
        throw std::runtime_error(msg);
#else
#error unknown platform
#endif
    }
    SetPosition(Position() + result);
}

void FileStream::Flush()
{
    int result = std::fflush(file);
    if (result != 0)
    {
#ifdef _WIN32
        char buf[4096];
        strerror_s(buf, sizeof(buf), ErrorNumber());
        throw std::runtime_error("could not flush file '" + filePath + "': " + PlatformStringToUtf8(buf));
#elif defined(__linux) || defined(__unix) || defined(__posix) || defined(OTAVA)
        std::string msg("could not flush file '");
        msg.append(filePath).append(std::strerror(ErrorNumber()));
        throw std::runtime_error(msg);
#else
#error unknown platform
#endif
    }
}

void FileStream::Seek(int64_t pos, Origin origin)
{
    int seekOrigin = SEEK_SET;
    switch (origin)
    {
    case Origin::seekCur:
    {
        seekOrigin = SEEK_CUR;
        break;
    }
    case Origin::seekEnd:
    {
        seekOrigin = SEEK_END;
        break;
    }
    case Origin::seekSet:
    {
        seekOrigin = SEEK_SET;
        break;
    }
    }
#ifdef _WIN32
    int result = _fseeki64(file, pos, seekOrigin);
#else
    int result = std::fseek(file, pos, seekOrigin);
#endif
    if (result != 0)
    {
#ifdef _WIN32
        char buf[4096];
        strerror_s(buf, sizeof(buf), ErrorNumber());
        throw std::runtime_error("could not seek file '" + filePath + "': " + PlatformStringToUtf8(buf));
#elif defined(__linux) || defined(__unix) || defined(__posix) || defined(OTAVA)
        std::string msg("could not seek file '");
        msg.append(filePath).append(std::strerror(ErrorNumber()));
        throw std::runtime_error(msg);
#else
#error unknown platform
#endif
    }
    switch (origin)
    {
    case Origin::seekCur:
    {
        SetPosition(Position() + pos);
        break;
    }
    case Origin::seekEnd:
    {
        SetPosition(Size() + pos);
        break;
    }
    case Origin::seekSet:
    {
        SetPosition(pos);
        break;
    }
    }
}

int64_t FileStream::Tell()
{
    int64_t result = std::ftell(file);
    if (result == -1)
    {
#ifdef _WIN32
        char buf[4096];
        strerror_s(buf, sizeof(buf), ErrorNumber());
        throw std::runtime_error("could not tell file '" + filePath + "': " + PlatformStringToUtf8(buf));
#elif defined(__linux) || defined(__unix) || defined(__posix) || defined(OTAVA)
        std::string msg("could not tell file '");
        msg.append(filePath).append(std::strerror(ErrorNumber()));
        throw std::runtime_error(msg);
#else
#error unknown platform
#endif
    }
    return result;
}

int64_t FileStream::Size() const
{
    std::filesystem::path path(filePath);
    return std::filesystem::file_size(path);
}

std::string ReadFile(const std::string& filePath)
{
    return ReadFile(filePath, false);
}

std::string ReadFile(const std::string& filePath, bool doNotSkipBOM)
{
    std::string s;
    FileStream fs(filePath, OpenMode::read | OpenMode::binary);
    int64_t n = fs.Size();
    for (int64_t i = 0; i < n; ++i)
    {
        int c = fs.ReadByte();
        bool skip = false;
        if (!doNotSkipBOM && i <= 2)
        {
            if (i == 0 && c == 0xEF)
            {
                skip = true;
            }
            else if (i == 1 && c == 0xBB)
            {
                skip = true;
            }
            else if (i == 2 && c == 0xBF)
            {
                skip = true;
            }
        }
        if (!skip)
        {
            if (c != -1)
            {
                s.append(1, c);
            }
            else
            {
                std::string msg("unexpected end of '");
                msg.append(filePath).append("'");
                throw std::runtime_error(msg);
            }
        }
    }
    return s;
}

} // util
