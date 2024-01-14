// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module util.file.stream;

import std.core;
import util.stream;

export namespace util {

enum class OpenMode : int
{
    none = 0,
    read = 1 << 0,
    write = 1 << 1,
    append = 1 << 2,
    binary = 1 << 3
};

inline constexpr OpenMode operator|(OpenMode left, OpenMode right)
{
    return OpenMode(int(left) | int(right));
}

inline constexpr OpenMode operator&(OpenMode left, OpenMode right)
{
    return OpenMode(int(left) & int(right));
}

inline constexpr OpenMode operator~(OpenMode mode)
{
    return OpenMode(~int(mode));
}

class FileStream : public Stream
{
public:
    explicit FileStream(int handle);
    FileStream(const std::string& filePath_, OpenMode openMode);
    ~FileStream() override;
    const std::string& FilePath() const { return filePath; }
    int ReadByte() override;
    int64_t Read(uint8_t* buf, int64_t count) override;
    void Write(uint8_t x) override;
    void Write(uint8_t* buf, int64_t count) override;
    void Flush() override;
    void Seek(int64_t pos, Origin origin) override;
    int64_t Tell() override;
    int64_t Size() const;
private:
    std::string filePath;
    FILE* file;
    bool needToClose;
};

std::string ReadFile(const std::string& filePath);
std::string ReadFile(const std::string& filePath, bool doNotSkipBOM);

} // util
